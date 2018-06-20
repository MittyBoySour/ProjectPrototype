
#include "Sampler.h"
#include "PIT.h"

// Analog functions
#include "analog.h"

#define CHANNEL_0 0

static TSamplerArray SamplerArray; // later multiple

volatile float PreviousFrequency;
volatile float CurrentFrequency;
volatile bool increaseSampleFrequency;

void Sampler_Init(const uint32_t moduleClk)
{
  // initialise sampling rate etc.
  PreviousFrequency = LFR;
  bool increaseSampleFrequency = true; // actually equal to another function, can remove later
  CurrentFrequency = WindowAdjuster(increaseSampleFrequency, PreviousFrequency);

  PIT_Init(moduleClk, TakeSample);

  // take initial samples and then make adjustments
  // while crosses (not 3 or 4) scrap samples and redo freq checker
  // while (not 16+ samps between cross 0, 2)
  TakeInitialSamples();
}

float WindowAdjuster(const bool increaseSampleFrequency, const uint8_t PreviousFrequency)
{
  if (increaseSampleFrequency)
    return ((HFR - PrevFreq) / 2) + PreviousFrequency;
  else
    return PreviousFrequency - ((PreviousFrequency - LFR) / 2);
}

float Sampler_GetRMS() // take channel number and place to store the data, return bool
{
  const uint8_t cycleIterator = SamplerArray.CrossPosition[0];
  const uint8_t cycleEnd = SamplerArray.CrossPosition[2];
  float allSquared = 0; // check for vol

  for (int i = cycleIterator; i < cycleEnd; i++)
  {
    if (i == SAMPLE_ARRAY_SIZE)
      i = 0;
    allSquared += (SamplerArray[i] * SamplerArray[i]);
  }

  float meanSquared = allSquared * (1 / (cycleEnd - cycleIterator));

  // store result in passed and return any error
  return sqrt(meanSquared);
}

void TakeSample(const bool initialSamples)
{
  int16_t analogInputValue;
  // Get analog sample
  Analog_Get(CHANNEL_0, &analogInputValue);
  // Put analog sample
  Analog_Put(CHANNEL_0, analogInputValue);

  // pass off to another thread eventually
  // check if cross removed and adjust crosses
  if (SamplerArray.CrossPositions[0] == SamplerArray.Iterator)
    AdjustCrosses();

  SamplerArray.Samples[SamplerArray.Iterator] = analogInputValue;

  CheckStateChange();

  SamplerArray.Iterator++;

}

void AdjustCrosses()
{
  // check for null or crosses
  // probably change to foreach
  SamplerArray.CrossPositions[0] = SamplerArray.CrossPositions[1];
  SamplerArray.CrossPositions[1] = SamplerArray.CrossPositions[2];
  SamplerArray.CrossPositions[2] = NULL; // not necessary
}

void CheckStateChange()
{
  // check for null
  bool previousAmpPos = (SamplerArray.Samples[SamplerArray.Iterator-1] >= 0);
  bool currentAmpPos = (SamplerArray.Samples[SamplerArray.Iterator] >= 0);
  if (previousAmpPos == currentAmpPos)
    // set current position as new CrossPosition
    SamplerArray.CrossPositions[SamplerArray.Crosses++] = SamplerArray.Iterator;
}

bool IncreaseSampleFrequency()
{
  if ()
  {

  }

}
