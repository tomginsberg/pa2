/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace. 
 *
 */

#include "filler.h"
#include <unordered_set>
using namespace std;


/**
 * Simple vector datatype to help with image traversal
 */
struct PVector{
    int x;
    int y;
    PVector(int x, int y) : x{x}, y{y}{};
    vector<PVector> neighbours(int width, int height){
        vector<PVector> nb;
        /**
         * UPRIGHT(+x,-y), UP(-y), UPLEFT(-x,-y), LEFT(-x), 
         * DOWNLEFT(-x,+y), DOWN(+y), DOWNRIGHT(+x,+y), RIGHT(+x)
         */
        if (y > 0) {
            if (x < width - 1) {
                nb.emplace_back(x + 1, y - 1);
            }
            nb.emplace_back(x,y-1);
        }
        if (x > 0){
            if (y > 0) {
                nb.emplace_back(x - 1, y - 1);
            }
            nb.emplace_back(x-1, y);
            if(y < height - 1) {
                nb.emplace_back(x-1, y+1);
            }
        }
        if(y < height - 1) {
            nb.emplace_back(x, y + 1);
        }
        if (x < width - 1 and y < height - 1) {
            nb.emplace_back(x + 1, y + 1);
        }
        if (x < width - 1) {
            nb.emplace_back(x + 1, y);
        }
        return nb;
    }

    bool operator==(const PVector other) const {
        return this->x == other.x and this->y == other.y;
    }

};
struct PVectorHasher
{
    size_t
    operator()(const PVector & vec) const
    {
        return std::hash<std::string>()(to_string(vec.x)+to_string(vec.y));
    }
};


namespace std
{
    template<>
    struct hash<PVector>
    {
        size_t
        operator()(const PVector & vec) const
        {
            return std::hash<std::string>()(to_string(vec.x)+to_string(vec.y));
        }
    };
}

animation filler::fillStripeDFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker stripe(fillColor, stripeSpacing);
    return fill<Stack>(img, x, y, stripe, tolerance, frameFreq);
}

animation filler::fillBorderDFS(PNG& img, int x, int y,
                                    HSLAPixel borderColor, double tolerance, int frameFreq)
{
    borderColorPicker border(borderColor, img, tolerance, *img.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y)));
    return fill<Stack>(img, x, y, border, tolerance, frameFreq);
}


/* Given implementation of a DFS rainbow fill. */
animation filler::fillRainDFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a(freq);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillStripeBFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker stripe(fillColor, stripeSpacing);
    return fill<Queue>(img, x, y, stripe, tolerance, frameFreq);
}

animation filler::fillBorderBFS(PNG& img, int x, int y,
                                    HSLAPixel borderColor, double tolerance, int frameFreq)
{
    borderColorPicker border(borderColor, img, tolerance, *img.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y)));
    return fill<Queue>(img, x, y, border, tolerance, frameFreq);
}

/* Given implementation of a BFS rainbow fill. */
animation filler::fillRainBFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a(freq);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillCustomDFS(PNG& img, int x, int y, int transition,
                                HSLAPixel fillColor, double tolerance, int frameFreq)
{
    customColorPicker a(fillColor, transition);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillCustomBFS(PNG& img, int x, int y, int transition,
                                HSLAPixel fillColor, double tolerance, int frameFreq)
{
    customColorPicker a(fillColor, transition);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

void pixelModAdd(int& x, int& y, colorPicker& fillColor, int& resCheck, int& frameFreq, animation& anim, PNG& img){
    HSLAPixel fill = fillColor.operator()(x, y);
    HSLAPixel* curr = img.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y));
    if(fill != *curr) {
        resCheck++;
        *curr = fill;
    }
    if(resCheck == frameFreq){
        resCheck = 0;
        anim.addFrame(img);
    }
}

