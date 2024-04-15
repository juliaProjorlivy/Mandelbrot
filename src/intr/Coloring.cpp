#include <immintrin.h>
#include "raylib.h"
#include "Coloring.h"
#include "VChange.h"

Color MakeColor(int Iter)
{
    if(Iter == MaxIter)
    {
        return {0, 0, 0, 255}; //BLACK
    }

    Color color = {110, 25, 104, 255};
	int mask  = 0b000000000000010101110111;
	int shiftMag = Iter / 13;
    int newColor = mask << shiftMag;
    Color CoLoR = {(uchar)(((newColor >> 16) & 255) | color.r),
        (uchar)(((newColor >> 8) & 255) | color.g),
        (uchar)((newColor & 255) | color.b), 255};
    return CoLoR;
}

Color *MakeColors(__m256i _Iters, Color *colors)
{
    int *Iters = (int *) &_Iters;
    for(int i = 7; i >= 0; i--)
    {
        colors[i] = MakeColor(Iters[i]);
    }
    return colors;
}

