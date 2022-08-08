#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "WindowsScreen.h"
#include <cmath>

#define ROUND(a) (static_cast<int>(a + 0.5))

class Graphics
{
public:
    Graphics(WindowsScreen *screen)
        : hdc(screen->hdc) {}

    Graphics() {}

    void setScreen(WindowsScreen *screen)
    {
        hdc = screen->hdc;
    }

    void setPixel(int x, int y, const COLORREF &color)
    {
        SetPixel(hdc, x, y, color);
    }

    void lineDDA(int xa, int ya, int xb, int yb, const COLORREF& color)
    {
        int dx = xa - xb;
        int dy = ya - yb;

        int steps = std::max(abs(dx), abs(dy));

        float xInc = static_cast<float>(dx) / steps;
        float yInc = static_cast<float>(dy) / steps;

        float x = xa;
        float y = ya;

        setPixel(ROUND(x), ROUND(y), color);
        for (int k = 0; k < steps; k++)
        {
            x += xInc;
            y += yInc;
            setPixel(ROUND(x), ROUND(y), color);
        }
    }

    void lineBresenham(int xa, int ya, int xb, int yb, const COLORREF& color)
    {
        int dx = abs(xa - xb);
        int dy = abs(ya - yb);

        int p = 2 * dy - dx; // decision parameter
        int _2dy = 2 * dy, _2dydx = 2 * (dy - dx);
        int x, y, xEnd;

        if (xa > xb)
        {
            x = xb;
            y = yb;
            xEnd = xa;
        }
        else
        {
            x = xa;
            y = ya;
            xEnd = xb;
        }

        setPixel(x, y, color);

        while(x < xEnd)
        {
            x++;
            if (p < 0)
                p += _2dy;
            else 
            {
                y++;
                p += _2dydx;
            }
            setPixel(x, y, color);
        }
    }

private:
    HDC hdc{nullptr};
};

#endif