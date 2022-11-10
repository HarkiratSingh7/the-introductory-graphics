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
#include <vector>
#include <string>
#include <cmath>
#include <functional>
#include <queue>

const char ln = '\n';

typedef float Matrix3x3[3][3];
#define ROUND(a) (static_cast<int>(a + 0.5))

struct ColorRGB
{
    GLbyte red = 0, green = 0, blue = 0;
    ColorRGB() {}
    ColorRGB(int b_red, int b_green, int b_blue)
        : red(b_red), blue(b_blue), green(b_green)
    {
    }

    ColorRGB(float f_red, float f_green, float f_blue)
    {
        red = static_cast<GLbyte>(255 * f_red);
        green = static_cast<GLbyte>(255 * f_green);
        blue = static_cast<GLbyte>(255 * f_blue);
    }

    bool operator==(const ColorRGB &color)
    {
        if (color.blue == this->blue and color.green == this->green and color.red == this->red)
            return true;
        return false;
    }
};

struct Point2D
{
    int x = 0;
    int y = 0;
    ColorRGB value;
};

struct Figure2DSimple
{
    Figure2DSimple(bool isOpen = false)
        : open(isOpen) {}

    /// @brief Creates a figure by connecting all points in order
    /// @param points The points to be connected one by one.
    /// @param isOpen The last point should be connected to first or not.
    Figure2DSimple(std::vector<Point2D> &points, bool isOpen)
    {
        this->points = points;
        this->open = isOpen;
    }

    void addPoint(Point2D pt)
    {
        points.push_back(pt);
    }

private:
    std::vector<Point2D> points;
    bool open = false;
    friend class GraphicsWindow;
};

