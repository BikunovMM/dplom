#ifndef CONVERTER_H
#define CONVERTER_H

#include <cstdlib>
#include <cstring>
#include <cinttypes>

#include <map>
#include <string>
#include <vector>

#include "utils.h"

extern "C" {

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>

#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libavutil/audio_fifo.h>

}

#define IMG_EXTS_LIST \
    X(png)       \
    X(jpg)       \
    X(jpeg)      \
    X(ico)       \
    X(bmp)

#define VID_EXTS_LIST \
    X(mp4)       \
    X(mov)       \
    X(mkv)       \
    X(webm)

#define AUD_EXTS_LIST \
    X(mp3)       \
    X(opus)      \
    X(wav)       \
    X(aac)       \
    X(flac)      \
    X(ogg)

namespace conv
{
    enum class MediaType {
        UNKNOWN,
        IMG,
        VID,
        AUD
    };

    enum class ImgFmts{
        #define X(name) name,
            IMG_EXTS_LIST
        #undef X
    };
    enum class VidFmts{
        #define X(name) name,
            VID_EXTS_LIST
        #undef X
    };
    enum class AudFmts{
        #define X(name) name,
            AUD_EXTS_LIST
        #undef X
    };

    typedef struct {
        MediaType    type;
        const char **exts;
        int          exts_len;
        int          ext_ind;
    } FmtData;

    typedef struct {
        const char *vcodec;
        const char *acodec;
        const char *pixfmt;
        const char *infmt;
        std::map<const std::string, const std::string> metadata;

        unsigned int nb_streams;
        unsigned int frame_w;
        unsigned int frame_h;

        int64_t duration;
        int sample_rate;
        int64_t bit_rate;
    } FileData;

    typedef struct {
        AVCodecID      vcodec;
        AVCodecID      acodec;
        AVPixelFormat      pixfmt;
        AVSampleFormat      smplfmt;
        int      max_width;
        int      smplrate;
        int64_t  bitrate;
        int      nb_channels;
        int      frame_size;
    } ConvData;

    typedef struct {
        AVPixelFormat   id;
        char *name;
    } PixFmt;

    typedef struct {
        AVSampleFormat   id;
        char *name;
    } SmplFmt;

    typedef struct {
        char *str;
        int   val;
    } SmplRate;

    typedef struct {
        char *str;
        int   val;
    } BitRate;

    typedef struct {
        char *str;
        int   val;
    } NbChannels;

    typedef struct {
        AVCodecID     id;
        char   *name;
        PixFmt *pixfmts;
        int     pixfmts_len;        
    } VidCodec;

    typedef struct {
        AVCodecID         id;
        char       *name;
        SmplFmt    *smplfmts;
        SmplRate   *smplrates;
        BitRate    *bitrates;
        NbChannels *nb_channels;
        int         smplfmts_len;
        int         smplrates_len;
        int         bitrates_len;
        int         nb_channels_len;
        int         frame_size;

        int def_smplrate_ind;
        int def_bitrate_ind;
    } AudCodec;

    typedef struct {
        VidCodec *codecs;
        int       len;
        int       max_width;
    } ImgData;

    typedef struct {
        AudCodec *codecs;
        int       len;
    } AudData;

    typedef struct {
        VidCodec *vcodecs;
        AudCodec *acodecs;
        int       vlen;
        int       alen;
    } VidData;



    int getFmtData(const char *fn, FmtData *fdata);
    int getFileData(const char *fn, FileData *fdata);
    int convert(const char *inpath, const char *outpath,
                const ConvData *cdata);

    ImgData* getImgParams(int index);
    AudData* getAudParams(int index);
    VidData* getVidParams(int index);

    const int IMG_EXTS_LEN = ( 0
        #define X(ext) + 1
            IMG_EXTS_LIST
        #undef X
    );

    const int VID_EXTS_LEN = ( 0
        #define X(ext) + 1
                VID_EXTS_LIST
        #undef X
    );

    const int AUD_EXTS_LEN = ( 0
        #define X(ext) + 1
                AUD_EXTS_LIST
        #undef X
    );

    inline const char *IMG_EXTS[] = {
        #define X(ext) #ext,
            IMG_EXTS_LIST
        #undef X
    };

    inline const char *VID_EXTS[] = {
        #define X(ext) #ext,
            VID_EXTS_LIST
        #undef X
    };

    inline const char *AUD_EXTS[] = {
        #define X(ext) #ext,
            AUD_EXTS_LIST
        #undef X
    };

    inline const char *img_filter =
        "Images ( "
        #define X(ext) "*." #ext " "
        IMG_EXTS_LIST
        #undef X
        ")";

    inline const char *vid_filter =
        "Videos ( "
        #define X(ext) "*." #ext " "
        VID_EXTS_LIST
        #undef X
        ")";

    inline const char *aud_filter =
        "Audio ( "
        #define X(ext) "*." #ext " "
        AUD_EXTS_LIST
        #undef X
        ")";

    inline const char *exts_filter =
        "Images ( "
        #define X(ext) "*." #ext " "
            IMG_EXTS_LIST
        #undef X
        ");;Videos ( "
        #define X(ext) "*." #ext " "
            VID_EXTS_LIST
        #undef X
        ");;Audio ( "
        #define X(ext) "*." #ext " "
            AUD_EXTS_LIST
        #undef X
        ")";
}

#endif // CONVERTER_H

