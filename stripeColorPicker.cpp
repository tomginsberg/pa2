#include "stripeColorPicker.h"

stripeColorPicker::stripeColorPicker(HSLAPixel fillColor, int stripeSpacing)
{
    this->color = fillColor;
    this->spacing = stripeSpacing;
}

HSLAPixel stripeColorPicker::operator()(int x, int y)
{
//TODO: Implement this
}