struct Figure2DGraph
{
    void add_link(Point2D first, Point2D second)
    {
        Figure2DSimple line(true);
        line.addPoint(first);
        line.addPoint(second);
    }

private:
    std::vector<Figure2DSimple> lines;
    friend class GraphicsWindow;
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
#ifdef GRAPHICS_VERBOSE
            log << "An error occurred while initializing GLFW" << ln;
#endif
            return;
        }

        window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);
        pixels = new ColorRGB[window_height * window_width];

        for (int i = 0; i < window_width; i++)
        {
            for (int j = 0; j < window_height; j++)
                pixels[(j * window_width) + i] = ColorRGB(255, 255, 255);
        }

        if (!window)
        {
#ifdef GRAPHICS_VERBOSE
            log << "Failed to create a window" << ln;
#endif
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
        {
#ifdef GRAPHICS_VERBOSE
            log << "Failed to initialize GLEW" << ln;
#endif
            glfwTerminate();
            return;
        }

        auto version_str = glGetString(GL_VERSION);
#ifdef GRAPHICS_VERBOSE
        log << "OpenGL Version " << version_str << ln;
#endif
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width - 1, height - 1, 0, 0, 1); // Origin at the top left

        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, window_width, window_height);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glEnable(GL_TEXTURE_2D);

        clearTransformations();
    }

    inline void setColor(ColorRGB color)
    {
        selectedColor = color;
    }

    ColorRGB getColor(int x, int y)
    {
        if (x < 0 or x >= window_width or y < 0 or y >= window_height)
            return ColorRGB();
        return pixels[(y * window_width) + x];
    }

    /// @brief plots a point using already set color
    /// @param x abscissa
    /// @param y ordinate
    void plotpt(int x, int y, std::vector<Point2D> *pts = nullptr)
    {
        if (pts)
            pts->push_back({x, y, selectedColor});
        if (x < 0 or x >= window_width or y < 0 or y >= window_height)
            return;
        pixels[(y * window_width) + x] = selectedColor;
    }

    /// @brief A drawing function that will run only once and then nothing will be drawn to screen. (It will persist).
    /// @param drawingfunction drawing function
    void ShowOnce(std::function<void()> drawingfunction = nullptr)
    {
        if (drawingfunction)
            drawingfunction();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glBegin(GL_QUADS);
            glTexCoord2d(0.0, 0.0);
            glVertex2d(0.0, 0.0);
            glTexCoord2d(1.0, 0.0);
            glVertex2d(window_width, 0.0);
            glTexCoord2d(1.0, 1.0);
            glVertex2d(window_width, window_height);
            glTexCoord2d(0.0, 1.0);
            glVertex2d(0.0, window_height);
            glEnd();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    /// @brief A drawing function that will be repeatedly run while window is not closing
    /// @param drawingFunction drawing function
    void Draw(std::function<void()> drawingFunction)
    {

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            drawingFunction();
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            glBegin(GL_QUADS);
            glTexCoord2d(0.0, 0.0);
            glVertex2d(0.0, 0.0);
            glTexCoord2d(1.0, 0.0);
            glVertex2d(window_width, 0.0);
            glTexCoord2d(1.0, 1.0);
            glVertex2d(window_width, window_height);
            glTexCoord2d(0.0, 1.0);
            glVertex2d(0.0, window_height);
            glEnd();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ~GraphicsWindow()
    {
        delete[] pixels;
#ifdef GRAPHICS_VERBOSE
        log << "GraphicsWindow: Exiting on out of scope" << ln;
#endif
        glfwTerminate(); // will delete window
    }

    // Drawing functions

    /// @brief Used to draw a line using opengl
    /// @param x1 abscissa of first point
    /// @param y1 ordinate of first point
    /// @param x2 abscissa of second point
    /// @param y2 ordinate of second point
    void lineDDA(int x1, int y1, int x2, int y2, std::vector<Point2D> *pts = nullptr)
    {
        int dx = x2 - x1;
        int dy = y2 - y1;

        int steps = std::max(abs(dx), abs(dy));

        float x = x1, y = y1;
        float xi = dx / static_cast<float>(steps);
        float yi = dy / static_cast<float>(steps);

        plotpt(x, y, pts);
        for (int k = 0; k < steps; k++)
        {
            x += xi;
            y += yi;
            plotpt(ROUND(x), ROUND(y));
        }
    }

    void drawFigureGraph(const Figure2DGraph &figure)
    {
        for (const auto &line : figure.lines)
            lineDDA(line.points.front().x, line.points.front().y, line.points.back().x, line.points.back().y);
    }

    void drawFigureSimple(const Figure2DSimple &figure)
    {
        for (int i = 0; i < figure.points.size() - 1; i++)
            lineDDA(figure.points[i].x, figure.points[i].y, figure.points[i + 1].x, figure.points[i + 1].y);

        if (!figure.open)
            lineDDA(figure.points.back().x, figure.points.back().y, figure.points.front().x, figure.points.front().y);
    }

    void circle(int cx, int cy, int r, ColorRGB color, std::vector<Point2D> *pts = nullptr)
    {
        int x = 0;
        int y = r;
        int p = 1 - r;

        setColor(color);
        circle_point(x, y, cx, cy, pts);

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

            circle_point(x, y, cx, cy, pts);
        }
    }

    void ellipse(int cx, int cy, int rx, int ry, ColorRGB color, std::vector<Point2D> *pts = nullptr)
    {
        int rx2 = rx * rx;
        int ry2 = ry * ry;
        int _2rx2 = 2 * rx2;
        int _2ry2 = 2 * ry2;
        int x = 0;
        int y = ry;

        int p1 = ry2 + std::round(rx2 * 0.25) - (ry * rx2);

        setColor(color);

        ellipse_point(x, y, cx, cy, pts);

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
            ellipse_point(x, y, cx, cy, pts);
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
            ellipse_point(x, y, cx, cy, pts);
        }
    }

    const int getSize() { return window_height * window_width; }

    void flood_fill4(int x, int y, ColorRGB fillColor, ColorRGB oldColor)
    {
        // Recursive version was causing crashes (segmentation fault) with large circles

        using namespace std;
        const vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

        queue<pair<int, int>> q;

        setColor(fillColor);
        q.push({x, y});
        plotpt(x, y);

        while (!q.empty())
        {
            auto pos = q.front();
            q.pop();

            for (const auto &direction : directions)
            {
                auto nx = direction.first + pos.first;
                auto ny = direction.second + pos.second;
                if (nx < 0 or nx >= window_width or ny < 0 or ny >= window_height)
                    continue;
                auto clr = getColor(nx, ny);
                if (clr == oldColor)
                {
                    q.push({nx, ny});
                    plotpt(nx, ny);
                }
            }
        }
    }

    /// @brief Apply all added transformations
    /// @param points vector containing all points (pair object having first as abscissa, second as ordinate)
    /// @param clear whether to clear added transformations or not
    void applyTransformations(std::vector<Point2D> &points, bool clear = true)
    {
        for (auto &point : points)
        {
            float temp = transformations[0][0] * point.x + transformations[0][1] * point.y + transformations[0][2];
            point.y = transformations[1][0] * point.x + transformations[1][1] * point.y + transformations[1][2];
            point.x = temp;
        }

        if (clear)
            clearTransformations();
    }

    /// @brief Clears added transformations
    void clearTransformations()
    {
        setIdentity3x3(transformations);
    }

    /// @brief Add a translation operation in order
    /// @param tx translation distance along x axis
    /// @param ty translation distance along y axis
    void addTranslation2D(int tx, int ty)
    {
        Matrix3x3 translations;
        setIdentity3x3(translations);
        translations[0][2] = tx;
        translations[1][2] = ty;
        matrix3x3Multiply(translations, transformations);
    }

    /// @brief Add a scaling operation in order
    /// @param sx scaling factor along x axis
    /// @param sy scaling factor along y axis
    /// @param fX fixed point Abscissa
    /// @param fY fixed point ordinate
    void addScaling2D(float sx, float sy, int fX, int fY)
    {
        Matrix3x3 scaling;
        setIdentity3x3(scaling);
        scaling[0][0] = sx;
        scaling[1][1] = sy;
        scaling[0][2] = (1 - sx) * fX;
        scaling[1][2] = (1 - sy) * fY;
        matrix3x3Multiply(scaling, transformations);
    }

    /// @brief Rotate a point counter clockwise
    /// @param angle Angle in degrees. Positive value for counter clockwise rotation, negative value for clockwise rotation.
    /// @param pX Pivot point abscissa
    /// @param pY Pivot point ordinate
    void addRotation2D(float angle, int pX, int pY)
    {
        angle = degToRad(angle);
        Matrix3x3 rotations;
        setIdentity3x3(rotations);

        rotations[0][0] = cosf(angle);
        rotations[0][1] = -sinf(angle);
        rotations[1][0] = sinf(angle);
        rotations[1][1] = cosf(angle);

        rotations[0][2] = pX * (1 - cosf(angle)) + pY * sinf(angle);
        rotations[1][2] = pY * (1 - cosf(angle)) - pX * sinf(angle);

        matrix3x3Multiply(rotations, transformations);
    }

