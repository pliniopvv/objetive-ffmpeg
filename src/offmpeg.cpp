extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

#include <iostream>
#include <cstdio>
#include "offmpeg.hpp"
#include <stdexcept>

Offmpeg::Offmpeg(char* video) {
    const char* caminho_video = video;
    //AVFormatContext* ctx = nullptr;

    double frame_selected = 10.0;

    if (avformat_open_input(&ctx, caminho_video, nullptr, nullptr) < 0) {
        throw std::invalid_argument("Erro ao abrir o vídeo.\n");
    }

    if (avformat_find_stream_info(ctx, nullptr) < 0) {
        std::cerr << "Erro ao buscar informações do stream\n";
        avformat_close_input(&ctx);
        throw std::invalid_argument("Erro ao buscar informações do stream\n");
    }

    int indice_video = -1;
    for (unsigned int i = 0; i < ctx->nb_streams; i++) {
        if (ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            indice_video = (int)i;
            break;
        }
    }

    if (indice_video == -1) {
        std::cerr << "Nenhum stream de vídeo encontrado.\n";
        avformat_close_input(&ctx);
        throw std::invalid_argument("Nenhum stream de vídeo encontrado.\n");
    }

    video_stream = ctx->streams[indice_video];
    cpar = video_stream->codecpar;
    codec = avcodec_find_decoder(cpar->codec_id);

    cctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(cctx, cpar);
    avcodec_open2(cctx, codec, nullptr);

    pkt = av_packet_alloc();
    frame = av_frame_alloc();

    while(av_read_frame(ctx, pkt) >= 0) {
        if (pkt->stream_index == indice_video) {
            if (avcodec_send_packet(cctx, pkt) == 0) {
                if (avcodec_receive_frame(cctx, frame) == 0) {

                    double frame_atual = 00.0;
                    if (frame->best_effort_timestamp != AV_NOPTS_VALUE) {
                        frame_atual = frame->best_effort_timestamp * av_q2d(video_stream->time_base);
                    }

                    if (frame_selected <= frame_atual) {
                        this->_save_frame(frame, "output_frame.ppm");
                        break;
                    }
                }
            }
        }
        av_packet_unref(pkt);
    }

    av_frame_free(&frame);
    av_packet_free(&pkt);
    avcodec_free_context(&cctx);
    avformat_close_input(&ctx);
}

int Offmpeg::_save_frame(AVFrame* frame, const char* name) {
    SwsContext* sws_ctx = sws_getContext(
            frame->width, frame->height, (AVPixelFormat)frame->format,
            frame->width, frame->height, AV_PIX_FMT_RGB24,
            SWS_BILINEAR, nullptr, nullptr, nullptr
            );
    AVFrame* frame_rgb = av_frame_alloc();
    frame_rgb->format = AV_PIX_FMT_RGB24;
    frame_rgb->width = frame->width;
    frame_rgb->height = frame->height;
    av_frame_get_buffer(frame_rgb, 32);

    sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height,
            frame_rgb->data, frame_rgb->linesize);

    FILE* arquivo = fopen(name, "wb");
    if (arquivo) {
        fprintf(arquivo, "P6\n%d %d\n255\n", frame_rgb->width, frame_rgb->height);

        for (int y = 0; y< frame_rgb->height; y++) {
            fwrite(frame_rgb->data[0] + y *frame_rgb->linesize[0], 1, frame_rgb->width*3, arquivo);
        }
        fclose(arquivo);
        std::cout << "Frame salvo com sucesso em 'output_frame.ppm'!\n";
    }

    sws_freeContext(sws_ctx);
    av_frame_free(&frame_rgb);

    return 0;
}
