#include <stdio.h>
#include <stdlib.h>
#include <opus.h>

#define CHANNELS            2
#define SAMPLES_PER_FRAME   160 // Based on 20 ms @ 8000 Hz
#define MAX_FRAME_SIZE      6*960
#define MAX_PACKET_SIZE     (3*1276)

void read_pcm_data(FILE* fp, opus_int16* buffer, int frame_size);
void setup_opus(int rate, int channels, OpusEncoder** encoder, OpusDecoder** decoder);
void process_audio(FILE* fp_in, FILE* fp_out_encoded, FILE* fp_out_decoded, OpusEncoder* encoder, OpusDecoder* decoder);
void cleanup(OpusEncoder* encoder, OpusDecoder* decoder);

int main(int argc, char *argv[]) {
    int rate, channels;
    OpusEncoder *encoder;
    OpusDecoder *decoder;

    if (argc < 5) {
        printf("Usage: %s <input_pcm_file> <sample_rate> <channels> <output_encoded> <output_decoded>\n", argv[0]);
        return 0;
    }

    FILE *fp_in = fopen(argv[1], "rb");
    FILE *fp_out_encoded = fopen(argv[4], "wb");
    FILE *fp_out_decoded = fopen(argv[5], "wb");

    rate = atoi(argv[2]);
    channels = atoi(argv[3]);

    setup_opus(rate, channels, &encoder, &decoder);
    process_audio(fp_in, fp_out_encoded, fp_out_decoded, encoder, decoder);
    cleanup(encoder, decoder);

    fclose(fp_in);
    fclose(fp_out_encoded);
    fclose(fp_out_decoded);

    return 0;
}

void read_pcm_data(FILE* fp, opus_int16* buffer, int frame_size) {
    size_t read = fread(buffer, sizeof(opus_int16), frame_size * CHANNELS, fp);
    if (read < frame_size * CHANNELS) {
        fprintf(stderr, "Not enough data read from PCM file\n");
        exit(-1);
    }
}

void setup_opus(int rate, int channels, OpusEncoder** encoder, OpusDecoder** decoder) {
    int err;

    *encoder = opus_encoder_create(rate, channels, OPUS_APPLICATION_AUDIO, &err);
    if (err < 0) {
        fprintf(stderr, "Failed to create an encoder: %s\n", opus_strerror(err));
        exit(-1);
    }

    opus_encoder_ctl(*encoder, OPUS_SET_BITRATE(OPUS_AUTO));

    *decoder = opus_decoder_create(rate, channels, &err);
    if (err < 0) {
        fprintf(stderr, "Failed to create a decoder: %s\n", opus_strerror(err));
        exit(-1);
    }
}

void process_audio(FILE* fp_in, FILE* fp_out_encoded, FILE* fp_out_decoded, OpusEncoder* encoder, OpusDecoder* decoder) {
    unsigned char cbits[MAX_PACKET_SIZE];
    opus_int16 in[SAMPLES_PER_FRAME * CHANNELS], out[MAX_FRAME_SIZE * CHANNELS];
    int nbBytes, frame_size;

    while (!feof(fp_in)) {
        read_pcm_data(fp_in, in, SAMPLES_PER_FRAME);
        nbBytes = opus_encode(encoder, in, SAMPLES_PER_FRAME, cbits, MAX_PACKET_SIZE);
        if (nbBytes < 0) {
            fprintf(stderr, "Encode failed: %s\n", opus_strerror(nbBytes));
            exit(-1);
        }
        fwrite(cbits, 1, nbBytes, fp_out_encoded);

        frame_size = opus_decode(decoder, cbits, nbBytes, out, MAX_FRAME_SIZE, 0);
        if (frame_size < 0) {
            fprintf(stderr, "Decode failed: %s\n", opus_strerror(frame_size));
            exit(-1);
        }
        fwrite(out, sizeof(opus_int16), frame_size * CHANNELS, fp_out_decoded);
    }
}

void cleanup(OpusEncoder* encoder, OpusDecoder* decoder) {
    opus_encoder_destroy(encoder);
    opus_decoder_destroy(decoder);
}
