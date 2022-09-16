#include <Graphics.h>

using namespace std;

static Graphics graphics;

// The Painting Method
void Draw()
{
    graphics.midPointCircle(300, 300, 100, RGB(0, 0, 0));
    graphics.midPointCircle(300, 400, 100, RGB(0, 255, 0));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    // One Screen only, because it's single instance
    WindowsScreen theScreen(800, 600, LPCSTR("My Application"), hInstance);
    graphics.setScreen(&theScreen);
    theScreen.Show(iCmdShow);
    
    return 0;
}