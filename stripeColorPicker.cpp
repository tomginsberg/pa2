#include "stripeColorPicker.h"

stripeColorPicker::stripeColorPicker(HSLAPixel fillColor, int stripeSpacing)
{
    this->color = fillColor;
    this->spacing = stripeSpacing;
}

HSLAPixel stripeColorPicker::operator()(int x, int y)
{
    if (x % spacing == 0 or y % spacing ==0){
        return this->color;
    }
    HSLAPixel white;
    return white;
}
