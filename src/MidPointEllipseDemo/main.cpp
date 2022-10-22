#include "graphics2d.h"

const int WIDTH = 800;
const int HEIGHT = 600;

void ellipse_point(int x, int y, int cx, int cy, GraphicsWindow &window)
{
    window.plotpt(cx + x, cy + y);
    window.plotpt(cx - x, cy + y);
    window.plotpt(cx + x, cy - y);
    window.plotpt(cx - x, cy - y);
}

void draw_ellipse(int cx, int cy, int rx, int ry, ColorRGB color, GraphicsWindow &window)
{
    int rx2 = rx * rx;
    int ry2 = ry * ry;
    int _2rx2 = 2 * rx2;
    int _2ry2 = 2 * ry2;
    int x = 0;
    int y = ry;

    int p1 = ry2 + std::round(rx2 * 0.25) - (ry * rx2);

    window.setColor(color);

    ellipse_point(x, y, cx, cy, window);

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
        ellipse_point(x, y, cx, cy, window);
    }

    int p2 = (ry2 * std::round((x + 0.5) * (x + 0.5))) + (rx2 * (y - 1) * (y - 1)) - (ry2 * rx2);

    while (y > 0)
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
        ellipse_point(x, y, cx, cy, window);
    }
}

int main()
{
    GraphicsWindow window(WIDTH, HEIGHT, "Ellipse Mid Point Algorithm Demo");

    window.Draw([&]{
    draw_ellipse(200, 300, 100, 140, ColorRGB(.0f, .0f, .0f), window);   
    draw_ellipse(400, 200, 200, 140, ColorRGB(.0f, .0f, 1.0f), window); 
    });

    return 0;
}