private:
    inline float degToRad(float degrees)
    {
        return degrees * M_PI / 180.0f;
    }

    inline void setIdentity3x3(Matrix3x3 mat)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                mat[i][j] = (i == j);
    }

    void matrix3x3Multiply(Matrix3x3 a, Matrix3x3 result)
    {
        Matrix3x3 tmp;

        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                tmp[r][c] = a[r][0] * result[0][c] + a[r][1] * result[1][c] + a[r][2] * result[2][c];

        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                result[r][c] = tmp[r][c];
    }

    void circle_point(int x, int y, int cx, int cy, std::vector<Point2D> *pts = nullptr)
    {
        plotpt(cx + x, cy + y, pts);
        plotpt(cx - x, cy + y, pts);
        plotpt(cx + x, cy - y, pts);
        plotpt(cx - x, cy - y, pts);
        plotpt(cx + y, cy + x, pts);
        plotpt(cx - y, cy + x, pts);
        plotpt(cx + y, cy - x, pts);
        plotpt(cx - y, cy - x, pts);
    }

    void ellipse_point(int x, int y, int cx, int cy, std::vector<Point2D> *pts = nullptr)
    {
        plotpt(cx + x, cy + y, pts);
        plotpt(cx - x, cy + y, pts);
        plotpt(cx + x, cy - y, pts);
        plotpt(cx - x, cy - y, pts);
    }

private:
    Matrix3x3 transformations;
    std::ostream &log;
    ColorRGB *pixels;
    ColorRGB selectedColor;
    GLFWwindow *window{nullptr};
    int window_width{0}, window_height{0};
    std::string window_title;
};

#endif // GRAPHICS2D_H