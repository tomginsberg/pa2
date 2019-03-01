#include "borderColorPicker.h"
#include "filler.cpp"
borderColorPicker::borderColorPicker(HSLAPixel fillColor, PNG & img, double tolerance, HSLAPixel center)
{
    this->color = fillColor;
    this->im = img;
    this->tol = tolerance;
    this->ctr = center;
}
/*
 * if p is within distance 3 of any cell that will be outside the
 *fill, then the pixel at position p should be colored with the given color.
 *
 * Colors outside the fill are defined by those whose pixel distance differs by a value greater then the tolerance.
 */
HSLAPixel borderColorPicker::operator()(int x, int y)
{
    if (x < 3) return this->color;
    if (im.width() - x <= 3) return this->color;
    if (y < 3) return this->color;
    if (im.height() - y <= 3) return this->color;
    for (int i = x-3; i<=x+3; i++) {
        for (int j = y-3; j<=y+3; j++) {
            if (pow(i-x, 2) + pow(j-y, 2) <= pow(3, 2)) {
                if ((*(im.getPixel(i, j))).dist(ctr) >= tol)
                {
                    return this->color;
                }
            }
        }
    }
    
    return *(im.getPixel(x,y));
    
}

