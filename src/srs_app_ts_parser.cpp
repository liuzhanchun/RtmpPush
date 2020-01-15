
#include "srs_app_ts_parser.hpp"
#include "srs_kernel_buffer.hpp"

#include "srs_kernel_error.hpp"
#include "srs_kernel_log.hpp"
#include "srs_kernel_stream.hpp"

#include "srs_raw_avc.hpp"

//ISrsTsParsedHandler
ISrsTsParsedHandler::ISrsTsParsedHandler()
{

}

ISrsTsParsedHandler::~ISrsTsParsedHandler()
{

}

//SrsTsParser
SrsTsParser::SrsTsParser()
{
    _buffer = new SrsSimpleBuffer();
    _stream = new SrsStream();
    _context = new SrsTsContext();
	_avc = new SrsRawH264Stream();
	_aac = new SrsRawAacStream();
	_handler = NULL;
}

SrsTsParser::~SrsTsParser()
{
    srs_freep(_buffer);
    srs_freep(_stream);
    srs_freep(_context);
	srs_freep(_avc);
	srs_freep(_aac);
	_handler = NULL;
}

int SrsTsParser::parse(char* buf,int nb_buf,ISrsTsParsedHandler* handler)
{
    if(!buf || nb_buf <= 0 || !handler)
    {
        return -1;
    }

    _buffer->append(buf,nb_buf);
	_handler = handler;

    return do_parse(nb_buf);
}

int SrsTsParser::do_parse(int nb_buf)
{
    int ret = ERROR_SUCCESS;

    // find the sync byte of mpegts.
    char* p = _buffer->bytes();
    for (int i = 0; i < _buffer->length(); i++) {
        if (p[i] != 0x47) {
            continue;
        }

        if (i > 0) {
            _buffer->erase(i);
        }
        break;
    }

    // drop ts packet when size not modulus by 188
    if (_buffer->length() < SRS_TS_PACKET_SIZE) {
        srs_warn("wait packet %d/%d bytes",nb_buf, _buffer->length());
        return ret;
    }

    // use stream to parse ts packet.
    int nb_packet =  _buffer->length() / SRS_TS_PACKET_SIZE;
    for (int i = 0; i < nb_packet; i++) {
        char* p = _buffer->bytes() + (i * SRS_TS_PACKET_SIZE);
        if ((ret = _stream->initialize(p, SRS_TS_PACKET_SIZE)) != ERROR_SUCCESS) {
            return ret;
        }

        // process each ts packet
        if ((ret = _context->decode(_stream, this)) != ERROR_SUCCESS) {
            srs_warn("mpegts: ignore parse ts packet failed. ret=%d", ret);
            continue;
        }
        srs_info("mpegts: parse ts packet completed");
    }
    srs_info("mpegts: parse udp packet completed");

    // erase consumed bytes
    if (nb_packet > 0) {
        _buffer->erase(nb_packet * SRS_TS_PACKET_SIZE);
    }

    return ret;
}

int SrsTsParser::on_ts_message(SrsTsMessage* msg)
{
    int ret = ERROR_SUCCESS;

    // when not audio/video, or not adts/annexb format, donot support.
    if (msg->stream_number() != 0) {
        ret = ERROR_STREAM_CASTER_TS_ES;
        srs_error("mpegts: unsupported stream format, sid=%#x(%s-%d). ret=%d",
            msg->sid, msg->is_audio() ? "A" : msg->is_video() ? "V" : "N", msg->stream_number(), ret);
        return ret;
    }

    // check supported codec
    if (msg->channel->stream != SrsTsStreamVideoH264 && msg->channel->stream != SrsTsStreamAudioAAC) {
        ret = ERROR_STREAM_CASTER_TS_CODEC;
        srs_error("mpegts: unsupported stream codec=%d. ret=%d", msg->channel->stream, ret);
        return ret;
    }

    // publish audio or video.
    if (msg->channel->stream == SrsTsStreamVideoH264) {
        return on_ts_video(msg);
    }
    if (msg->channel->stream == SrsTsStreamAudioAAC) {
        return on_ts_audio(msg);
    }

    return ret;
}

int SrsTsParser::on_ts_video(SrsTsMessage* msg)
{
    int ret = ERROR_SUCCESS;

    // ts tbn to flv tbn.
    u_int32_t dts = (u_int32_t)(msg->dts / 90); 
    u_int32_t pts = (u_int32_t)(msg->dts / 90);
    
    if(_handler)
	{
		_handler->on_parsed_video(msg->payload->bytes(),msg->payload->length(),pts,dts);
    }

    return ret;
}

int SrsTsParser::on_ts_audio(SrsTsMessage* msg)
{
    int ret = ERROR_SUCCESS;

    // ts tbn to flv tbn.
    u_int32_t dts = (u_int32_t)(msg->dts / 90);

    if(_handler) {
		_handler->on_parsed_audio(msg->payload->bytes(),msg->payload->length(),dts);
	}

    return ret;
}
