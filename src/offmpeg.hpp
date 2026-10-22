#ifndef __OFFMPEG_HPP__
#define __OFFMPEG_HPP__
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

#include <iostream>
#include <cstdio>
typedef struct Offmpeg {
    AVFormatContext *ctx = nullptr;
    AVCodecContext *cctx = nullptr;
    const AVCodec *codec = nullptr;
    AVStream *video_stream = nullptr;
    AVCodecParameters *cpar = nullptr;
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;

    public:
        Offmpeg(char* video);
        int init(char* video);
        int _save_frame(AVFrame* frame,const char* name);
} Offmpeg;

#endif 
