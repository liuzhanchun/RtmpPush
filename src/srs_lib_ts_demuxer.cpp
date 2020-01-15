#include "srs_lib_ts_demuxer.hpp"
#include "srs_app_ts_parser.hpp"

struct DemuxContext : public ISrsTsParsedHandler {
	ts_demuxed_video_h video_h;
	ts_demuxed_audio_h audio_h;
	void* user_data;
	SrsTsParser *parser;

	DemuxContext() {
        video_h = NULL;
		audio_h = NULL;
		user_data = NULL;
		parser = NULL;
	}

	virtual ~DemuxContext() {
		if(parser) {
			delete parser;
			parser = NULL;
		}
		video_h = NULL;
		audio_h = NULL;
		user_data = NULL;
	}

	virtual int on_parsed_video(char* data,int nb_data,int pts,int dts) {
		if(video_h) {
			return video_h(data,nb_data,pts,dts,user_data);
		}

		return 0;
	}

	virtual int on_parsed_audio(char* data,int nb_data,int ts) {
		if(audio_h) {
			return audio_h(data,nb_data,ts,user_data);
		}

		return 0;
	}
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

srs_ts_demuxer_t srs_ts_demuxer_create(ts_demuxed_video_h video_h,
	ts_demuxed_audio_h audio_h,
	void* user_data)
{
	DemuxContext* ctx = new DemuxContext();
	ctx->video_h = video_h;
	ctx->audio_h = audio_h;
	ctx->user_data = user_data;
	ctx->parser = new SrsTsParser();

	return ctx;
}

int srs_ts_demuxer_demux(srs_ts_demuxer_t demux,char* ts_buf,int nb_buf)
{
	DemuxContext* ctx = (DemuxContext*)demux;
	if(!ctx || !ctx->parser) {
		return -1;
	}

	return ctx->parser->parse(ts_buf,nb_buf,ctx);
}

void srs_ts_demuxer_destroy(srs_ts_demuxer_t demux)
{
	DemuxContext* ctx = (DemuxContext*)demux;
	if(ctx) {
		delete ctx;
	}
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
