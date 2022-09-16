#include <Graphics.h>

using namespace std;

static Graphics graphics;

// The Painting Method
void Draw()
{
    graphics.midPointEllipse(200, 300, 100, 140, RGB(0, 0, 0));   
    graphics.midPointEllipse(400, 200, 200, 140, RGB(0, 0, 255));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    // One Screen only, because it's single instance
    WindowsScreen theScreen(800, 600, LPCSTR("My Application"), hInstance);
    graphics.setScreen(&theScreen);
    theScreen.Show(iCmdShow);
    
    return 0;
}