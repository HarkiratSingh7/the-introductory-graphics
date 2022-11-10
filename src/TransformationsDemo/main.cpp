#include "graphics2d.h"

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
    GraphicsWindow window(WIDTH, HEIGHT, "Transformations Demonstrations");
    ColorRGB blue(0, 0, 255);
    ColorRGB green(0, 255, 0);

    std::vector<Point2D> points = {{100, 200}, {400, 200}, {250, 500}};
    int refX = 250;
    int refY = 500;

    Figure2DSimple triangle(points, false);
    
    window.setColor(green);
    window.drawFigureSimple(triangle);

    window.clearTransformations();
    window.addScaling2D(0.5f, 0.5f, refX, refY);
    window.addRotation2D(-45.0f, refX, refY);
    window.addTranslation2D(50, -50);
    window.applyTransformations(points);

    window.setColor(blue);
    Figure2DSimple transformedTriangle(points, false);
    window.drawFigureSimple(transformedTriangle);

    std::vector<Point2D> polygon_pts = {{520, 50}, {630, 150}, {700, 270}, {450, 320}};
    Figure2DSimple polygon(polygon_pts, false);
    window.setColor(green);
    window.drawFigureSimple(polygon);

    window.clearTransformations();
    window.addRotation2D(90, 450, 320);
    window.applyTransformations(polygon_pts);
    Figure2DSimple transformedPolygon(polygon_pts, false);
    window.setColor(blue);
    window.drawFigureSimple(transformedPolygon);

    window.ShowOnce();

    return 0;
}