template <template <class T> class OrderingStructure>
animation filler::fill(PNG& img, int x, int y, colorPicker& fillColor,
                       double tolerance, int frameFreq)
{
    animation anim;
    OrderingStructure<PVector> pos;
    HSLAPixel center = *img.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y));
    unordered_set<PVector> visited;
    int resCheck = 0;
    visited.insert(PVector(x,y));
    pos.add(PVector(x,y));

    //Realized I wasn't adding the first pixel so cpy pasting was a quick fix
    pixelModAdd(x, y, fillColor, resCheck, frameFreq, anim, img);

    while(!pos.isEmpty()){

        PVector currPos = pos.remove();

        int width = img.width();
        int height = img.height();
        for (PVector neighbour : currPos.neighbours(width, height)){
            if(visited.count(neighbour) == 0 and
                    (*img.getPixel(static_cast<unsigned int>(neighbour.x), static_cast<unsigned int>(neighbour.y))).dist(center) < tolerance){
                visited.insert(neighbour);
                pos.add(neighbour);
                pixelModAdd(neighbour.x, neighbour.y, fillColor, resCheck, frameFreq, anim, img);
            }
        }
    }
    anim.addFrame(img);
    return anim;

    /*
 *
 * This is the basic description of a flood-fill algorithm: Every fill
 * algorithm requires an ordering structure, which is passed to this
 * function via its template parameter. For a breadth-first-search
 * fill, that structure is a Queue, for a depth-first-search, that
 * structure is a Stack. To begin the algorithm, you simply place the
 * given point in the ordering structure, marking it processed
 * (the way you mark it is a design decision you'll make yourself).
 * We have a choice to either change the color, if appropriate, when we
 * add the point to the OS, or when we take it off. In our test cases,
 * we have assumed that you will change the color when a point is added
 * to the structure.
 *
 * Until the structure is empty, you do the following:
 *
 * 1.     Remove a point from the ordering structure, and then...
 *
 *        1.    add its unprocessed neighbors whose color values are
 *              within (or equal to) tolerance distance from the center,
 *              to the ordering structure.
 *        2.    use the colorPicker to set the new color of the point.
 *        3.    mark the point as processed.
 *        4.    if it is an appropriate frame, send the current PNG to the
 *              animation (as described below).
 *
 * 2.     When implementing your breadth-first-search and
 *        depth-first-search fills, you will need to explore neighboring
 *        pixels in some order.
 *
 *        For this assignment, each pixel p has 8 neighbors, consisting of
 *        the 8 pixels who share an edge or corner with p. (We leave it to
 *        you to describe those 8 pixel locations, relative to the location
 *        of p.)
 *
 *        While the order in which you examine neighbors does not matter
 *        for a proper fill, you must use the same order as we do for
 *        your animations to come out like ours!
 *
 *        The order you should put
 *        neighboring pixels **ONTO** the ordering structure (queue or stack)
 *        is as follows: **UPRIGHT(+x,-y), UP(-y), UPLEFT(-x,-y), LEFT(-x),
 *        DOWNLEFT(-x,+y), DOWN(+y), DOWNRIGHT(+x,+y), RIGHT(+x)**
 *
 *        If you do them in a different order, your fill may
 *        still work correctly, but your animations will be different
 *        from the grading scripts!
 *
 *        IMPORTANT NOTE: *UP*
 *        here means towards the top of the image, so since an image has
 *        smaller y coordinates at the top, this is in the *negative y*
 *        direction. Similarly, *DOWN* means in the *positive y*
 *        direction.
 *
 * 3.     For every k pixels filled, **starting at the kth pixel**, you
 *        must add a frame to the animation, where k = frameFreq.
 *
 *        For example, if frameFreq is 4, then after the 4th pixel has
 *        been filled you should add a frame to the animation, then again
 *        after the 8th pixel, etc.  You must only add frames for the
 *        number of pixels that have been filled, not the number that
 *        have been checked. So if frameFreq is set to 1, a pixel should
 *        be filled every frame.
 * 4.     Finally, as you leave the function, send one last frame to the
 *        animation. This frame will be the final result of the fill, and
 *        it will be the one we test against.
 */
}
