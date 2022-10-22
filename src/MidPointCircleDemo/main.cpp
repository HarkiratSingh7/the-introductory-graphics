#include "graphics2d.h"

const int WIDTH = 800;
const int HEIGHT = 600;

void circle_point(int x, int y, int cx, int cy, GraphicsWindow &window)
{
    window.plotpt(cx + x, cy + y);
    window.plotpt(cx - x, cy + y);
    window.plotpt(cx + x, cy - y);
    window.plotpt(cx - x, cy - y);
    window.plotpt(cx + y, cy + x);
    window.plotpt(cx - y, cy + x);
    window.plotpt(cx + y, cy - x);
    window.plotpt(cx - y, cy - x);
}

void draw_circle(int cx, int cy, int r, ColorRGB color, GraphicsWindow &window)
{
    int x = 0;
    int y = r;
    int p = 1 - r;

    window.setColor(color);
    circle_point(x, y, cx, cy, window);

    while (x < y)
    {
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }

        circle_point(x, y, cx, cy, window);
    }
}

int main()
{
    GraphicsWindow window(WIDTH, HEIGHT, "Mid Point Circle Demonstration");

    window.Draw([&] {
        ColorRGB black;
        ColorRGB red(1.0f, 0.0f, 0.0f);
        ColorRGB blue(0.0f, 0.0f, 1.0f);
        ColorRGB yellow(0.85f, 0.85f, 0.0f);

        draw_circle(300, 300, 50, blue, window);
        draw_circle(400, 400, 200, red, window);
        draw_circle(400, 500, 100, yellow, window);
        draw_circle(400, 300, 299, black, window);
    });

    return 0;
}