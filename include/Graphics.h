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


    void midPointCircle(int xc, int yc, int r, const COLORREF &boundary)
    {
        int x = 0;
        int y = r;
        int p = 1 - r;
        circlePointPoints(xc, yc, x, y, boundary);
        while (x < y)
        {
            x++;
            if (p < 0)
                p += 2 * (x) + 1;
            else
            {
                y--;
                p += 2 * (x - y) + 1;
            }
            circlePointPoints(xc, yc, x, y, boundary);
        }
    }

    void midPointEllipse(int xc, int yc, int rx, int ry, const COLORREF& boundary)
    {
        int rx2 = rx * rx;
        int ry2 = ry * ry;
        int _2rx2 = 2 * rx2;
        int _2ry2 = 2 * ry2;
        int x = 0;
        int y = ry;

        int p1 = ry2 + ROUND(rx2 * 0.25) - (ry * rx2);

        ellipsePointPoints(xc, yc, x, y, boundary);

        int px = 0;
        int py = _2rx2 * y;

        while (px < py)
        {
            px += _2ry2;
            ++x;
            if (p1 < 0)
                p1 += ry2 + px;
            else
            {
                y--;
                py -= _2rx2;
                p1 += ry2 + px - py;
            }
            ellipsePointPoints(xc, yc, x, y, boundary);
        }

        int p2 = (ry2 * ROUND((x + 0.5) * (x + 0.5))) + (rx2 * (y - 1) * (y - 1)) - (ry2 * rx2);

        while(y > 0) 
        {
            y--;
            py -= _2rx2;
            if (p2 > 0)
                p2 += rx2 - py;
            else
            {
                x++;
                px += _2ry2;
                p2 += rx2 - py + px;
            }
            ellipsePointPoints(xc, yc, x, y, boundary);
        }
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
    inline void ellipsePointPoints(int cx, int cy, int x, int y, const COLORREF& boundary)
    {
        SetPixel(hdc, cx + x, cy + y, boundary);
        SetPixel(hdc, cx - x, cy + y, boundary);
        SetPixel(hdc, cx + x, cy - y, boundary);
        SetPixel(hdc, cx - x, cy - y, boundary);
    }

    inline void circlePointPoints(int cx, int cy, int x, int y, const COLORREF& boundary)
    {
        SetPixel(hdc, cx + x, cy + y, boundary);
        SetPixel(hdc, cx + x, cy - y, boundary);
        SetPixel(hdc, cx - x, cy + y, boundary);
        SetPixel(hdc, cx - x, cy - y, boundary);
        SetPixel(hdc, cx + y, cy + x, boundary);
        SetPixel(hdc, cx + y, cy - x, boundary);
        SetPixel(hdc, cx - y, cy + x, boundary);
        SetPixel(hdc, cx - y, cy - x, boundary);
    }
private:
    HDC hdc{nullptr};
};

#endif