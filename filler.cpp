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
    vector<PVector> neighbours(){
        vector<PVector> nb;
        /**
         * UPRIGHT(+x,-y), UP(-y), UPLEFT(-x,-y), LEFT(-x), 
         * DOWNLEFT(-x,+y), DOWN(+y), DOWNRIGHT(+x,+y), RIGHT(+x)
         */
        if (y > 0) {
            nb.push_back(PVector(x+1,y-1));
            nb.push_back(PVector(x,y-1));
        }
        if (x > 0){ 
            nb.push_back(PVector(x-1, y-1));
            nb.push_back(PVector(x-1, y));
            nb.push_back(PVector(x-1, y+1));
        }
        nb.push_back(PVector(x , y + 1));
        nb.push_back(PVector(x+1 , y +1));
        nb.push_back(PVector(x+1 , y));
        return nb;
    }

    bool operator==(PVector other){
        return this->x == other.x and this->y == other.y;
    }

};

animation filler::fillStripeDFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker stripe(fillColor, stripeSpacing);
    return fill<Stack>(img, x, y, stripe, tolerance, frameFreq);
}

animation filler::fillBorderDFS(PNG& img, int x, int y,
                                    HSLAPixel borderColor, double tolerance, int frameFreq)
{
    borderColorPicker border(borderColor, img, tolerance, img.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y)));
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
    borderColorPicker border(borderColor, img, tolerance, img.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y)));
    return fill<Queue>(img, x, y, border, tolerance, frameFreq);
}

/* Given implementation of a BFS rainbow fill. */
animation filler::fillRainBFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a(freq);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}


//maybe im missing something but it seems like tolerance is useless here, should only be used by the color picker
template <template <class T> class OrderingStructure>
animation filler::fill(PNG& img, int x, int y, colorPicker& fillColor,
                       double tolerance, int frameFreq)
{
    animation anim;
    anim.addFrame(img);

    OrderingStructure<PVector> pos;

    unordered_set<PVector> visited;
    int resCheck = 0;
    visited.insert(PVector(x,y));
    pos.add(PVector(x,y));

    while(!pos.isEmpty()){

        PVector currPos = pos.remove();

        *img.getPixel(static_cast<unsigned int>(currPos.x), static_cast<unsigned int>(currPos.y)) = fillColor.operator()(currPos.x, currPos.y);
        resCheck++;
        if (resCheck == frameFreq){
            resCheck = 0;
            anim.addFrame(img);
        }

        for (PVector neighbour : currPos.neighbours()){
            //if operator returns the target color and he have not seen the pixel already
	    //the fill color check is only really useful for the border fill and will probably just slow down the other algorithms

            if(*img.getPixel(static_cast<unsigned int>(neighbour.x), static_cast<unsigned int>(neighbour.y)) == fillColor.operator()(neighbour.x, neighbour.y)
                        and visited.count(neighbour) == 0){
                visited.insert(neighbour);
                pos.add(neighbour);
            }
        }
    }
    return anim;
}
