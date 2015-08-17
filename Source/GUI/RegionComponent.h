/*
  ==============================================================================

    RegionComponent.h
    Created: 13 Aug 2015 12:21:11pm
    Author:  Thomas

  ==============================================================================
*/

#ifndef REGIONCOMPONENT_H_INCLUDED
#define REGIONCOMPONENT_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Audio/Region.h"

//==============================================================================
/*
*/
class RegionComponent    : public Component
{
public:
    RegionComponent(int64 x, Audio::Region* region, AudioFormatManager& formatManager, const File& file);
    ~RegionComponent();

    void paint (Graphics&);
    void resized();

private:
    String filename;
    int64 _posX;
    Audio::Region* _region;
    FileInputSource _inputSource;
    AudioThumbnail _thumbnail;
    AudioThumbnailCache _thumbnailCache;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RegionComponent)
};


#endif  // REGIONCOMPONENT_H_INCLUDED
