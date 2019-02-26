#ifndef CUSTOMCOLORPICKER_H
#define CUSTOMCOLORPICKER_H

#include "colorPicker.h"
#include "cs221util/HSLAPixel.h"

class customColorPicker : public colorPicker
{
public:
    
    customColorPicker(HSLAPixel fillColor, int transition);
    virtual HSLAPixel operator()(int x, int y);
    
private:
    HSLAPixel color;
    int trans;
};

#endif
