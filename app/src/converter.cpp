#include "converter.h"

conv::PixFmt png_pixfmts[] = {
    {AV_PIX_FMT_RGBA,  (char*)"RGBA"},
    {AV_PIX_FMT_RGB24, (char*)"RGB24"},
    //{AV_PIX_FMT_PAL8, (char*)"PAL8"},
    {AV_PIX_FMT_RGBA64, (char*)"RGBA64"},
    {AV_PIX_FMT_YA8, (char*)"YA8"},
    {AV_PIX_FMT_GRAY8, (char*)"GRAY8"},
    {AV_PIX_FMT_MONOBLACK, (char*)"MONOBLACK"}
};

conv::PixFmt jpg_pixfmts[] = {
    {AV_PIX_FMT_YUVJ420P, (char*)"YUVJ420P"},
    {AV_PIX_FMT_YUV420P, (char*)"YUV420P"},
    {AV_PIX_FMT_YUV422P, (char*)"YUV422P"},
    {AV_PIX_FMT_YUV444P, (char*)"YUV444P"}
};

conv::PixFmt ico_png_pixfmts[] = {
    {AV_PIX_FMT_RGBA,  (char*)"RGBA"},
    //{AV_PIX_FMT_PAL8, (char*)"PAL8"}
};

conv::PixFmt ico_bmp_pixfmts[] = {
    {AV_PIX_FMT_BGRA,  (char*)"BGRA"},
    //{AV_PIX_FMT_PAL8, (char*)"PAL8"},
    {AV_PIX_FMT_MONOBLACK,  (char*)"MONOBLACK"}
};

conv::PixFmt bmp_pixfmts[] = {
    {AV_PIX_FMT_BGRA,  (char*)"BGRA"},
    {AV_PIX_FMT_BGR24,  (char*)"BGR24"},
    //{AV_PIX_FMT_PAL8, (char*)"PAL8"},
    {AV_PIX_FMT_GRAY8,  (char*)"GRAY8"},
    {AV_PIX_FMT_MONOBLACK,  (char*)"MONOBLACK"}
};

//
//
//

conv::VidCodec png_codecs[] = {
    {AV_CODEC_ID_PNG, (char*)"PNG", png_pixfmts, 6}
};

conv::VidCodec jpg_codecs[] = {
    {AV_CODEC_ID_MJPEG, (char*)"MJPEG", jpg_pixfmts, 4}
};

conv::VidCodec ico_codecs[] = {
    {AV_CODEC_ID_PNG, (char*)"PNG", ico_png_pixfmts, 1},
    {AV_CODEC_ID_BMP, (char*)"BMP", ico_bmp_pixfmts, 2}
};

conv::VidCodec bmp_codecs[] = {
    {AV_CODEC_ID_BMP, (char*)"BMP", bmp_pixfmts, 4}
};

conv::ImgData img_data[] = {
    {png_codecs, 1, 0},
    {jpg_codecs, 1, 0},
    {jpg_codecs, 1, 0},
    {ico_codecs, 2, 255},
    {bmp_codecs, 1, 0}
};

//
//
//

conv::SmplFmt mp3_smplfmts[] = {
    {AV_SAMPLE_FMT_S16, (char*)"S16"}
};

conv::SmplFmt opus_smplfmts[] = {
    {AV_SAMPLE_FMT_FLTP, (char*)"FLTP"},
    {AV_SAMPLE_FMT_S16, (char*)"S16"}
};

conv::SmplFmt wav_smplfmts[] = {
    {AV_SAMPLE_FMT_S16, (char*)"S16"},
    {AV_SAMPLE_FMT_FLT, (char*)"FLT"}
};

conv::SmplFmt aac_smplfmts[] = {
    {AV_SAMPLE_FMT_FLTP, (char*)"FLTP"},
    {AV_SAMPLE_FMT_S16, (char*)"S16"}
};

conv::SmplFmt flac_smplfmts[] = {
    {AV_SAMPLE_FMT_S16, (char*)"S16"},
    {AV_SAMPLE_FMT_S32, (char*)"S32"}
};

conv::SmplFmt vorbis_smplfmts[] = {
    {AV_SAMPLE_FMT_FLTP, (char*)"FLTP"}
};

//
//
//

conv::SmplRate mp3_smplrates[] = {
    {(char*)"8000", 8000}, {(char*)"11025", 11025},
    {(char*)"12000", 12000}, {(char*)"16000", 16000},
    {(char*)"22050", 22050}, {(char*)"24000", 24000},
    {(char*)"32000", 32000}, {(char*)"44100", 44100},
    {(char*)"48000", 48000}
};

conv::SmplRate opus_smplrates[] = {
    {(char*)"8000", 8000}, {(char*)"12000", 12000},
    {(char*)"16000", 16000}, {(char*)"24000", 24000},
    {(char*)"48000", 48000}
};

conv::SmplRate wav_smplrates[] = {
    {(char*)"8000", 8000}, {(char*)"11025", 11025},
    {(char*)"12000", 12000}, {(char*)"16000", 16000},
    {(char*)"22050", 22050}, {(char*)"24000", 24000},
    {(char*)"32000", 32000}, {(char*)"44100", 44100},
    {(char*)"48000", 48000}, {(char*)"64000", 64000},
    {(char*)"88200", 88200}, {(char*)"96000", 96000},
    {(char*)"176400", 176400}, {(char*)"192000", 192000}
};

conv::SmplRate aac_smplrates[] = {
    {(char*)"8000", 8000}, {(char*)"11025", 11025},
    {(char*)"12000", 12000}, {(char*)"16000", 16000},
    {(char*)"22050", 22050}, {(char*)"24000", 24000},
    {(char*)" 32000", 32000}, {(char*)" 44100", 44100},
    {(char*)"48000", 48000}, {(char*)"64000", 64000},
    {(char*)"88200", 88200}, {(char*)"96000", 96000}
};

conv::SmplRate flac_smplrates[] = {
    {(char*)"8000", 8000}, {(char*)"11025", 11025},
    {(char*)"12000", 12000}, {(char*)"16000", 16000},
    {(char*)"22050", 22050}, {(char*)"24000", 24000},
    {(char*)"32000", 32000}, {(char*)"44100", 44100},
    {(char*)"48000", 48000}, {(char*)"64000", 64000},
    {(char*)"88200", 88200}, {(char*)"96000", 96000},
    {(char*)"176400", 176400}, {(char*)"192000", 192000}
};

conv::SmplRate vorbis_smplrates[] = {
    {(char*)"8000", 8000}, {(char*)"12000", 12000},
    {(char*)"16000", 16000}, {(char*)"24000", 24000},
    {(char*)"32000", 32000}, {(char*)"44100", 44100},
    {(char*)"48000", 48000}, {(char*)"64000", 64000},
    {(char*)"88200", 88200}, {(char*)"96000", 96000},
    {(char*)"176400", 176400}, {(char*)"192000", 192000}
};

//
//
//

conv::BitRate mp3_bitrates[] = {
    {(char*)"8000", 8000}, {(char*)"16000", 16000},
    {(char*)"32000", 32000}, {(char*)"48000", 48000},
    {(char*)"64000", 64000}, {(char*)"96000", 96000},
    {(char*)"128000", 128000}, {(char*)"160000", 160000},
    {(char*)"256000", 256000}, {(char*)"320000", 320000}
};

