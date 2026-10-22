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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Faltou o argumento do vídeo!";
        return -1;
    }

    Offmpeg ffmpeg(argv[1]);
    return 0;
}
