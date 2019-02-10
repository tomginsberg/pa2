#include "borderColorPicker.h"

borderColorPicker::borderColorPicker(HSLAPixel fillColor, PNG & img, double tolerance, HSLAPixel center)
{
    this->color = fillColor;
    this->im = img;
    this->tol = tolerance;
    this->ctr = center;
}

HSLAPixel borderColorPicker::operator()(int x, int y)
{
//TODO: Implement this
}
