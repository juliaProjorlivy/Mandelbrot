#include "/home/julia/projects/raylib-5.0_linux_amd64/include/raylib.h"

typedef unsigned char uchar;

const float zoomCoef = 2.0;
const int ScreenWidth = 900;
const int ScreenHeight = 600;
const int DeltaShift = 50;
const float R2 = 4.f;
const int MaxIter = 255;

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
    Color CoLoR = {(uchar)((newColor >> 16) & 255 | color.r),
        (uchar)((newColor >> 8) & 255 || color.g),
        (uchar)((newColor & 255) | color.b), 255};
    return CoLoR;
}

Vector2 AdjustVector(Vector2 StartPoint, Vector2 ZoomPoint, Vector2 RawVect, Vector2 NewZoomCoef)
{
    Vector2 vect = {};
    vect.x = RawVect.x / NewZoomCoef.x + StartPoint.x;
    vect.y = RawVect.y / NewZoomCoef.y + StartPoint.y;

    return vect;
}


Color IterateColor(Vector2 vect)
{
    int Iter = 0;
    Vector2 C = vect;
    for(; Iter < MaxIter; Iter++)
    {
        float xx = vect.x * vect.x,
        yy = vect.y * vect.y,
        xy = vect.x * vect.y;
        if(xx + yy  > R2) break;
        vect = {xx - yy + C.x, 2 * xy + C.y};
    }
    return MakeColor(Iter);
}

Vector2 ZoomStartPoint(Vector2 StartPoint, Vector2 OldZoomCoef, Vector2 NewZoomCoef, Vector2 ZoomPoint)
{
    StartPoint.x += ZoomPoint.x / OldZoomCoef.x - ScreenWidth / (2 * NewZoomCoef.x);
    StartPoint.y -= -ZoomPoint.y / OldZoomCoef.y + ScreenHeight / (2 * NewZoomCoef.y);

    return StartPoint;
}
Vector2 ShiftStartPoint(Vector2 StartPoint, Vector2 ShiftVect, Vector2 NewZoomCoef)
{
    StartPoint.x += ShiftVect.x * ScreenWidth / (5 * NewZoomCoef.x);
    StartPoint.y += ShiftVect.y * ScreenHeight / (5 * NewZoomCoef.y);

    return StartPoint;
}

int main()
{
    Vector2 StartPoint = {-2, -1};
    Vector2 NewZoomCoef = {300, 300};
    Vector2 OldZoomCoef = NewZoomCoef;
    Vector2 ZoomPoint = {};
    Vector2 ShiftVect = {};

    InitWindow(ScreenWidth, ScreenHeight, "Mandelbrot");
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
            NewZoomCoef.x *= 2;
            NewZoomCoef.y *= 2;
            StartPoint = ZoomStartPoint(StartPoint, OldZoomCoef, NewZoomCoef, ZoomPoint);
            OldZoomCoef = NewZoomCoef;
        }
        if(IsKeyDown(KEY_MINUS))
        {
            ZoomPoint = GetMousePosition();
            NewZoomCoef.x /= 2;
            NewZoomCoef.y /= 2;
            StartPoint = ZoomStartPoint(StartPoint, OldZoomCoef, NewZoomCoef, ZoomPoint);
            OldZoomCoef = NewZoomCoef;
        }

        BeginDrawing();
            ClearBackground(WHITE);
            for(int y = 0; y < ScreenHeight; y++)
            {
                for(int x = 0; x < ScreenWidth; x++)
                {
                    Vector2 RawVect = {(float)x, (float)y};
                    Vector2 vect = AdjustVector(StartPoint, ZoomPoint, RawVect, NewZoomCoef);
                    Color color = IterateColor(vect);
                    DrawPixelV(RawVect, color);
                }
            }
        EndDrawing();
    }
}
