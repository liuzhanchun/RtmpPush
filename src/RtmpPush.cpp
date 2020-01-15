//============================================================================
// Name        : RtmpPush.cpp
// Author      : liuzhanchun
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "srs_librtmp.hpp"
#include "srs_lib_ts_demuxer.hpp"
#include <stdio.h>


#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#endif

#define PUSH_FILE 1

#define TIMEOUT 5000
#define TS_PACKET_SIZE 1024

//const char* filename = "testts.ts";
//const char* filename = "zmlx.ts";


int64_t getNowMicro()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

static srs_rtmp_t _rtmp = NULL;
static srs_ts_demuxer_t _ts_demux = NULL;

static int _first_video_pts = 0;
static int _first_video_dts = 0;
static int _first_audio_ts = 0;

static int64_t _first_send_time = 0;

static int open_url(const char* rtmp_url)
{
    int ret = 0;
    _rtmp = srs_rtmp_create(rtmp_url);
    if (!_rtmp) {
        return -1;
    }

    srs_rtmp_set_timeout(_rtmp, TIMEOUT, TIMEOUT);

    ret = srs_rtmp_handshake(_rtmp);
    if (ret) {
        return ret;
    }
    ret = srs_rtmp_connect_app(_rtmp);
    if (ret) {
        return ret;
    }

    return srs_rtmp_publish_stream(_rtmp);
}

static void cleanup()
{
    if (_rtmp) {
        srs_rtmp_destroy(_rtmp);
        _rtmp = NULL;
    }
	if(_ts_demux) {
		srs_ts_demuxer_destroy(_ts_demux);
		_ts_demux = NULL;
	}
}

static int on_parsed_video(char* data, int nb_data, int pts, int dts,void* user_data)
{
    if (_first_video_pts <= 0) {
        _first_video_pts = pts;
    }
    if (_first_video_dts <= 0) {
        _first_video_dts = dts;
    }

    pts -= _first_video_pts;
    dts -= _first_video_dts;

#if PUSH_FILE
    if (_first_send_time > 0) {
        int64_t cur_time = 0;
        while (1) {
            cur_time = getNowMicro();
            int64_t delay = _first_send_time + dts * 1000 - cur_time;
            if (delay <= 0) {
                break;
            }

            printf("h264 delay: %d\n",delay);
            usleep(delay);
        }
    } else {
        _first_send_time = getNowMicro();
    }
#endif /* PUSH_FILE */

    printf("push h264,pts: %d,dts: %d,size: %d\n",pts,dts,nb_data);

    return srs_h264_write_raw_frames(_rtmp, data, nb_data, dts, pts);
}

static int on_parsed_audio(char* data, int nb_data, int dts,void* user_data)
{
    if (_first_audio_ts <= 0) {
        _first_audio_ts = dts;
    }

    dts -= _first_audio_ts;

#if PUSH_FILE

    if (_first_send_time > 0) {
        int64_t cur_time = 0;
        while (1) {
            cur_time = getNowMicro();
            int64_t delay = _first_send_time + dts * 1000 - cur_time;
            if (delay <= 0) {
                break;
            }

            printf("aac delay: %d\n",delay);
            usleep(delay);
        }
    } else {
        _first_send_time = getNowMicro();
    }

#endif /* PUSH_FILE */

    printf("push aac, ts: %d,size: %d\n",dts,nb_data);
    return srs_adts_aac_write_raw_frame(_rtmp, data, nb_data, dts);
}

int main(int argc, char** argv)
{
    if(argc < 3) {
        printf("USAGE: ts2rtmp filename rtmpurl\n");
        return 0;
    }

    FILE* pfile = fopen(argv[1], "rb");
    if (!pfile) {
        printf("connot open file: %s\n",argv[1]);
        return -1;
    }

	//std::string url("rtmp://192.168.8.202:1935/live/demo");

	int ret = open_url(argv[2]);
	if(ret) {
        printf("connect rtmp url(%s) failed,ret = %d\n",argv[2],ret);
		return ret;
	}

	_ts_demux = srs_ts_demuxer_create(on_parsed_video,on_parsed_audio,NULL);

    char buf[TS_PACKET_SIZE] = { 0 };
    size_t nb_read = 0;

    int step_count = 0;
    int read_count = 0;

    while ((nb_read = fread(buf, 1, TS_PACKET_SIZE, pfile)) > 0) {
        ++read_count;
        if (--step_count > 0) {
            continue;
        }
        ret = srs_ts_demuxer_demux(_ts_demux,buf, nb_read);
        printf(" read_count= %d\n",read_count);
        if (ret) {
            printf("ts demuxer demux failed,ret = %d\n",ret);
            return ret;
        }
    }

	cleanup();

    return 0;
}




