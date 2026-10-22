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
    AVCodecParameters *cpar = nullptr;
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;

    public:
        Offmpeg(char* video);
        int init(char* video);
        int save_frame(char* name[]);
} Offmpeg;

#endif 