conv::BitRate opus_bitrates[] = {
    {(char*)"6000", 6000}, {(char*)"8000", 8000},
    {(char*)"16000", 16000}, {(char*)"32000", 32000},
    {(char*)"64000", 64000}, {(char*)"96000", 96000},
    {(char*)"128000", 128000}, {(char*)"160000", 160000},
    {(char*)"256000", 256000}, {(char*)"320000", 320000}
};

conv::BitRate wav_bitrates[] = {
    {(char*)"auto", 0}
};

conv::BitRate aac_bitrates[] = {
    {(char*)"8000", 8000}, {(char*)"16000", 16000},
    {(char*)"32000", 32000}, {(char*)"64000", 64000},
    {(char*)"96000", 96000}, {(char*)"128000", 128000},
    {(char*)"160000", 160000}, {(char*)"192000", 192000},
    {(char*)"256000", 256000}
};

conv::BitRate flac_bitrates[] = {
    {(char*)"auto", 0}
};

conv::BitRate vorbis_bitrates[] = {
    {(char*)"16000", 16000}, {(char*)"32000", 32000},
    {(char*)"64000", 64000}, {(char*)"96000", 96000},
    {(char*)"128000", 128000}, {(char*)"160000", 160000},
    {(char*)"256000", 256000}, {(char*)"320000", 320000}
};

//
//
//

conv::NbChannels def_nb_channels[] = {
    {(char*)"Mono", 1},
    {(char*)"Stereo", 2}
};

//
//
//

conv::AudCodec mp3_codecs[] = {
    {AV_CODEC_ID_MP3, (char*)"MP3", mp3_smplfmts, mp3_smplrates,
     mp3_bitrates, def_nb_channels,  1, 9, 10, 2, 1152, 8, 6}
};

conv::AudCodec opus_codecs[] = {
    {AV_CODEC_ID_OPUS, (char*)"OPUS", opus_smplfmts, opus_smplrates,
     opus_bitrates, def_nb_channels, 2, 5, 10, 2, 960, 4, 6}
};

conv::AudCodec wav_codecs[] = {
    {AV_CODEC_ID_PCM_S16LE, (char*)"PCM_S16LE",
     wav_smplfmts, wav_smplrates,
     wav_bitrates, def_nb_channels, 2, 14, 1, 2, 1024, 8, 0}
};

conv::AudCodec aac_codecs[] = {
    {AV_CODEC_ID_AAC, (char*)"AAC",
     aac_smplfmts, aac_smplrates,
     aac_bitrates, def_nb_channels, 2, 12, 9, 2, 1024, 8, 5}
};

conv::AudCodec flac_codecs[] = {
    {AV_CODEC_ID_FLAC, (char*)"FLAC",
     flac_smplfmts, flac_smplrates,
     flac_bitrates, def_nb_channels, 2, 14, 1, 2, 0, 8, 0}
};

conv::AudCodec ogg_codecs[] = {
    {AV_CODEC_ID_VORBIS, (char*)"VORBIS",
     vorbis_smplfmts, vorbis_smplrates,
     vorbis_bitrates, def_nb_channels, 1, 12, 8, 2, 1024, 6, 4},
    {opus_codecs[0]}
};

conv::AudData aud_data[] = {
    {mp3_codecs, 1},
    {opus_codecs, 1},
    {wav_codecs, 1},
    {aac_codecs, 1},
    {flac_codecs, 1},
    {ogg_codecs, 2}
};

//
//
//

conv::PixFmt mp4_mov_pixfmts[] = {
    {AV_PIX_FMT_YUV420P, (char*)"YUV420P"}
};

conv::PixFmt webm_vp9_pixfmts[] = {
    {AV_PIX_FMT_YUV420P, (char*)"YUV420P"},
    {AV_PIX_FMT_YUV420P10LE, (char*)"YUV420P10LE"},
    {AV_PIX_FMT_YUV422P, (char*)"YUV422"},
    {AV_PIX_FMT_YUV422P10LE, (char*)"YUV422P10LE"},
    {AV_PIX_FMT_YUV444P, (char*)"YUV444P"},
    {AV_PIX_FMT_YUV444P10LE, (char*)"YUV444P10LE"}
};

conv::PixFmt webm_vp8_pixfmts[] = {
    {AV_PIX_FMT_YUV420P, (char*)"YUV420P"},
    {AV_PIX_FMT_YUV422P, (char*)"YUV422P"},
    {AV_PIX_FMT_YUV444P, (char*)"YUV444P"}
};

conv::PixFmt webm_av1_pixfmts[] = {
    {AV_PIX_FMT_YUV420P, (char*)"YUV420P"},
    {AV_PIX_FMT_YUV420P10LE, (char*)"YUV420P10LE"},
    {AV_PIX_FMT_YUV422P, (char*)"YUV422P"},
    {AV_PIX_FMT_YUV422P10LE, (char*)"YUV422P10LE"},
    {AV_PIX_FMT_YUV444P, (char*)"YUV444P"},
    {AV_PIX_FMT_YUV444P10LE, (char*)"YUV444P10LE"},
    {AV_PIX_FMT_YUV420P12LE, (char*)"YUV420P12LE"},
    {AV_PIX_FMT_YUV444P12LE, (char*)"YUV444P12LE"},
    {AV_PIX_FMT_RGB8, (char*)"YUV444P12LE"},
    {AV_PIX_FMT_GBRP, (char*)"GBRP"},
    {AV_PIX_FMT_GBRP10LE, (char*)"GBRP10LE"},
    {AV_PIX_FMT_GBRP12LE, (char*)"GBRP12LE"},
    {AV_PIX_FMT_GBRP16LE, (char*)"GBRP16LE"}
};

//
//
//

conv::VidCodec mp4_mov_mkv_vcodecs[] = {
    {AV_CODEC_ID_H264, (char*)"H264", mp4_mov_pixfmts, 1},
    {AV_CODEC_ID_HEVC, (char*)"H265/HEVC", mp4_mov_pixfmts, 1},
    {AV_CODEC_ID_MPEG4, (char*)"MPEG4", mp4_mov_pixfmts, 1}
};

conv::VidCodec webm_vcodecs[] = {
    {AV_CODEC_ID_VP9, (char*)"VP9", webm_vp9_pixfmts, 6},
    {AV_CODEC_ID_VP8, (char*)"VP8", webm_vp8_pixfmts, 3},
    {AV_CODEC_ID_AV1, (char*)"AV1", webm_av1_pixfmts, 13}
};

//
//
//

conv::AudCodec mp4_mov_acodecs[] = {
    mp3_codecs[0], aac_codecs[0]
};

conv::AudCodec mkv_acodecs[] = {
    mp3_codecs[0], aac_codecs[0], opus_codecs[0], flac_codecs[0]
};

conv::AudCodec webm_acodecs[] = {
    opus_codecs[0]
};

conv::VidData vid_data[] = {
    {mp4_mov_mkv_vcodecs, mp4_mov_acodecs, 3, 2},
    {mp4_mov_mkv_vcodecs, mp4_mov_acodecs, 3, 2},
    {mp4_mov_mkv_vcodecs, mkv_acodecs, 3, 4},
    {webm_vcodecs, webm_acodecs, 3, 1}
};

