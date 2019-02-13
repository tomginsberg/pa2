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
    /**
     * We check all pixels within a distance 3 of (x,y) here I call it o. x's indicate the pixels that we need to check
     * * * x * * *
     * x x x x x *
     x x x o x x x
     * x x x x x *
     * * * x * * *
     */
    int width = this->im.width();
    int height = this->im.height();

    //auto return color if pixel is within 3 of the border

    if( x - 3 < 0 or x + 3 >= width or y - 3 < 0 or y + 3 >= height) return this->color;

    for (int i = -2; i <= 2; i++) {
        for(int j = -2; j<= 2; j++) {
            if (this->im.getPixel(static_cast<unsigned int>(x),
                                  static_cast<unsigned int>(y))->dist(*this->im.getPixel(
                    static_cast<unsigned int>(x + i), static_cast<unsigned int>(y + j))) > this->tol) {
                return this->color;
            }
        }
    }
    for (int i = -1; i <= 1; i++) {
        if (this->im.getPixel(static_cast<unsigned int>(x),
                              static_cast<unsigned int>(y))->dist(*this->im.getPixel(
                static_cast<unsigned int>(x + 3 * i), static_cast<unsigned int>(y))) > this->tol) {
            return this->color;
        }
        if (this->im.getPixel(static_cast<unsigned int>(x),
                              static_cast<unsigned int>(y))->dist(*this->im.getPixel(
                static_cast<unsigned int>(x), static_cast<unsigned int>(y + 3*i))) > this->tol) {
            return this->color;
        }
    }
    return *this->im.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y));
}

