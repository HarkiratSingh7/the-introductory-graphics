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
        glOrtho(0, width - 1, height - 1, 0, 0, 1);
    }

    void plotpt(int x, int y, const ColorRGB &color)
    {
        glBegin(GL_POINTS);
        glColor3f(color.red, color.green, color.blue);
        glVertex2i(x, y);
        glEnd();
    }

    void line(int xa, int ya, int xb, int yb)
    {
        glBegin(GL_LINES);
        glVertex2i(xa, ya);
        glVertex2i(xb, yb);
        glEnd();
    }

    void setColor(ColorRGB color)
    {
        glColor3f(color.red, color.green, color.blue);
    }

    void plotpt(int x, int y)
    {
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }

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
    std::ostream &log;
    GLFWwindow *window{nullptr};
    int window_width{0}, window_height{0};
    std::string window_title;
};

#endif // GRAPHICS2D_H