int conv::getFmtData(const char *fn, FmtData *fdata)
{
    for (int i = 0;;++i) {
        if (i > IMG_EXTS_LEN && i > VID_EXTS_LEN && i > AUD_EXTS_LEN) {
            fdata->type = MediaType::UNKNOWN;
            fdata->exts_len = 0;
            fdata->ext_ind = -1;
            return Errors::UNKNOWN_FMT;
        }

        if (i < IMG_EXTS_LEN && strstr(fn, IMG_EXTS[i])) {
            fdata->type = MediaType::IMG;
            fdata->exts = IMG_EXTS;
            fdata->exts_len = IMG_EXTS_LEN;
            fdata->ext_ind = i;
            return 0;
        }
        else if (i < VID_EXTS_LEN && strstr(fn, VID_EXTS[i])) {
            fdata->type = MediaType::VID;
            fdata->exts = VID_EXTS;
            fdata->exts_len = VID_EXTS_LEN;
            fdata->ext_ind = i;
            return 0;
        }
        else if (i < AUD_EXTS_LEN && strstr(fn, AUD_EXTS[i])) {
            fdata->type = MediaType::AUD;
            fdata->exts = AUD_EXTS;
            fdata->exts_len = AUD_EXTS_LEN;
            fdata->ext_ind = i;
            return 0;
        }
    }
}

