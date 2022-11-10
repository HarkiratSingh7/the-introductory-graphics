#include "graphics2d.h"

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
    GraphicsWindow window(WIDTH, HEIGHT, "Transformations Demonstrations");
    ColorRGB blue(0, 0, 255);
    ColorRGB green(0, 255, 0);

    std::vector<std::pair<int, int>> points = {{100, 200}, {400, 200}, {250, 500}};
    int refX = 250;
    int refY = 500;

    auto draw = [&]()
    {
        window.lineDDA(points[0].first, points[0].second, points[1].first, points[1].second);
        window.lineDDA(points[1].first, points[1].second, points[2].first, points[2].second);
        window.lineDDA(points[2].first, points[2].second, points[0].first, points[0].second);
    };

    window.Run([&]
               {
                   window.setColor(green);
                   draw();

                   window.clearTransformations();
                   window.addScaling2D(0.5f, 0.5f, refX, refY);
                   window.addRotation2D(-45.0f, refX, refY);
                   window.addTranslation2D(50, -50);
                   window.applyTransformations(points);

                   window.setColor(blue);
                   draw(); 
                });

    return 0;
}