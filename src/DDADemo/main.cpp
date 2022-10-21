#include "graphics2d.h"

const int WIDTH = 800;
const int HEIGHT = 600;

/// @brief This is the digital differential analyzer algorithm (a scan-conversion algorithm).
/// @param x1 abscissa of first point
/// @param y1 ordinate of first point
/// @param x2 abscissa of second point
/// @param y2 ordinate of second point
/// @param color color
/// @param window the window instance to which method should apply
void lineDDA(int x1, int y1, int x2, int y2, ColorRGB color, GraphicsWindow& window)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    int steps = std::max(abs(dx), abs(dy));

    int x = x1, y = y1;
    float xi = dx / static_cast<float>(steps);
    float yi = dy / static_cast<float>(steps);

    window.setColor(color);
    window.plotpt(x, y);
    for (int k = 0; k < steps; k++)
    {
        x += xi;
        y += yi;
        window.plotpt(x, y);
    }
}

int main()
{
    GraphicsWindow window(WIDTH, HEIGHT, "DDA Demo");

    window.Draw([&]{
        auto green = ColorRGB(0.0f, 1.0f, 0.0f);
        auto blue = ColorRGB(0.0f, 0.0f, 1.0f);
        auto red = ColorRGB(1.0f, 0.0f, 0.0f);

        lineDDA(100, 100, 500, 500, blue, window);
        lineDDA(400, 100, 100, 700, green, window);
        lineDDA(100, 400, 600, 400, red, window);

        auto black = ColorRGB();
        window.setColor(black);
        window.line(WIDTH - 1, 0, 0, HEIGHT - 1); // diagonal
    });

    return 0;
}