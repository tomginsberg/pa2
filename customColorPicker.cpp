#include "customColorPicker.h"
#include "filler.h"

customColorPicker::customColorPicker(HSLAPixel fillColor, int transition)
{
    this->color = fillColor;
    this->trans = transition;
}

HSLAPixel customColorPicker::operator()(int x, int y)
{
    if (x > this->trans or y > this->trans)
        return this->color;
    HSLAPixel green;
    return green;
}

