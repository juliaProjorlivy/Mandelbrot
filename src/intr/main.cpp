#include <immintrin.h>
#include <stdlib.h>
#include "raylib.h"
#include "VChange.h"
#include "Coloring.h"
#include "verror.h"


Color *IterateColors(Vector vect, Color *colors)
{
    __m256i DeltaIters = _mm256_setzero_si256();

    Vector Cs = vect; //spbctf, picoctf
    int Iter = 0;

    for(; Iter < MaxIter; Iter++)
    {
        __m256 XX = _mm256_mul_ps(vect.x, vect.x),
               YY = _mm256_mul_ps(vect.y, vect.y),
               XY = _mm256_mul_ps(vect.x, vect.y);

        __m256 cmp = _mm256_cmp_ps(R2, _mm256_add_ps(XX, YY), _CMP_LT_OS);
        int mask = _mm256_movemask_ps(cmp);
        if(mask == 0xff) break;
        DeltaIters = _mm256_sub_epi32(DeltaIters, _mm256_srli_epi32(_mm256_castps_si256(cmp), 31));
        
        vect.x = _mm256_add_ps(_mm256_sub_ps(XX, YY), Cs.x);
        vect.y = _mm256_add_ps(_mm256_add_ps(XY, XY), Cs.y);
    }

    return MakeColors(_mm256_add_epi32(_mm256_set1_epi32(Iter), DeltaIters), colors);

}

int main()
{
    Vector2 StartPoint = {-2, -1};
    float NewZoomCoef = 300.f;
    float OldZoomCoef = NewZoomCoef;
    Vector2 ZoomPoint = {};
    Vector2 ShiftVect = {};
    Color *colors = (Color *)calloc(sizeof(Color), 8);
    if(!colors)
    {
        VERROR_MEM;
        return 1;
    }

    InitWindow(ScreenWidth, ScreenHeight, "Mandelbrot");
    SetExitKey(KEY_S);
    while(!WindowShouldClose())
    {
        ShiftVect = {};
        if(IsKeyDown(KEY_L)) {ShiftVect.x = 1; StartPoint = ShiftStartPoint(StartPoint, ShiftVect, NewZoomCoef);}
        if(IsKeyDown(KEY_H)) {ShiftVect.x = -1; StartPoint = ShiftStartPoint(StartPoint, ShiftVect, NewZoomCoef);}
        if(IsKeyDown(KEY_K)) {ShiftVect.y = -1; StartPoint = ShiftStartPoint(StartPoint, ShiftVect, NewZoomCoef);}
        if(IsKeyDown(KEY_J)) {ShiftVect.y = 1; StartPoint = ShiftStartPoint(StartPoint, ShiftVect, NewZoomCoef);}
        if(IsKeyDown(KEY_EQUAL))
        {
            ZoomPoint = GetMousePosition();
            NewZoomCoef *= 2;
            StartPoint = ZoomStartPoint(StartPoint, OldZoomCoef, NewZoomCoef, ZoomPoint);
            OldZoomCoef = NewZoomCoef;
        }
        if(IsKeyDown(KEY_MINUS))
        {
            ZoomPoint = GetMousePosition();
            NewZoomCoef /= 2;
            StartPoint = ZoomStartPoint(StartPoint, OldZoomCoef, NewZoomCoef, ZoomPoint);
            OldZoomCoef = NewZoomCoef;
        }

        BeginDrawing();
        
            ClearBackground(WHITE);
            for(int y = 0; y < ScreenHeight; y++)
            {
                __m256 RawY = _mm256_set1_ps((float)y);
                __m256 Y = AdjustVector(RawY, StartPoint.y, NewZoomCoef);
                for(int x = 0; x < ScreenWidth; x += 8)
                {
                    __m256 RawX = _mm256_set1_ps((float)x);
                    __m256 X = AdjustVector(_mm256_add_ps(RawX, _70), StartPoint.x, NewZoomCoef);

                    colors = IterateColors({.x = X, .y = Y}, colors);
                    DrawPixels(x, y, colors);
                }
            }
        EndDrawing();
        printf("%lf\n", GetFrameTime());
    }
    CloseWindow();

    free(colors);
    return 0;
}
