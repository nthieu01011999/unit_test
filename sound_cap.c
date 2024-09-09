// https://equalarea.com/paul/alsa-audio.html
// opus convert ref: https://blog.csdn.net/sinat_27720649/article/details/126530085

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#include <opus.h>

#define CHANNELS	2
#define MAX_FRAME_SIZE	6*960
#define MAX_PACKET_SIZE (3*1276)

int main (int argc, char *argv[]) {
  int err, i, j;
  int rate, channels;
  int nbBytes, frame_size;
  snd_pcm_t *capture_handle;
  snd_pcm_hw_params_t *hw_params;
  char buf[160 * 4];
  FILE *fp_out, *fp2_out, *fp3_out;

  OpusEncoder *encoder;
  OpusDecoder *decoder;
  unsigned char cbits[MAX_PACKET_SIZE];
  opus_int16 out[MAX_FRAME_SIZE * CHANNELS];
  unsigned char pcm_bytes[MAX_FRAME_SIZE * CHANNELS * 2];

  if (argc < 7) {
    printf("usage: %s <device> <sample_rate> <channels> <out_file> <out_file2> <out_file3>\n", argv[0]);
    return 0;
  }

  rate     = atoi(argv[2]);
  channels = atoi(argv[3]);

  fp_out = fopen(argv[4], "wb");
  fp2_out = fopen(argv[5], "wb");
  fp3_out = fopen(argv[6], "wb");

  if ((err = snd_pcm_open(&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0) {
    fprintf(stderr, "cannot open audio device %s (%s)\n", argv[1], snd_strerror(err));
    return -1;
  }
  printf("snd_pcm_open ok\n");

  if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror (err));
    return -1;
  }
  printf("snd_pcm_hw_params_malloc ok\n");

  if ((err = snd_pcm_hw_params_any(capture_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror (err));
    return -1;
  }
  printf("snd_pcm_hw_params_any ok\n");

  if ((err = snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n", snd_strerror (err));
    return -1;
  }
  printf("snd_pcm_hw_params_set_access ok\n");

  if ((err = snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
    fprintf (stderr, "cannot set sample format (%s)\n", snd_strerror (err));
    return -1;
  }
  printf("snd_pcm_hw_params_set_format ok\n");

  //val = 44100;
  if ((err = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n", snd_strerror (err));
    return -1;
  }
  printf("snd_pcm_hw_params_set_rate_near ok\n");

  if ((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, channels)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n", snd_strerror (err));
    return -1;
  }
  printf("snd_pcm_hw_params_set_channels ok\n");

  if ((err = snd_pcm_hw_params(capture_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n", snd_strerror (err));
    return -1;
  }
  printf("snd_pcm_hw_params ok\n");

  snd_pcm_hw_params_free(hw_params);

  if ((err = snd_pcm_prepare(capture_handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror (err));
    return -1;
  }
  printf("snd_pcm_prepare ok\n");

  // fs: sampling rate, one of 8000, 12000, 16000, 24000, 48000
  // channels: number of channels
  // application: encoding mode, there are three types:
  // OPUS_APPLICATION_VOIP: Processes voice signals, suitable for voip business scenarios
  // OPUS_APPLICATION_AUDIO: This mode is suitable for non-voice content such as music
  // OPUS_APPLICATION_RESTRICTED_LOWDELAY: low latency mode
  // error: Encoded return value

  encoder = opus_encoder_create(rate, channels, OPUS_APPLICATION_AUDIO, &err);
  if (err < 0) {
    fprintf(stderr, "failed to create an encoder: %s\n", opus_strerror(err));
    return -1;
  }
  printf("opus_encoder_create ok\n");

  // bitrate = sample rate x 2
  err = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(OPUS_AUTO));
  if (err < 0) {
    fprintf(stderr, "failed to set bitrate: %s\n", opus_strerror(err));
    return -1;
  }
  printf("opus_set_bitrate ok\n");

  decoder = opus_decoder_create(rate, channels, &err);
  if (err < 0) {
    fprintf(stderr, "failed to create decoder: %s\n", opus_strerror(err));
    return -1;
  }
  printf("opus_decoder_create ok\n");
  printf("sizeof(short): %d\n", sizeof(short));

  for (j = 0; j < 100 * 5; j++) {
    // sample rate 8000, 160 frames ~ 20ms duration
    if ((err = snd_pcm_readi(capture_handle, buf, 160)) != 160) {
      fprintf (stderr, "read from audio interface failed (%s)\n", snd_strerror (err));
      break;
    }
    if (fp_out) {
      fwrite(buf, 1, err * 2 * channels, fp_out);
    }

    // PCM size: samples per ms: 8000 / 1000 = 8
    // PCM bytes = 8 * 20 (ms) * 2 (bytes) * 2 (channels) = 640 (bytes)
    nbBytes = opus_encode(encoder, (opus_int16 *) buf, 160, cbits, MAX_PACKET_SIZE);
    if (nbBytes<0) {
      fprintf(stderr, "encode failed: %s\n", opus_strerror(nbBytes));
      return -1;
    }
    //printf("opus_encode return %d bytes\n", nbBytes);
    if (fp2_out) {
      fwrite(cbits, 1, nbBytes, fp2_out);
    }

    frame_size = opus_decode(decoder, cbits, nbBytes, out, MAX_FRAME_SIZE, 0);
    if (frame_size < 0) {
      fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
      return -1;
    }
    printf("opus_decode frame_size return: %d\n", frame_size);

    // Convert to little-endian ordering
    for(i = 0; i < CHANNELS * frame_size; i++) {
      pcm_bytes[2*i] = out[i] & 0xFF;
      pcm_bytes[2*i+1] = (out[i] >> 8) & 0xFF;
    }
    fwrite(pcm_bytes, sizeof(short), frame_size * CHANNELS, fp3_out);
  }

  opus_encoder_destroy(encoder);
  opus_decoder_destroy(decoder);
  snd_pcm_close(capture_handle);

  if (fp_out)
    fclose(fp_out);

  if (fp2_out)
    fclose(fp2_out);

  if (fp3_out)
    fclose(fp3_out);

  return 0;
}
