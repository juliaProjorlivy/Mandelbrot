#include <immintrin.h>
#include "raylib.h"
#include "VChange.h"

__m256 AdjustVector(__m256 Raw, float StartPoint, float NewZoomCoef)
{
    __m256 _ZoomCoef = _mm256_set1_ps(NewZoomCoef);
    __m256 _StartPoint = _mm256_set1_ps(StartPoint);

    return _mm256_add_ps(_mm256_div_ps(Raw, _ZoomCoef), _StartPoint);
}

Vector2 ZoomStartPoint(Vector2 StartPoint, float OldZoomCoef, float NewZoomCoef, Vector2 ZoomPoint)
{
    StartPoint.x += ZoomPoint.x / OldZoomCoef - ScreenWidth / (2 * NewZoomCoef);
    StartPoint.y -= -ZoomPoint.y / OldZoomCoef + ScreenHeight / (2 * NewZoomCoef);

    return StartPoint;
}

Vector2 ShiftStartPoint(Vector2 StartPoint, Vector2 ShiftVect, float NewZoomCoef)
{
    StartPoint.x += ShiftVect.x * ScreenWidth / (5 * NewZoomCoef);
    StartPoint.y += ShiftVect.y * ScreenHeight / (5 * NewZoomCoef);

    return StartPoint;
}

void DrawPixels(int RawX, int RawY, Color *colors)
{
    for(int i = 7; i >=0 ; i--)
    {
        DrawPixel(RawX + i, RawY, colors[i]);
    }
}

