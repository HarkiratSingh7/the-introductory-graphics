/*
    The aim of this project is to implement the actual algorithms used by various graphical
    applications and libraries.

    I have been doing this project as a part of my studies to understand the matter.

    This header file aims to abstract all other additional processes so that it becomes easy to
    focus on the algorithms.
*/
#ifndef GRAPHICS2D_H
#define GRAPHICS2D_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cmath>
#include <functional>

const char ln = '\n';

struct ColorRGB
{
    float red = 0.0f, blue = 0.0f, green = 0.0f;
    ColorRGB() {}
    ColorRGB(float b_red, float b_green, float b_blue)
        : red(b_red), blue(b_blue), green(b_green)
    {
    }
};

class GraphicsWindow
{
public:
    GraphicsWindow(GraphicsWindow &window) = delete;
    GraphicsWindow(int width, int height, const std::string &title, std::ostream &outstream = std::cout)
        : window_title(title), window_width(width), window_height(height), log(outstream)
    {
        if (!glfwInit())
        {
            log << "An error occurred while initializing GLFW" << ln;
            return;
        }

        window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);
        if (!window)
        {
            log << "Failed to create a window" << ln;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
        {
            log << "Failed to initialize GLEW" << ln;
            glfwTerminate();
            return;
        }

        auto version_str = glGetString(GL_VERSION);
        log << "OpenGL Version " << version_str << ln;

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glOrtho(0, width - 1, height - 1, 0, 0, 1); // Origin at the top left
    }

    /// @brief plots a point
    /// @param x abscissa
    /// @param y ordinate
    /// @param color color of the point
    void plotpt(int x, int y, const ColorRGB &color)
    {
        glBegin(GL_POINTS);
        glColor3f(color.red, color.green, color.blue);
        glVertex2i(x, y);
        glEnd();
    }

    /// @brief Used to draw a line using opengl
    /// @param xa abscissa of first point
    /// @param ya ordinate of first point
    /// @param xb abscissa of second point
    /// @param yb ordinate of second point
    void line(int xa, int ya, int xb, int yb)
    {
        glBegin(GL_LINES);
        glVertex2i(xa, ya);
        glVertex2i(xb, yb);
        glEnd();
    }

    void circle(int cx, int cy, int r, ColorRGB color)
    {
        int x = 0;
        int y = r;
        int p = 1 - r;

        setColor(color);
        circle_point(x, y, cx, cy);

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

            circle_point(x, y, cx, cy);
        }
    }

    void ellipse(int cx, int cy, int rx, int ry, ColorRGB color)
    {
        int rx2 = rx * rx;
        int ry2 = ry * ry;
        int _2rx2 = 2 * rx2;
        int _2ry2 = 2 * ry2;
        int x = 0;
        int y = ry;

        int p1 = ry2 + std::round(rx2 * 0.25) - (ry * rx2);

        setColor(color);

        ellipse_point(x, y, cx, cy);

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
            ellipse_point(x, y, cx, cy);
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
            ellipse_point(x, y, cx, cy);
        }
    }

    void setColor(ColorRGB color)
    {
        glColor3f(color.red, color.green, color.blue);
    }

    /// @brief plots a point using already set color
    /// @param x abscissa
    /// @param y ordinate
    void plotpt(int x, int y)
    {
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }

    /// @brief A drawing function that will be repeatedly run while window is not closing
    /// @param drawingFunction drawing function
    void Draw(std::function<void()> drawingFunction)
    {

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            drawingFunction();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ~GraphicsWindow()
    {
        log << "GraphicsWindow: Exiting on out of scope" << ln;
        glfwTerminate(); // will delete window
    }

private:
    void circle_point(int x, int y, int cx, int cy)
    {
        plotpt(cx + x, cy + y);
        plotpt(cx - x, cy + y);
        plotpt(cx + x, cy - y);
        plotpt(cx - x, cy - y);
        plotpt(cx + y, cy + x);
        plotpt(cx - y, cy + x);
        plotpt(cx + y, cy - x);
        plotpt(cx - y, cy - x);
    }

    void ellipse_point(int x, int y, int cx, int cy)
    {
        plotpt(cx + x, cy + y);
        plotpt(cx - x, cy + y);
        plotpt(cx + x, cy - y);
        plotpt(cx - x, cy - y);
    }

private:
    std::ostream &log;
    GLFWwindow *window{nullptr};
    int window_width{0}, window_height{0};
    std::string window_title;
};

#endif // GRAPHICS2D_H