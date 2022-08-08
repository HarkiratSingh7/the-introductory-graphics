#include <cmath>
#include <WindowsScreen.h>
#include <string>
#include <Graphics.h>

using namespace std;

static Graphics graphics;

void lineDDA(int xa, int ya, int xb, int yb, const COLORREF &color)
{
    int dx = xb - xa, dy = yb - ya;
    float x = xa, y = ya;

    int steps = max(abs(dx), abs(dy));

    float xIncrement = dx / static_cast<float>(steps);
    float yIncrement = dy / static_cast<float>(steps);

    graphics.setPixel(ROUND(x), ROUND(y), color);
    
    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        graphics.setPixel(ROUND(x), ROUND(y), color);
    }
}

// The Painting Method
void Draw()
{
    lineDDA(100, 200, 400, 400, RGB(255, 134, 23));
    lineDDA(500, 590, 100, 400, RGB(90, 255, 23));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    // One Screen only, because it's single instance
    WindowsScreen theScreen(800, 600, string("My Application"), hInstance);
    graphics.setScreen(&theScreen);
    theScreen.Show(iCmdShow);
    
    return 0;
}