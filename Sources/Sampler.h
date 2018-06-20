#ifndef SAMPLER_H
#define SAMPLER_H

// new types
#include "types.h"

#define SAMPLES_PER_CYCLE 20
#define LFR 47.5 // lowest freq range
#define HFR 52.5 // highes freq range
#define SAMPLE_ARRAY_SIZE 24 // amount of samples to hold at once
#define SAMPLE_WINDOW_SIZE 3 // amount of crosses that should be visible

typedef struct
{
  // uint8_t channelNb;
  uint8_t Crosses;
  uint8_t Iterator;
  float Samples[SAMPLE_ARRAY_SIZE];
  uint8_t CrossPositions[SAMPLE_WINDOW_SIZE];
} TSamplerArray;

void Sampler_Init(const uint32_t moduleClk);

float Sampler_GetRMS(/* channelNb, floatPointer */);
