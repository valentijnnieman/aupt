#include <algorithm>
#include <complex>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include <vector>

#include "portaudio.h"

#include "midi_controller.hpp"
#include <cmath>
#include <numbers>

#define FRAMES_PER_BUFFER (512)

static int callback(const void *input, void *output, unsigned long frameCount,
                    const PaStreamCallbackTimeInfo *timeInfo,
                    PaStreamCallbackFlags statusFlags, void *userData) {
  callback_data_s *data = (callback_data_s *)userData;
  float *out = (float *)output;
  std::vector<float> buffer(frameCount, 0.0f);
  memset(out, 0, sizeof(float) * frameCount);

  float amp = 1.0f;

  for (int i = 0; i < frameCount; i++) {
    *out++ = buffer[i];
    *out++ = buffer[i]; // stereo
  }

  return paContinue;
}

int main(int argc, const char *argv[]) {
  std::vector<std::string> args(argv + 1, argv + argc);

  int midi_in = 1;
  int midi_out = 1;
  int device = 0;

  for (int i = 0; i < args.size(); i++) {
    if (args[i] == "-midi") {
      midi_in = stoi(args[i + 1]);
      midi_out = midi_in;
      std::cout << "[Sampler] midi i/o ports: " << midi_in << std::endl;
    }
    if (args[i] == "-device") {
      device = stoi(args[i + 1]);
      std::cout << "[Sampler] device: " << device << std::endl;
    } else {
    }
  }

  std::vector<float> samples;

  PaStream *stream;
  PaError error;
  callback_data_s data;

  MidiController ctrl;

  for (auto &name : ctrl.portNames) {
    std::cout << name << std::endl;
  }

  ctrl.midiIn->openPort(midi_in);
  ctrl.midiIn->setCallback(ctrl.callback, &data);

  ctrl.midiOut->openPort(midi_out);

  /* init portaudio */
  error = Pa_Initialize();
  if (error != paNoError) {
    fprintf(stderr, "Problem initializing\n");
    return 1;
  }
  PaStreamParameters outputParameters;

  int numDevices = Pa_GetDeviceCount();
  if (numDevices < 0) {
    printf("ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices);
  }
  const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(device);
  printf("Name                        = %s\n", deviceInfo->name);
  printf("Host API                    = %s\n",
         Pa_GetHostApiInfo(deviceInfo->hostApi)->name);
  printf("Max inputs = %d", deviceInfo->maxInputChannels);
  printf(", Max outputs = %d\n", deviceInfo->maxOutputChannels);

  printf("Default low input latency   = %8.4f\n",
         deviceInfo->defaultLowInputLatency);
  printf("Default low output latency  = %8.4f\n",
         deviceInfo->defaultLowOutputLatency);
  printf("Default high input latency  = %8.4f\n",
         deviceInfo->defaultHighInputLatency);
  printf("Default high output latency = %8.4f\n",
         deviceInfo->defaultHighOutputLatency);

  outputParameters.device = device;
  if (outputParameters.device == paNoDevice) {
    fprintf(stderr, "Error: No default output device.\n");
  }
  outputParameters.channelCount = 2;
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency =
      Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  error =
      Pa_OpenStream(&stream, NULL, &outputParameters, /* &outputParameters, */
                    44100, 256, paNoFlag, callback, &data);

  if (error != paNoError) {
    fprintf(stderr, "Problem opening Default Stream\n");
    return 1;
  }

  /* Start the stream */
  error = Pa_StartStream(stream);
  if (error != paNoError) {
    fprintf(stderr, "Problem opening starting Stream\n");
    return 1;
  }

  std::cout << "Done! opening stream..." << std::endl;
  /* Run until EOF is reached */
  while (Pa_IsStreamActive(stream)) {
    // Pa_Sleep(1000);
  }

  /*  Shut down portaudio */
  error = Pa_CloseStream(stream);
  if (error != paNoError) {
    fprintf(stderr, "Problem closing stream\n");
    return 1;
  }

  error = Pa_Terminate();
  if (error != paNoError) {
    fprintf(stderr, "Problem terminating\n");
    return 1;
  }

  return 0;
}
