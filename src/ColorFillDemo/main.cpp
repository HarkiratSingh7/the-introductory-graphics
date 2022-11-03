#include "graphics2d.h"

const int width = 800;
const int height = 600;

#define position(x, y) ((y * width) + x)

void flood_fill4(int x, int y, ColorRGB fillColor, ColorRGB oldColor, GraphicsWindow &window)
{
    // Recursive version was causing crashes (segmentation fault) with large circles
    
    using namespace std;
    const vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    bitset<width * height> bits(0);
    queue<pair<int, int>> q;

    q.push({x, y});
    bits[position(x, y)] = 1;
    window.setColor(fillColor);

    while (!q.empty())
    {
        auto pos = q.front();
        q.pop();
        window.plotpt(pos.first, pos.second);

        for (const auto &direction : directions)
        {
            auto nx = direction.first + pos.first;
            auto ny = direction.second + pos.second;
            if (bits[position(nx, ny)])
                continue;
            auto clr = window.getColor(nx, ny);
            if (clr == oldColor)
            {
                q.push({nx, ny});
                bits[position(nx, ny)] = 1;
            }
        }
    }
}

int main()
{
    GraphicsWindow window(width, height, "Coloring algorithms");

    ColorRGB black(0, 0, 0);
    ColorRGB red(255, 0, 0);
    ColorRGB green(0, 255, 0);
    ColorRGB white(255, 255, 255);
    ColorRGB blue(100, 100, 255);

    window.Draw([&]
                {
                    window.circle(400, 300, 200, black);
                    window.circle(500, 500, 200, red);
                    window.circle(200, 200, 100, black);

                    flood_fill4(450, 450, green, white, window);
                    flood_fill4(250, 250, blue, white, window);
                });

    return 0;
}