int conv::getFileData(const char *fn, FileData *fdata)
{
    (void)fn;

    if (!fn || !fdata) {
        log_err("[!] [getFileData] Failed. Argument is NULL.\n");
        return Errors::NULL_ARGUMENT_ERR;
    }

    AVFormatContext   *ifmt_ctx = nullptr;
    AVDictionaryEntry *tag = nullptr;
    AVPacket          *pkt = nullptr;
    AVFrame           *frame = nullptr;
    int                ret = 0;

    ret = avformat_open_input(&ifmt_ctx, fn, nullptr, nullptr);
    if (ret < 0) {
        log_err("[!] [getFileData] Failed to avformat_open_input.\n");
        return Errors::LIBAV_ERR;
    }

    ret = avformat_find_stream_info(ifmt_ctx, nullptr);
    if (ret < 0) {
        log_err("[!] [getFileData] Failed to "
                "avformat_find_stream_info.\n");
        ret = Errors::LIBAV_ERR;
        goto cleanup;
    }

    fdata->duration = (float)ifmt_ctx->duration / 1000;
    fdata->nb_streams = ifmt_ctx->nb_streams;
    fdata->bit_rate = ifmt_ctx->bit_rate;
    fdata->infmt = ifmt_ctx->iformat->long_name;

    while ((tag = av_dict_get(ifmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        fdata->metadata.insert(std::make_pair(tag->key, tag->value));
    }

    pkt = av_packet_alloc();
    frame = av_frame_alloc();

    for (unsigned int i = 0; i < ifmt_ctx->nb_streams; ++i) {
        AVCodecParameters *dec_par = nullptr;
        const AVCodec *dec = nullptr;
        AVCodecContext *dec_ctx = nullptr;
        AVStream *stream = ifmt_ctx->streams[i];

        dec_par = stream->codecpar;
        dec = avcodec_find_decoder(dec_par->codec_id);

        dec_ctx = avcodec_alloc_context3(dec);
        avcodec_parameters_to_context(dec_ctx, dec_par);
        avcodec_open2(dec_ctx, dec, nullptr);

        av_read_frame(ifmt_ctx, pkt);

        avcodec_send_packet(dec_ctx, pkt);
        avcodec_receive_frame(dec_ctx, frame);

        while ((tag = av_dict_get(frame->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            fdata->metadata.insert(std::make_pair(tag->key, tag->value));// [tag->key] = tag->value;
        }

        av_packet_unref(pkt);
        av_frame_unref(frame);

        while ((tag = av_dict_get(ifmt_ctx->streams[i]->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            fdata->metadata.insert(std::make_pair(tag->key, tag->value));// [tag->key] = tag->value;
        }
    }

    av_packet_free(&pkt);
    av_frame_free(&frame);

cleanup:
    avformat_close_input(&ifmt_ctx);

    return ret;
}


conv::ImgData* conv::getImgParams(int index)
{
    return &img_data[index];
}

conv::AudData* conv::getAudParams(int index)
{
    return &aud_data[index];
}

conv::VidData* conv::getVidParams(int index)
{
    return &vid_data[index];
}



































static int encode_video(AVFrame         *frame,
                        AVFormatContext *ofmt_ctx,
                        AVFormatContext *ifmt_ctx,
                        AVCodecContext  *enc_ctx,
                        AVPacket        *out_pkt,
                        int              video_index)
{
    static     size_t  pts = 0;
    int        res         = 0;
    AVStream  *enc_stream  = ofmt_ctx->streams[video_index];

    if (frame) {
        puts("* encode_video");
        fflush(stdout);
        fflush(stderr);
    }
    else {
        puts("! encode_video. frame is nullptr.");
        fflush(stdout);
        fflush(stderr);
    }

    if (frame) {
        frame->pts = pts;
        pts        += frame->nb_samples;

        if (frame->format != enc_stream->codecpar->format) {
            printf("\n********\n"
                   "frame has diffrent fortmat than stream!(f: %d, s: %d)"
                   "\n********\n",
                   frame->format, enc_stream->codecpar->format);
        }
    }
    else {
        pts = 0;
    }

    res = avcodec_send_frame(enc_ctx, frame);
    if (res < 0 && res != AVERROR_EOF) {
        fprintf(stderr, "Failed to send_frame."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }

    while (avcodec_receive_packet(enc_ctx, out_pkt) >= 0) {
        out_pkt->stream_index = video_index;

        av_packet_rescale_ts(out_pkt,
                             enc_ctx->time_base,
                             enc_stream->time_base);

        res = av_interleaved_write_frame(ofmt_ctx, out_pkt);
        if (res < 0) {
            fprintf(stderr, "Failed to write_frame."
                            " On line: %d.\n", __LINE__);
            goto cleanup;
        }
    }

    if (res == AVERROR(EAGAIN)) {
        res = -1;
    }
    else if (res == AVERROR_EOF) {
        res = -1;
    }
    else if (res < 0) {
        fprintf(stderr, "Failed to receive_packet."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }
    else {
        res = 0;
    }

cleanup:
    av_packet_unref(out_pkt);
    return res;
}

static int encode_audio_frame(AVFrame         *frame,
                              AVFormatContext *ofmt_ctx,
                              AVFormatContext *ifmt_ctx,
                              AVCodecContext  *enc_ctx,
                              AVPacket        *out_pkt,
                              int             *data_presended,
                              int              audio_index)
{
    static int     counter         = 0;
    static size_t  samples_counter = 0;
    static int     flush_counter   = 0;
    static size_t  pts      = 0;
    int            res      = 0;

    if (frame) {
        frame->pts = pts;
        pts        += frame->nb_samples;
    }
    else {
        pts = 0;

        counter         = 0;
        samples_counter = 0;
        flush_counter   = 0;
    }

    *data_presended = 0;
    res = avcodec_send_frame(enc_ctx, frame);
    if ((res < 0) && (res != AVERROR_EOF)) {
        char buff[64] = {'\0'};
        av_strerror(res, buff, sizeof(buff));
        fprintf(stderr, "Failed to send_frame. On line: %d. Err: %s\n", __LINE__, buff);
        goto cleanup;
    }

    if (!frame) {
        printf("\tgoing to recive audio packets (res: %d).\n", res);
    }

    while (avcodec_receive_packet(enc_ctx, out_pkt) >= 0) {
        if (!frame) {
            printf("\t|(res: %d)receiving packet...", res);
        }

        av_packet_rescale_ts(out_pkt,
                             enc_ctx->time_base,
                             ofmt_ctx->streams[audio_index]->time_base);

        out_pkt->stream_index = audio_index;

        res = av_interleaved_write_frame(ofmt_ctx, out_pkt);
        if (res < 0) {
            fprintf(stderr, "Failed to write_frame."
                            " On line: %d.\n", __LINE__);
            goto cleanup;
        }

        if (!frame) {
            printf("received packet(res: %d)|\n", res);
        }
    }

    if (res == AVERROR(EAGAIN)) {
        res = 0;
    }
    else if (res == AVERROR_EOF) {
        res = 0;
    }
    else if (res < 0) {
        fprintf(stderr, "Failed to receive_packet."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }
    else {
        *data_presended = 1;
    }

    if (frame) {
        samples_counter += frame->nb_samples;
        ++counter;
    }
    else {
        ++flush_counter;

        printf("* total_nb_samples: %zu, frames: %d, flushed_frames: %d.\n", samples_counter, counter, flush_counter);
    }
    /*
        if (frame && frame->pts) {
            printf("\t$ %6d, %zu, %f.\n", counter, frame->pts, frame->pts * av_q2d(ofmt_ctx->streams[audio_index]->time_base));
        }
*/
cleanup:
    av_packet_unref(out_pkt);
    return res;
}



int conv::convert(const char *inpath, const char *outpath,
            const ConvData *data)
{
    AVFormatContext    *ifmt_ctx    = nullptr;
    AVFormatContext    *ofmt_ctx    = nullptr;
    AVCodecContext     *dec_ctx[3]  = {nullptr, nullptr, nullptr};// 0 - video, 1 - audio, 2 - NULL
    AVCodecContext     *enc_ctx[3]  = {nullptr, nullptr, nullptr};// 0 - video, 1 - audio, 2 - NULL
    AVPacket           *in_pkt      = nullptr;
    AVPacket           *out_pkt     = nullptr;
    AVFrame            *in_frame    = nullptr;
    struct SwsContext  *sws_ctx     = nullptr;
    struct SwrContext  *swr_ctx     = nullptr;
    AVAudioFifo        *fifo        = nullptr;
    AVFrame            *out_frame   = nullptr;
    int                 res         = 0;
    int                 video_index = 0;
    int                 audio_index = 1;
    char                cpy_video   = 0;
    char                cpy_audio   = 0;
    char                reading     = 1;
    int                 aframe_size = 0;

    size_t samples_counter = 0;
    int    frames_counter  = 0;

    size_t video_type_counter = 0;
    size_t audio_type_counter = 0;

    printf("inpath: %s; outpath: %s.\n", inpath, outpath);

    printf("data_fields:\n"
           "\tvideo_codec: %d;\n"
           "\tvideo_codec_name: %s;\n"
           "\taudio_codec: %d;\n"
           "\taudio_codec_name: %s;\n"
           "\tpixfmt: %d;\n"
           "\tsmplfmt: %d;\n"
           "\tsample_rate: %d;\n"
           "\tbit_rate: %" PRId64 ".\n",
           data->vcodec,
           avcodec_get_name(data->vcodec),
           data->acodec,
           avcodec_get_name(data->acodec),
           data->pixfmt,
           data->smplfmt,
           data->smplrate,
           data->bitrate
           );

    /*
     *  IFMT_CTX
     */


    res = avformat_open_input(&ifmt_ctx, inpath, NULL, NULL);
    if (res < 0) {
        fprintf(stderr, "Failed to open_input. On line: %d.\n",
                __LINE__);
        return res;
    }

    res = avformat_find_stream_info(ifmt_ctx, NULL);
    if (res < 0) {
        fprintf(stderr, "Failed to find_stream_info. On line: %d.\n",
                __LINE__);
        goto cleanup;
    }

    /*
     *  OFMT_CTX
     */
    res = avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, outpath);
    if (res < 0) {
        fprintf(stderr, "Failed to alloc_output_context2."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }
    else {
        char dur_buff[32];

        snprintf(dur_buff, sizeof(dur_buff), "%.3f", (double)ifmt_ctx->duration / AV_TIME_BASE);
        av_dict_set(&ofmt_ctx->metadata, "duration", dur_buff, 0);
    }

    /*
     *  SETTING UP DEC AND ENC CODEC_CONTEXTs
     */
    for (unsigned int i = 0; i < ifmt_ctx->nb_streams; ++i) {
        AVCodecParameters *dec_par = nullptr;

        dec_par = ifmt_ctx->streams[i]->codecpar;

        if (dec_par->codec_type == AVMEDIA_TYPE_VIDEO && data->vcodec != AV_CODEC_ID_NONE) {
            video_index = i;

            if (!cpy_video) {
                const AVCodec  *dec        = nullptr;
                const AVCodec  *enc        = nullptr;
                AVStream       *enc_stream = nullptr;

                /*
                 *  DEC_CTX
                 */
                dec = avcodec_find_decoder(dec_par->codec_id);
                if (!dec) {
                    fprintf(stderr, "Failed to find_decoder."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                dec_ctx[0] = avcodec_alloc_context3(dec);
                if (!dec_ctx[0]) {
                    fprintf(stderr, "Failed to alloc_context3."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                res = avcodec_parameters_to_context(dec_ctx[0], dec_par);
                if (res < 0) {
                    fprintf(stderr, "Failed to copy "
                                    "parameters_to_context. On line: %d.\n",
                            __LINE__);
                    goto cleanup;
                }

                res = avcodec_open2(dec_ctx[0], dec, NULL);
                if (res < 0) {
                    fprintf(stderr, "Failed to open2 dec_ctx[0]."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                dec_ctx[0]->pkt_timebase = ifmt_ctx->streams[i]->time_base;

                /*
                 *  ENC_CTX
                 */
                enc_stream = avformat_new_stream(ofmt_ctx, NULL);
                if (!enc_stream) {
                    fprintf(stderr, "Failed to new_stream."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                enc = avcodec_find_encoder(data->vcodec);
                if (!enc) {
                    fprintf(stderr, "Failed to find_encoder."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                enc_ctx[0] = avcodec_alloc_context3(enc);
                if (!enc_ctx[0]) {
                    fprintf(stderr, "Failed to alloc_context3."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                /* clmaping max image's width, height */
                if (data->max_width
                    && ((dec_ctx[0]->width > data->max_width) || (dec_ctx[0]->height > data->max_width)))
                {
                    if (dec_ctx[0]->width > dec_ctx[0]->height) {
                        const float scale = (float)dec_ctx[0]->width / (float)dec_ctx[0]->height;

                        enc_ctx[0]->width = data->max_width;
                        enc_ctx[0]->height = data->max_width / scale;
                    }
                    else {
                        const float scale = dec_ctx[0]->height / dec_ctx[0]->width;

                        enc_ctx[0]->width = data->max_width / scale;
                        enc_ctx[0]->height = data->max_width;
                    }
                }
                else {
                    enc_ctx[0]->width = dec_ctx[0]->width;
                    enc_ctx[0]->height = dec_ctx[0]->height;
                }

                enc_ctx[0]->bit_rate = data->bitrate ? data->bitrate : 0;
                enc_ctx[0]->sample_aspect_ratio =
                    dec_ctx[0]->sample_aspect_ratio;
                enc_ctx[0]->pix_fmt = data->pixfmt;
                enc_ctx[0]->time_base = {1, 25};

                if (dec_ctx[0]->framerate.num <= 0) {
                    printf("dec_ctx[0]->framerate.num <= 0.\n");
                }

                printf("dec_ctx[0]->extradata_size: %d.\n", dec_ctx[0]->extradata_size);
                fflush(stdout);

                res = avcodec_open2(enc_ctx[0], enc, NULL);
                if (res < 0) {
                    char err_buff[32] = {0};
                    av_strerror(res, err_buff, sizeof(err_buff));
                    fprintf(stderr, "Failed to open2 enc_ctx."
                                    " On line: %d. Err: %s.\n",
                            __LINE__, err_buff);
                    goto cleanup;
                }

                avcodec_parameters_from_context(enc_stream->codecpar,
                                                enc_ctx[0]);

                if (dec_ctx[0]->extradata_size > 0 && data->acodec != AV_CODEC_ID_NONE) {
                    enc_stream->codecpar->extradata = (uint8_t*)malloc(dec_ctx[0]->extradata_size);
                    if (!enc_stream->codecpar->extradata) {
                        fprintf(stderr, "Failed to malloc enc_stream->codecpar->extradata. "
                                        "Line: %d.\n", __LINE__ - 3);
                        goto cleanup;
                    }

                    memcpy(enc_stream->codecpar->extradata, dec_ctx[0]->extradata, dec_ctx[0]->extradata_size);
                    enc_stream->codecpar->extradata_size = dec_ctx[0]->extradata_size;
                }

                enc_stream->codecpar->format = enc_ctx[0]->pix_fmt;
                enc_stream->codecpar->codec_tag = 0;
                enc_stream->time_base = enc_ctx[0]->time_base;
                enc_stream->avg_frame_rate = ifmt_ctx->streams[video_index]->avg_frame_rate;

                /*
                 *  SWS_CTX
                 */
                sws_ctx = sws_getContext(
                    dec_ctx[0]->width, dec_ctx[0]->height,
                    dec_ctx[0]->pix_fmt, enc_ctx[0]->width,
                    enc_ctx[0]->height, enc_ctx[0]->pix_fmt,
                    SWS_BICUBIC, NULL, NULL, NULL
                    );
                if (!sws_ctx) {
                    fprintf(stderr, "Failed to sws_getContext."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }
            }
            else { /* COPY_VIDEO_STREAM */
                AVStream *video_stream = NULL;

                video_stream = avformat_new_stream(ofmt_ctx, NULL);
                if (!video_stream) {
                    fprintf(stderr, "Failed to alloc new_stream."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                avcodec_parameters_copy(video_stream->codecpar,
                                        ifmt_ctx->streams[i]->codecpar);
            }
        }
        else if (dec_par->codec_type == AVMEDIA_TYPE_AUDIO && data->acodec != AV_CODEC_ID_NONE) {
            audio_index = i;

            if (!cpy_audio) {
                const AVCodec   *dec        = nullptr;
                const AVCodec   *enc        = nullptr;
                AVStream        *enc_stream = nullptr;
                AVDictionary    *opts       = nullptr;

                /*
                 *  DEC_CTX
                 */

                dec = avcodec_find_decoder(dec_par->codec_id);
                if (!dec) {
                    fprintf(stderr, "Failed to find_decoder."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                dec_ctx[1] = avcodec_alloc_context3(dec);
                if (!dec_ctx[1]) {
                    fprintf(stderr, "Failed to alloc_context3."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                res = avcodec_parameters_to_context(dec_ctx[1], dec_par);
                if (res < 0) {
                    fprintf(stderr, "Failed to copy "
                                    "parameters_to_context. On line: %d.\n",
                            __LINE__);
                    goto cleanup;
                }

                res = avcodec_open2(dec_ctx[1], dec, NULL);
                if (res < 0) {
                    fprintf(stderr, "Failed to open2 dec_ctx[1]."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                dec_ctx[1]->pkt_timebase = ifmt_ctx->streams[i]->time_base;

                /*
                 *  ENC_CTX
                 */

                enc = avcodec_find_encoder(data->acodec);
                if (!enc) {
                    fprintf(stderr, "Failed to find_encoder."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                enc_ctx[1] = avcodec_alloc_context3(enc);
                if (!enc_ctx[1]) {
                    fprintf(stderr, "Failed to alloc_context3."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                enc_ctx[1]->sample_rate         = data->smplrate;
                enc_ctx[1]->sample_fmt          = data->smplfmt;
                enc_ctx[1]->bit_rate            = data->bitrate;
                enc_ctx[1]->frame_size          = data->frame_size;
                av_channel_layout_default(&enc_ctx[1]->ch_layout, data->nb_channels);

                printf("enc_ctx[1]->bit_rate: %" PRId64 " / %" PRId64 " (before opening).\n", enc_ctx[1]->bit_rate, data->bitrate);

                av_dict_set(&opts, "strict", "experimental", 0);

                res = avcodec_open2(enc_ctx[1], enc, &opts);
                if (res < 0) {
                    fprintf(stderr, "Failed to open2 enc_ctx[1]."
                                    " On line: %d.\n", __LINE__);
                    av_dict_free(&opts);
                    goto cleanup;
                }

                printf("enc_ctx[1]->bit_rate: %" PRId64 " / %" PRId64 " (after opening).\n", enc_ctx[1]->bit_rate, data->bitrate);

                /* 0 or 1 - wav.
                 * These codecs expects variable frame_sizes.
                 * Then data->frame_size value is taken */

                aframe_size = enc_ctx[1]->frame_size > 1 ? enc_ctx[1]->frame_size : data->frame_size;

                av_dict_free(&opts);

                enc_stream = avformat_new_stream(ofmt_ctx, NULL);
                if (!enc_stream) {
                    fprintf(stderr, "Failed to new_stream."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                enc_stream->time_base = {1, enc_ctx[1]->sample_rate};

                res = avcodec_parameters_from_context(
                    enc_stream->codecpar, enc_ctx[1]);
                if (res < 0) {
                    fprintf(stderr, "Failed to copy "
                                    "parameters_from_context."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                /*
                 *  SWR_CTX
                 */
                res = swr_alloc_set_opts2(&swr_ctx,
                                          &enc_ctx[1]->ch_layout,
                                          enc_ctx[1]->sample_fmt,
                                          enc_ctx[1]->sample_rate,
                                          &dec_ctx[1]->ch_layout,
                                          dec_ctx[1]->sample_fmt,
                                          dec_ctx[1]->sample_rate,
                                          0, NULL);
                if (res < 0) {
                    fprintf(stderr, "Failed to alloc_set_opts2."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                res = swr_init(swr_ctx);
                if (res < 0) {
                    fprintf(stderr, "Failed to init swr."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                /*
                 *  FIFO
                 */

                fifo = av_audio_fifo_alloc(enc_ctx[1]->sample_fmt,
                                           enc_ctx[1]->ch_layout.nb_channels, 1);
                if (!fifo) {
                    fprintf(stderr, "Failed to fifo_alloc."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }
            }
            else { /* COPY_AUDIO_STREAM */
                AVStream *audio_stream = NULL;

                audio_stream = avformat_new_stream(ofmt_ctx, NULL);
                if (!audio_stream) {
                    fprintf(stderr, "Failed to alloc new_stream."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                avcodec_parameters_copy(audio_stream->codecpar,
                                        ifmt_ctx->streams[i]->codecpar);
            }
        }
    }

    if (data->vcodec && data->acodec) {
        //
        //
        printf("^^^in_media^^^\n"
               "\tvideo_codec: %s;\n"
               "\taudio_codec: %s;\n"
               "\tvideo_bit_rate: %" PRId64 ";\n"
               "\taudio_bit_rate: %" PRId64 ";\n"
               "\tsample_rate: %d;\n"
               "\tpix_fmt: %d;\n"
               "\tsample_fmt: %d;\n",
               avcodec_get_name(dec_ctx[0]->codec_id),
               avcodec_get_name(dec_ctx[1]->codec_id),
               dec_ctx[0]->bit_rate,
               dec_ctx[1]->bit_rate,
               dec_ctx[1]->sample_rate,
               dec_ctx[0]->pix_fmt,
               dec_ctx[1]->sample_fmt
               );
        //
        printf("^^^out_media^^^\n"
               "\tvideo_codec: %s;\n"
               "\taudio_codec: %s;\n"
               "\tvideo_bit_rate: %" PRId64 ";\n"
               "\taudio_bit_rate: %" PRId64 ";\n"
               "\tsample_rate: %d;\n"
               "\tpix_fmt: %d;\n"
               "\tsample_fmt: %d;\n",
               avcodec_get_name(enc_ctx[0]->codec_id),
               avcodec_get_name(enc_ctx[1]->codec_id),
               enc_ctx[0]->bit_rate,
               enc_ctx[1]->bit_rate,
               enc_ctx[1]->sample_rate,
               enc_ctx[0]->pix_fmt,
               enc_ctx[1]->sample_fmt
               );
        //
        //
    }

    /*
     *  OFMT_CTX STUFF
     */

    if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
        ofmt_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }


    res = avio_open(&ofmt_ctx->pb, outpath, AVIO_FLAG_WRITE);
    if (res < 0) {
        fprintf(stderr, "Failed to avio_open output file."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }

    puts("> header");
    res = avformat_write_header(ofmt_ctx, NULL);
    if (res < 0) {
        fprintf(stderr, "Failed to write_header to output file."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }
    puts("< header");

    /*
     *  ALLOCATING PKTs AND FRAMEs (IN, OUT)
     */

    in_pkt = av_packet_alloc();
    if (!in_pkt) {
        fprintf(stderr, "Failed to packet_alloc."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }

    out_pkt = av_packet_alloc();
    if (!out_pkt) {
        fprintf(stderr, "Failed to packet_alloc."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }

    in_frame = av_frame_alloc();
    if (!in_frame) {
        fprintf(stderr, "Failed to frame_alloc."
                        " On line: %d.\n", __LINE__);
        goto cleanup;
    }

    if (!cpy_video || !cpy_audio) {
        out_frame = av_frame_alloc();
        if (!out_frame) {
            fprintf(stderr, "Failed to frame_alloc."
                            " On line: %d.\n", __LINE__);
            goto cleanup;
        }
    }

    /*
     *  READING INPUT FILEs PACKETS AND TRANSCODE OR REMUX THEM
     */

    for (;;) {
        AVCodecParameters *codecpar = nullptr;
        int                finished = 0;

        if (reading) {
            res = av_read_frame(ifmt_ctx, in_pkt);
            if (res < 0) {
                if (res == AVERROR_EOF) {
                    finished = 1;
                    break;
                }
                else {
                    fprintf(stderr, "Failed to read_frame."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }
            }
        }

        codecpar = ifmt_ctx->streams[in_pkt->stream_index]->codecpar;

        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            ++video_type_counter;
        }
        else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            ++audio_type_counter;
        }

        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
            data->vcodec != AV_CODEC_ID_NONE)
        {
            puts("video!");

            av_packet_rescale_ts(in_pkt, ifmt_ctx->streams[in_pkt->stream_index]->time_base, dec_ctx[0]->time_base);

            if (!cpy_video) {
                res = avcodec_send_packet(dec_ctx[0], in_pkt);
                if (res < 0) {
                    fprintf(stderr, "Failed to send_packet."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }

                while (res >= 0) {
                    puts("v frame!");

                    res = avcodec_receive_frame(dec_ctx[0], in_frame);
                    if (res == AVERROR(EAGAIN) || res == AVERROR_EOF) {
                        break;
                    }
                    else if (res < 0) {
                        fprintf(stderr, "Failed to receive_frame."
                                        " On line: %d.\n", __LINE__);
                        goto cleanup;
                    }

                    /* if it is a video, then check pts.
                     * otherwise it is just an img,
                     * where pts isn't defined\used
                     */
                    if (in_frame->pts == AV_NOPTS_VALUE &&
                        data->acodec != AV_CODEC_ID_NONE)
                    {
                        puts("AV_NOPTS_VALUE so continue");
                        //fprintf(stderr, "\nin_frame->pts is nopts_value.\n\n");
                        continue;
                    }

                    /*
                     *  OUT_FRAME
                     */

                    out_frame->format = enc_ctx[0]->pix_fmt;
                    out_frame->width  = enc_ctx[0]->width;
                    out_frame->height = enc_ctx[0]->height;

                    res = av_image_alloc(out_frame->data,
                                         out_frame->linesize,
                                         enc_ctx[0]->width,
                                         enc_ctx[0]->height,
                                         enc_ctx[0]->pix_fmt,
                                         32);
                    if (res < 0) {
                        fprintf(stderr, "failed to av_image_alloc."
                                        "On line: %d.\n", __LINE__);
                        av_frame_free(&out_frame);
                        goto cleanup;
                    }

                    res = sws_scale(sws_ctx,
                                    (const uint8_t *const *)in_frame,
                                    in_frame->linesize, 0,
                                    in_frame->height, out_frame->data,
                                    out_frame->linesize);
                    if (res < 0) {
                        fprintf(stderr, "failed to sws_scale."
                                        "On line: %d.\n", __LINE__);
                        sws_freeContext(sws_ctx);
                        av_freep(&out_frame->data[0]);
                        av_frame_free(&out_frame);
                        goto cleanup;
                    }

                    puts("encoding video");

                    res = encode_video(out_frame, ofmt_ctx, ifmt_ctx,
                                       enc_ctx[0], out_pkt, video_index);
                    if (res < 0) {
                        fprintf(stderr, "Failed to encode_video."
                                        "On line: %d.\n", __LINE__);
                        goto cleanup;
                    }

                    av_freep(&out_frame->data[0]);
                    av_frame_unref(in_frame);
                    av_frame_unref(out_frame);
                }
                av_packet_unref(in_pkt);
            }
            else { /* REMUX VIDEO */
                av_packet_rescale_ts(in_pkt,
                                     ifmt_ctx->streams[video_index]->time_base,
                                     ofmt_ctx->streams[video_index]->time_base);

                res = av_interleaved_write_frame(ofmt_ctx, in_pkt);
                if (res < 0) {
                    fprintf(stderr, "Failed to interleaved_write_frame."
                                    " On line: %d.\n", __LINE__);
                    goto cleanup;
                }
                av_packet_unref(in_pkt);
            }
        }
        else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO &&
                   data->acodec != AV_CODEC_ID_NONE)
        {
            av_packet_rescale_ts(in_pkt, ofmt_ctx->streams[in_pkt->stream_index]->time_base, dec_ctx[1]->time_base);

            if (!cpy_audio) {
                if (av_audio_fifo_size(fifo) < aframe_size) {
                    uint8_t **converted_in_samples = nullptr;

                    reading = 1;

                    res = avcodec_send_packet(dec_ctx[1], in_pkt);
                    if (res < 0) {
                        fprintf(stderr, "Failed to send_packet."
                                        " On line: %d.\n", __LINE__);
                        goto cleanup;
                    }

                    while (res >= 0) {
                        int64_t  audio_channel_len    = 0;
                        int64_t  converted_samples_nb = 0;
                        //int64_t  delay                = 0;
                        uint8_t *buffer               = nullptr;
                        int      buffer_padding       = 0;

                        res = avcodec_receive_frame(dec_ctx[1], in_frame);

                        if (res == AVERROR(EAGAIN)) {
                            break;
                        }
                        else if (res == AVERROR_EOF) {
                            finished = 1;
                            break;
                        }
                        else if (res < 0) {
                            fprintf(stderr, "Failed to receive_frame."
                                            " On line: %d.\n", __LINE__);
                            goto cleanup;
                        }

                        if (finished) {
                            res = 0;
                            break;
                        }

                        if (in_frame->nb_samples == 0) {
                            fprintf(stderr, "\n\nFailed. in_frame is empty.\n\n");
                            continue;
                        }
                        if (in_frame->pts == AV_NOPTS_VALUE) {
                            //fprintf(stderr, "\n\nin_frame->pts is nopts_value.\n\n");
                            continue;
                        }

                        samples_counter += in_frame->nb_samples;
                        ++frames_counter;

                        converted_in_samples = (uint8_t**)malloc(
                            enc_ctx[1]->ch_layout.nb_channels *  sizeof(uint8_t*));
                        if (!converted_in_samples) {
                            fprintf(stderr, "Failed to malloc "
                                            "converted_in_samples."
                                            " On line: %d.\n", __LINE__);
                            goto cleanup;
                        }

                        audio_channel_len = in_frame->nb_samples * av_get_bytes_per_sample(enc_ctx[1]->sample_fmt) * sizeof(uint8_t);

                        buffer = (uint8_t*)malloc(audio_channel_len * enc_ctx[1]->ch_layout.nb_channels * sizeof(uint8_t));
                        if (!buffer) {
                            fprintf(stderr,
                                    "Failed to malloc buffer. Line: %d. Err: %s.\n",
                                    __LINE__, strerror(errno));
                            goto free_converted_in_samples;
                        }

                        for (int i = 0; i < enc_ctx[1]->ch_layout.nb_channels; ++i) {
                            converted_in_samples[i] = buffer + buffer_padding;
                            buffer_padding += audio_channel_len;
                        }

                        converted_samples_nb = swr_convert(swr_ctx, converted_in_samples,
                                                           in_frame->nb_samples,
                                                           (const uint8_t**)in_frame->extended_data,
                                                           in_frame->nb_samples);
                        if (converted_samples_nb < 0) {
                            fprintf(stderr, "Failed to swr_convert."
                                            " On line: %d.\n", __LINE__);
                            goto free_channels_buffer;
                        }

                        res = av_audio_fifo_realloc(fifo,
                                                    av_audio_fifo_size(fifo)
                                                        + converted_samples_nb);
                        if (res < 0) {
                            fprintf(stderr, "Failed to fifo_realloc."
                                            " On line: %d.\n", __LINE__);
                            goto free_channels_buffer;
                        }

                        res = av_audio_fifo_write(fifo,
                                                  (void**)converted_in_samples,
                                                  converted_samples_nb);
                        if (res < converted_samples_nb) {
                            fprintf(stderr, "Failed to fifo_write."
                                            " On line: %d.\n", __LINE__);
                            goto free_channels_buffer;
                        }

                        /* cleanup */
                        free(buffer);
                        free(converted_in_samples);

                        continue;

                        /* error_cleanup */
                    free_channels_buffer:
                        free(buffer);

                    free_converted_in_samples:
                        free(converted_in_samples);

                        goto cleanup;
                    }
                    av_packet_unref(in_pkt);
                }
                else { /* FIFO == enc_ctx[1]->frame_size. Now write fifo's sample in ofmt_ctx */
                    reading = 0;
                }

                while ((av_audio_fifo_size(fifo) >= aframe_size)
                       || (finished && av_audio_fifo_size(fifo) > 0)) {
                    const int frame_size = std::min(av_audio_fifo_size(fifo),
                                                    aframe_size);

                    out_frame->nb_samples  = frame_size;
                    av_channel_layout_copy(&out_frame->ch_layout,
                                           &enc_ctx[1]->ch_layout);
                    out_frame->format      = enc_ctx[1]->sample_fmt;
                    out_frame->sample_rate = enc_ctx[1]->sample_rate;

                    res = av_frame_get_buffer(out_frame, 0);
                    if (res < 0) {
                        char err_buff[64] = {'\0'};
                        av_strerror(res, err_buff, sizeof(err_buff));

                        fprintf(stderr, "Failed to frame_get_buffer."
                                        " On line: %d. Err: %s.\n", __LINE__, err_buff);
                        goto cleanup;
                    }

                    res = av_audio_fifo_read(fifo, (void**)out_frame->data,
                                             frame_size);
                    if (res < frame_size) {
                        fprintf(stderr, "Failed to fifo_read."
                                        " On line: %d.\n", __LINE__);
                        goto cleanup;
                    }

                    int data_written;
                    res = encode_audio_frame(out_frame, ofmt_ctx, ifmt_ctx,
                                             enc_ctx[1], out_pkt,
                                             &data_written, audio_index);
                    if (res) {
                        fprintf(stderr, "Failed to encode_audio_frame."
                                        " On line: %d.\n", __LINE__);
                        printf("\nenc_ctx[1]:\n"
                               "\tsample_rate: %d;\n"
                               "\tsample_fmt:  %d;\n"
                               "\tbit_rate:    %" PRId64 ";\n"
                               "\tframe_size:  %d;\n"
                               "\taframe_size: %d.\n"
                               "out_frame:\n"
                               "\tsample_rate: %d;\n"
                               "\tnb_samples: %d;\n"
                               "\tformat: %d;\n"
                               "\thas data: %s;\n"
                               "\thas extended_data: %s;\n"
                               "\tchannels: %d;\n"
                               "\tdata[0]: %p;\n"
                               "\t$ %zu, %f.\n",
                               enc_ctx[1]->sample_rate,
                               enc_ctx[1]->sample_fmt,
                               enc_ctx[1]->bit_rate,
                               enc_ctx[1]->frame_size,
                               aframe_size,
                               out_frame->sample_rate,
                               out_frame->nb_samples,
                               out_frame->format,
                               "TRUE",
                               out_frame->extended_data ? "TRUE" : "FALSE",
                               out_frame->ch_layout.nb_channels,
                               (void*)out_frame->data[0],
                               out_frame->pts, out_frame->pts * av_q2d(ofmt_ctx->streams[audio_index]->time_base));

                        fflush(stderr);
                        goto cleanup;
                    }
                }
            }
            else { /* REMUX AUDIO */
                av_packet_rescale_ts(in_pkt,
                                     ifmt_ctx->streams[audio_index]->time_base,
                                     ofmt_ctx->streams[audio_index]->time_base);
                res = av_interleaved_write_frame(ofmt_ctx, in_pkt);
                if (res < 0) {
                    fprintf(stderr,
                            "Failed to interleaved_write_frame."
                            " On line: %d.\n",
                            __LINE__);
                    goto cleanup;
                }
                av_packet_unref(in_pkt);
            }
        }
    }

    printf("\n@@@ video_types: %zu | audio_types: %zu.\n", video_type_counter, audio_type_counter);
    puts("# convertation is done!");

    /* Flushing video */
    if (data->vcodec != AV_CODEC_ID_NONE && !cpy_video) {
        do {
            res = encode_video(NULL, ofmt_ctx, ifmt_ctx,
                               enc_ctx[0], out_pkt, video_index);
        } while (res == 0);
    }

    if (!swr_ctx) {
        log_str("no audio provided. goint to write_trailer.\n");
        goto write_trailer;
    }

    /* Flushing swr_ctx */

    av_frame_unref(out_frame);
    av_packet_unref(out_pkt);

    printf("\ngoing to flush swr. delay: %" PRId64 ".\n", swr_get_delay(swr_ctx, enc_ctx[1]->sample_rate));

    for (;;) {
        int64_t delay = swr_get_delay(swr_ctx, enc_ctx[1]->sample_rate);
        int flushed = 0;

        if (delay <= 0) {
            puts("\tFinally delay is 0.\n");
            break;
        }

        delay = delay < 64000 ? delay : 64000; /* bigger value (or less) can be used instead of 64000 */

        uint8_t **flush_data = (uint8_t**)malloc(enc_ctx[1]->ch_layout.nb_channels * sizeof(uint8_t*));
        if (!flush_data) {
            fprintf(stderr, "Failed to calloc flush data. On line: %d.\n", __LINE__);
            goto cleanup;
        }

        res = av_samples_alloc(flush_data, nullptr,
                               enc_ctx[1]->ch_layout.nb_channels,
                               delay,
                               enc_ctx[1]->sample_fmt, 0);
        if (res < 0) {
            fprintf(stderr, "Failed to semples_alloc."
                            " On line: %d.\n", __LINE__);
            free(flush_data);
            goto cleanup;
        }

        flushed = swr_convert(swr_ctx, flush_data, delay, nullptr, 0);

        if (flushed > 0) {
            res = av_audio_fifo_realloc(fifo,
                                        av_audio_fifo_size(fifo)
                                            + delay);
            if (res < 0) {
                fprintf(stderr, "Failed to fifo_realloc."
                                " On line: %d.\n", __LINE__);
                free(flush_data);
                goto cleanup;
            }

            res = av_audio_fifo_write(fifo,
                                      (void**)flush_data,
                                      delay);
            if (res < in_frame->nb_samples) {
                fprintf(stderr, "Failed to fifo_write."
                                " On line: %d.\n", __LINE__);
                free(flush_data);
                goto cleanup;
            }

            while (av_audio_fifo_size(fifo) >= aframe_size) {
                const int frame_size = std::min(av_audio_fifo_size(fifo),
                                                aframe_size);

                out_frame->nb_samples  = frame_size;
                av_channel_layout_copy(&out_frame->ch_layout,
                                       &enc_ctx[1]->ch_layout);
                out_frame->format      = enc_ctx[1]->sample_fmt;
                out_frame->sample_rate = enc_ctx[1]->sample_rate;

                res = av_frame_get_buffer(out_frame, 0);
                if (res < 0) {
                    fprintf(stderr, "Failed to frame_get_buffer."
                                    " On line: %d.\n", __LINE__);
                    free(flush_data);
                    goto cleanup;
                }

                res = av_audio_fifo_read(fifo, (void**)out_frame->data,
                                         frame_size);
                if (res < frame_size) {
                    fprintf(stderr, "Failed to fifo_read."
                                    " On line: %d.\n", __LINE__);
                    free(flush_data);
                    goto cleanup;
                }

                int data_written;
                res = encode_audio_frame(out_frame, ofmt_ctx, ifmt_ctx,
                                         enc_ctx[1], out_pkt,
                                         &data_written, audio_index);
                if (res) {
                    fprintf(stderr, "Failed to encode_audio_frame."
                                    " On line: %d.\n", __LINE__);
                    free(flush_data);
                    goto cleanup;
                }

                av_frame_unref(out_frame);
                av_packet_unref(out_pkt);
            }
        }
        else {
            free(flush_data);
            break;
        }

        free(flush_data);
    }

    /* Flushing audio */
    if (data->acodec != AV_CODEC_ID_NONE && !cpy_audio) {
        int data_written   = 0;
        int flushed_frames = 0;

        puts("flushing audio");

        if (av_audio_fifo_size(fifo) > 0) {
            const int frame_size = std::min(av_audio_fifo_size(fifo),
                                            aframe_size);

            out_frame->nb_samples  = frame_size;
            av_channel_layout_copy(&out_frame->ch_layout,
                                   &enc_ctx[1]->ch_layout);
            out_frame->format      = enc_ctx[1]->sample_fmt;
            out_frame->sample_rate = enc_ctx[1]->sample_rate;

            res = av_frame_get_buffer(out_frame, 0);
            if (res < 0) {
                fprintf(stderr, "Failed to frame_get_buffer."
                                " On line: %d.\n", __LINE__);
                goto cleanup;
            }

            res = av_audio_fifo_read(fifo, (void**)out_frame->data,
                                     frame_size);
            if (res < frame_size) {
                fprintf(stderr, "Failed to fifo_read."
                                " On line: %d.\n", __LINE__);
                goto cleanup;
            }

            res = encode_audio_frame(out_frame, ofmt_ctx, ifmt_ctx,
                                     enc_ctx[1], out_pkt,
                                     &data_written, audio_index);
            if (res) {
                fprintf(stderr, "Failed to encode_audio_frame."
                                " On line: %d.\n", __LINE__);
                goto cleanup;
            }
        }

        do {
            ++flushed_frames;
            printf(">>> flushing frame: %d.\n", flushed_frames);
            res = encode_audio_frame(NULL, ofmt_ctx, ifmt_ctx,
                                     enc_ctx[1], out_pkt,
                                     &data_written, audio_index);
            if (res < 0) {
                fprintf(stderr, "Failed to encode_audio_frame."
                                " On line: %d.\n", __LINE__);
                goto cleanup;
            }
        } while (data_written);

        res = av_interleaved_write_frame(ofmt_ctx, NULL);
        if (res < 0) {
            fprintf(stderr, "Failed to flush write_frame."
                            " On line: %d.\n", __LINE__);
            goto cleanup;
        }
    }

write_trailer:
    puts("> trailer");
    res = av_write_trailer(ofmt_ctx);
    if (res < 0) {
        fprintf(stderr,
                "Failed to write_trailer."
                " On line: %d.\n", __LINE__);
        goto cleanup;
    }
    puts("< trailer");

cleanup:
    puts("convert clenaup");
    fflush(stderr);
    fflush(stdout);
    if (fifo) {
        av_audio_fifo_free(fifo);
    }
    if (swr_ctx) {
        swr_free(&swr_ctx);
    }

    if (in_pkt) {
        av_packet_free(&in_pkt);
    }
    if (out_pkt) {
        av_packet_free(&out_pkt);
    }

    if (in_frame) {
        av_frame_free(&in_frame);
    }
    if (out_frame) {
        av_frame_free(&out_frame);
    }

    if (sws_ctx) {
        sws_freeContext(sws_ctx);
    }

    if (enc_ctx[0]) {
        avcodec_free_context(&enc_ctx[0]);
    }
    if (enc_ctx[1]) {
        avcodec_free_context(&enc_ctx[1]);
    }

    if (dec_ctx[0]) {
        avcodec_free_context(&dec_ctx[0]);
    }
    if (dec_ctx[1]) {
        avcodec_free_context(&dec_ctx[1]);
    }

    if (ofmt_ctx->pb) {
        avio_closep(&ofmt_ctx->pb);
    }
    if (ofmt_ctx) {
        avformat_free_context(ofmt_ctx);
    }

    if (ifmt_ctx) {
        avformat_close_input(&ifmt_ctx);
    }

    log_str("\n\n[*][convert]res: %d.\n\n", res);

    return res;
}
