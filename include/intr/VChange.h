#ifndef VCHANGE_H
#define VCHANGE_H

typedef struct
{
    __m256 x;
    __m256 y;
} Vector;

typedef unsigned char uchar;

const Color BlackColor = {0, 0, 0, 255};
const float zoomCoef = 2.0;
const int ScreenWidth = 900;
const int ScreenHeight = 600;
const int DeltaShift = 50;
const __m256 R2 = _mm256_set1_ps(4.f);
const int MaxIter = 255;
const __m256 _MaxIter = _mm256_set1_ps(255.f);
const __m256 _70 = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);


__m256 AdjustVector(__m256 Raw, float StartPoint, float NewZoomCoef);

Vector2 ZoomStartPoint(Vector2 StartPoint, float OldZoomCoef, float NewZoomCoef, Vector2 ZoomPoint);

Vector2 ShiftStartPoint(Vector2 StartPoint, Vector2 ShiftVect, float NewZoomCoef);

void DrawPixels(int RawX, int RawY, Color *colors);

#endif 

