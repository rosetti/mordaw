/*
  ==============================================================================

    Meter.h
    Created: 1 Nov 2015 7:24:19pm
    Author:  dtl

  ==============================================================================
*/

#ifndef METER_H_INCLUDED
#define METER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Meter    : public Component,
                 public Timer
{
public:
    Meter();
    ~Meter();

    void paint (Graphics&);
    void resized();

private:
    unsigned int _numRedSegs, _numYellowSegs, _numGreenSegs;
    float _dBPerSegment;
    unsigned int _sampleCount, _numSamples;
    float _sampleMaxValue;
    bool _repaint;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter)
};


#endif  // METER_H_INCLUDED
