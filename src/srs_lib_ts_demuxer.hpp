#ifndef SRS_LIB_TS_DEMUXER
#define SRS_LIB_TS_DEMUXER

/**
* TS流解封装接口，输入TS流，输出视频和音频
* 目前视频只支持H264,音频只支持ADTS AAC格式
* 阿里云RTMP推流貌似支持H264 + AAC
*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void* srs_ts_demuxer_t;

/**
* 解封装后视频原始数据回调接口
* 目前只支持h264
* @param data，原始视频数据，目前为h264，包含00 00 00 01分隔符
* @param nb_data，视频数据大小
* @param pts，dts, 视频相关时间戳，已转换为实际时间戳
* @param user_data，创建时，用户传入的私有数据
* @return 执行成功返回0，否则返回非0错误码
* @remark pts和dts初始值不一定为0，因此在推流到服务器时，需要
*     记录初始pts和dts值，然后后续回调时间戳依次减去初始值即为推流所需时间戳
*/
typedef int (*ts_demuxed_video_h)(char* data,int nb_data,int pts,int dts,void* user_data);

/**
* 解封装后音频原始数据回调接口
* 目前只支持ADTS AAC
* @param data，原始音频数据，目前为AAC，包含ADTS头
* @param nb_data，音频数据大小
* @param ts，音频时间戳
* @param user_data，创建时，用户传入的私有数据
* @remark ts计算说明，同ts_demuxed_video_h
*/
typedef int (*ts_demuxed_audio_h)(char* data,int nb_data,int ts,void* user_data);

/**
* 创建ts_demux_t
* @param ts_demuxed_video_h，原始视频回调接口
* @param ts_demuxed_audio_h，原始音频回调接口
* @param user_data，用户私有数据，通过回调返回
* @return srs_ts_demuxer_t，解封装器实例
*/
extern srs_ts_demuxer_t srs_ts_demuxer_create(ts_demuxed_video_h video_h,
	ts_demuxed_audio_h audio_h,
	void* user_data);

/**
* 解复用
* 成功返回0，失败返回非0值错误码
* @param ts_buf，一包TS packet数据指针
* @param nb_buf，数据大小
*/
extern int srs_ts_demuxer_demux(srs_ts_demuxer_t demux,char* ts_buf,int nb_buf);

/**
* 销毁ts_demux_t，释放所有源
*/
extern void srs_ts_demuxer_destroy(srs_ts_demuxer_t demux);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SRS_LIB_TS_DEMUXER */