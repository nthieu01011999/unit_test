// https://equalarea.com/paul/alsa-audio.html
// opus convert ref: https://blog.csdn.net/sinat_27720649/article/details/126530085

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <opus.h>

#define CHANNELS	2
#define MAX_FRAME_SIZE	6*960
#define MAX_PACKET_SIZE (3*1276)

void initialize_audio_capture(snd_pcm_t **capture_handle, snd_pcm_hw_params_t **hw_params, const char *device, int rate, int channels);
void initialize_opus_encoder_decoder(OpusEncoder **encoder, OpusDecoder **decoder, int rate, int channels);
void cleanup(snd_pcm_t *capture_handle, OpusEncoder *encoder, OpusDecoder *decoder, FILE *fp_out, FILE *fp2_out, FILE *fp3_out);
void process_audio(snd_pcm_t *capture_handle, OpusEncoder *encoder, OpusDecoder *decoder, FILE *fp_out, FILE *fp2_out, FILE *fp3_out, int channels);

int main(int argc, char *argv[]) {
    int rate, channels;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    FILE *fp_out, *fp2_out, *fp3_out;
    OpusEncoder *encoder;
    OpusDecoder *decoder;

    if (argc < 7) {
        printf("usage: %s <device> <sample_rate> <channels> <out_file> <out_file2> <out_file3>\n", argv[0]);
        return 0;
    }

    rate = atoi(argv[2]);
    channels = atoi(argv[3]);

    fp_out = fopen(argv[4], "wb");
    fp2_out = fopen(argv[5], "wb");
    fp3_out = fopen(argv[6], "wb");

    initialize_audio_capture(&capture_handle, &hw_params, argv[1], rate, channels);
    initialize_opus_encoder_decoder(&encoder, &decoder, rate, channels);

    process_audio(capture_handle, encoder, decoder, fp_out, fp2_out, fp3_out, channels);

    cleanup(capture_handle, encoder, decoder, fp_out, fp2_out, fp3_out);

    return 0;
}

void initialize_audio_capture(snd_pcm_t **capture_handle, snd_pcm_hw_params_t **hw_params, const char *device, int rate, int channels) {
    int err;

    if ((err = snd_pcm_open(capture_handle, device, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "cannot open audio device %s (%s)\n", device, snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_open ok\n");

    if ((err = snd_pcm_hw_params_malloc(hw_params)) < 0) {
        fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_hw_params_malloc ok\n");

    if ((err = snd_pcm_hw_params_any(*capture_handle, *hw_params)) < 0) {
        fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_hw_params_any ok\n");

    if ((err = snd_pcm_hw_params_set_access(*capture_handle, *hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "cannot set access type (%s)\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_hw_params_set_access ok\n");

    if ((err = snd_pcm_hw_params_set_format(*capture_handle, *hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf(stderr, "cannot set sample format (%s)\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_hw_params_set_format ok\n");

    if ((err = snd_pcm_hw_params_set_rate_near(*capture_handle, *hw_params, &rate, 0)) < 0) {
        fprintf(stderr, "cannot set sample rate (%s)\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_hw_params_set_rate_near ok\n");

    if ((err = snd_pcm_hw_params_set_channels(*capture_handle, *hw_params, channels)) < 0) {
        fprintf(stderr, "cannot set channel count (%s)\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_hw_params_set_channels ok\n");

    if ((err = snd_pcm_hw_params(*capture_handle, *hw_params)) < 0) {
        fprintf(stderr, "cannot set parameters (%s)\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_hw_params ok\n");

    snd_pcm_hw_params_free(*hw_params);

    if ((err = snd_pcm_prepare(*capture_handle)) < 0) {
        fprintf(stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("snd_pcm_prepare ok\n");
}

void initialize_opus_encoder_decoder(OpusEncoder **encoder, OpusDecoder **decoder, int rate, int channels) {
    int err;

    *encoder = opus_encoder_create(rate, channels, OPUS_APPLICATION_AUDIO, &err);
    if (err < 0) {
        fprintf(stderr, "failed to create an encoder: %s\n", opus_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("opus_encoder_create ok\n");

    err = opus_encoder_ctl(*encoder, OPUS_SET_BITRATE(OPUS_AUTO));
    if (err < 0) {
        fprintf(stderr, "failed to set bitrate: %s\n", opus_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("opus_set_bitrate ok\n");

    *decoder = opus_decoder_create(rate, channels, &err);
    if (err < 0) {
        fprintf(stderr, "failed to create decoder: %s\n", opus_strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("opus_decoder_create ok\n");
}

void process_audio(snd_pcm_t *capture_handle, OpusEncoder *encoder, OpusDecoder *decoder, FILE *fp_out, FILE *fp2_out, FILE *fp3_out, int channels) {
    int err, i, j, nbBytes, frame_size;
    char buf[160 * 4];
    unsigned char cbits[MAX_PACKET_SIZE];
    opus_int16 out[MAX_FRAME_SIZE * CHANNELS];
    unsigned char pcm_bytes[MAX_FRAME_SIZE * CHANNELS * 2];

    for (j = 0; j < 100 * 5; j++) {
        if ((err = snd_pcm_readi(capture_handle, buf, 160)) != 160) {
            fprintf(stderr, "read from audio interface failed (%s)\n", snd_strerror(err));
            break;
        }
        if (fp_out) {
            fwrite(buf, 1, err * 2 * channels, fp_out);
        }

        nbBytes = opus_encode(encoder, (opus_int16 *)buf, 160, cbits, MAX_PACKET_SIZE);
        if (nbBytes < 0) {
            fprintf(stderr, "encode failed: %s\n", opus_strerror(nbBytes));
            exit(EXIT_FAILURE);
        }
        if (fp2_out) {
            fwrite(cbits, 1, nbBytes, fp2_out);
        }

        frame_size = opus_decode(decoder, cbits, nbBytes, out, MAX_FRAME_SIZE, 0);
        if (frame_size < 0) {
            fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
            exit(EXIT_FAILURE);
        }
        printf("opus_decode frame_size return: %d\n", frame_size);

        for (i = 0; i < CHANNELS * frame_size; i++) {
            pcm_bytes[2 * i] = out[i] & 0xFF;
            pcm_bytes[2 * i + 1] = (out[i] >> 8) & 0xFF;
        }
        fwrite(pcm_bytes, sizeof(short), frame_size * CHANNELS, fp3_out);
    }
}

void cleanup(snd_pcm_t *capture_handle, OpusEncoder *encoder, OpusDecoder *decoder, FILE *fp_out, FILE *fp2_out, FILE *fp3_out) {
    opus_encoder_destroy(encoder);
    opus_decoder_destroy(decoder);
    snd_pcm_close(capture_handle);

    if (fp_out) fclose(fp_out);
    if (fp2_out) fclose(fp2_out);
    if (fp3_out) fclose(fp3_out);
}
