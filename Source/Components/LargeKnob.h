#ifndef LARGEKNOB_H_INCLUDED
#define LARGEKNOB_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// these represent what actually gets drawn when we say we want something thats a large or small knob
class LargeKnob : public LookAndFeel_V3
{
public:
    
//    Image sprite = ImageCache::getFromMemory(BinaryData::blackknob.jpg, BinaryData::black_knob.jpg);
    Image sprite = ImageCache::getFromMemory(BinaryData::blackknoblarge_png, BinaryData::blackknoblarge_pngSize);
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider);
};

#endif /* LARGEKNOB_H_INCLUDED */
