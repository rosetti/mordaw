/*
  ==============================================================================

    RegionComponent.h
    Created: 13 Aug 2015 12:21:11pm
    Author: Matt & Dan

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
    RegionComponent(int64 x, double sampleRate, Audio::Region* region, AudioFormatManager& formatManager, const File& file, int64 pixelsPerClip);
    ~RegionComponent();
    
    void setPixelsPerClip(int64 pixels);
	int64 getPositionX();
	int64 getRegionWidth();
    Rectangle<int> getRegionBounds() { return _bounds; };
    
    Audio::Region* getRegion();
    
    void paint (Graphics&);
    void resized();
            
private:
    double _sampleRate;
    Rectangle<int> _bounds;
    int64 _pixelsPerClip;
    String _filename;
    int64 _posX;
	int64 _lengthSeconds;
    Audio::Region* _region;
    FileInputSource* _inputSource;
    AudioThumbnail *_thumbnail;
    AudioThumbnailCache *_thumbnailCache;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RegionComponent)
};


#endif  // REGIONCOMPONENT_H_INCLUDED
