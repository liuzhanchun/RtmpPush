#ifndef SRS_APP_TS_PARSER
#define SRS_APP_TS_PARSER

#include "srs_kernel_ts.hpp"

class SrsSimpleBuffer;
class SrsStream;
class SrsRawH264Stream;
class SrsRawAacStream;

class ISrsTsParsedHandler
{
public:
    ISrsTsParsedHandler();
    virtual ~ISrsTsParsedHandler();

public:
    virtual int on_parsed_video(char* data,int nb_data,int pts,int dts) = 0;
    virtual int on_parsed_audio(char* data,int nb_data,int ts) = 0;
};

class SrsTsParser : public ISrsTsHandler
{
public:
    SrsTsParser();
    virtual ~SrsTsParser();

public:
    int parse(char* buf,int nb_buf,ISrsTsParsedHandler* handler);

    // ISrsTsHandler Impl
    virtual int on_ts_message(SrsTsMessage* msg);

private:
    int do_parse(int nb_buf);
    int on_ts_video(SrsTsMessage* msg);
    int on_ts_audio(SrsTsMessage* msg);
private:
    SrsSimpleBuffer* _buffer;
    SrsStream* _stream;
    SrsTsContext* _context;
	SrsRawH264Stream* _avc;
	SrsRawAacStream* _aac;
	ISrsTsParsedHandler* _handler;
};

#endif //SRS_APP_TS_PARSER
