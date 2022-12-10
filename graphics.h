#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "Frame2D.h"
#include <vector>

class Color {
    public:
        int r;
        int g;
        int b;
        int a;

        Color(int r, int g, int b, int a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        Color(int r, int g, int b) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = 255;
        }

        Color() {
            this->r = 0;
            this->g = 0;
            this->b = 0;
            this->a = 255;
        }
    
    static Color black() {
        return Color(0, 0, 0);
    }

    static Color darkGray() {
        return Color(64, 64, 64);
    }

    static Color gray() {
        return Color(128, 128, 128);
    }

    static Color lightGray() {
        return Color(192, 192, 192);
    }

    static Color white() {
        return Color(255, 255, 255);
    }

    static Color red() {
        return Color(255, 0, 0);
    }

    static Color green() {
        return Color(0, 255, 0);
    }

    static Color blue() {
        return Color(0, 0, 255);
    }

    static Color cyan() {
        return Color(0, 255, 255);
    }

    static Color magenta() {
        return Color(255, 0, 255);
    }

    static Color yellow() {
        return Color(255, 255, 0);
    }

    static Color orange() {
        return Color(255, 165, 0);
    }

    static Color purple() {
        return Color(128, 0, 128);
    }

    static Color random() {
        return Color(rand() % 256, rand() % 256, rand() % 256);
    }
};

namespace draw {

void setColor(SDL_Renderer *renderer, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void line(SDL_Renderer *renderer, Vector2D start, Vector2D end) {
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void circle(SDL_Renderer *renderer, Vector2D center, float radius) {
    int x = radius;
    int y = 0;
    int err = 0;
    while (x >= y) {
        SDL_RenderDrawPoint(renderer, center.x + x, center.y + y);
        SDL_RenderDrawPoint(renderer, center.x + y, center.y + x);
        SDL_RenderDrawPoint(renderer, center.x - y, center.y + x);
        SDL_RenderDrawPoint(renderer, center.x - x, center.y + y);
        SDL_RenderDrawPoint(renderer, center.x - x, center.y - y);
        SDL_RenderDrawPoint(renderer, center.x - y, center.y - x);
        SDL_RenderDrawPoint(renderer, center.x + y, center.y - x);
        SDL_RenderDrawPoint(renderer, center.x + x, center.y - y);
        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void arrow(SDL_Renderer *renderer, Vector2D start, Vector2D end) {
    line(renderer, start, end);
    Vector2D dir = end - start;
    dir /= dir.magnitude();
    Vector2D perp = dir.perpendicular();
    Vector2D arrow1 = end - dir * 10 + perp * 5;
    Vector2D arrow2 = end - dir * 10 - perp * 5;
    line(renderer, end, arrow1);
    line(renderer, end, arrow2);
}

void cross(SDL_Renderer *renderer, Vector2D center, float radius) {
    line(renderer, center + Vector2D(-radius, -radius), center + Vector2D(radius, radius));
    line(renderer, center + Vector2D(-radius, radius), center + Vector2D(radius, -radius));
}

void rect(SDL_Renderer *renderer, Vector2D topleft, Vector2D bottomright) {
    SDL_Rect rect;
    rect.x = topleft.x;
    rect.y = topleft.y;
    rect.w = bottomright.x - topleft.x;
    rect.h = bottomright.y - topleft.y;
    SDL_RenderDrawRect(renderer, &rect);
}

void clearScreen(SDL_Renderer *renderer, Color color) {
    setColor(renderer, color);
    SDL_RenderClear(renderer);
}

}

class Camera;
class Drawable;

/// @brief A class that can be drawn to the screen.
/// @details This class is the base class for all drawable objects. It is
/// abstract, so it cannot be instantiated. It has a depth value, which is used
/// to determine the order in which objects are drawn.
class Drawable {
    public:
        int depth = 0;
        /// @brief Draw the object to the screen.
        /// @param camera The camera that is drawing the object.
        /// @details This function is called by the camera to draw the object to the screen.
        virtual void draw(Camera *camera) = 0;
        Drawable();
        ~Drawable();
};

/// @brief A class that represents a camera.
/// @details This class represents a camera that can be used to draw objects to the screen.
class Camera {
    private:
        SDL_Renderer* renderer;
        SDL_Window* window;
        static std::vector<Drawable*> drawables;
        static int cameras;
        Frame2D* frame = nullptr;
        Vector2D center;
    public:
        /// @brief Create a camera.
        /// @param name The name of the window.
        /// @param width The width of the window.
        /// @param height The height of the window.
        /// @details This function creates a camera that can be used to draw objects to the screen. It creates a window with the given name and dimensions.
        Camera(const char *name, Frame2D* frame, int width=640, int height=400) {
            if (cameras == 0) SDL_Init(SDL_INIT_VIDEO);
            window = SDL_CreateWindow(
                    name,
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    width, height,
                    SDL_WINDOW_OPENGL);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            center = Vector2D(width / 2, height / 2);
            cameras += 1;
            this->frame = frame;
        }

        ~Camera() {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            cameras -= 1;
            if (cameras == 0) SDL_Quit();
        }

        /// @brief Add a drawable object to the pool of objects to be drawn.
        /// @param drawable The drawable object to be added.
        /// @details This function adds a drawable object to the pool of objects to be drawn. The object will be drawn in the order of its depth value.
        static void addDrawable(Drawable* drawable) {
            if (drawables.size() == 0) drawables.push_back(drawable);
            for (int i = 0; i < drawables.size(); i++) {
                if (drawables[i]->depth > drawable->depth) {
                    drawables.insert(drawables.begin() + i, drawable);
                    return;
                }
            }
        }

        /// @brief Remove a drawable object from the pool of objects to be drawn.
        /// @param drawable The drawable object to be removed.
        /// @details This function removes a drawable object from the pool of objects to be drawn.
        static void removeDrawable(Drawable* drawable) {
            for (int i = 0; i < drawables.size(); i++) {
                if (drawables[i] == drawable) {
                    drawables.erase(drawables.begin() + i);
                    return;
                }
            }
        }

        /// @brief Set the frame of reference for the camera.
        /// @param frame The frame of reference for the camera.
        /// @details This function sets the frame of reference for the camera. The camera will draw objects in the frame of reference of this frame.
        void setFrame(Frame2D* frame) {
            this->frame = frame;
        }

        /// @brief Get the frame of reference for the camera.
        /// @return The frame of reference for the camera.
        Frame2D* getFrame() {
            return frame;
        }

        /// @brief Draw all objects to the screen.
        /// @details This function draws all objects to the screen.
        /// @todo Make this function only draw objects that are in the camera's view.
        void render() {
            draw::clearScreen(renderer, Color::black());
            for (int i = 0; i < drawables.size(); i++) {
                drawables[i]->draw(this);
            }
            SDL_RenderPresent(renderer);
        }

        /// @brief Set the color that will be used to draw objects.
        /// @param color The color that will be used to draw objects.
        /// @details This function sets the color that will be used to draw objects.
        void setDrawColor(Color color) {
            draw::setColor(renderer, color);
        }

        /// @brief Draw a line.
        /// @param start The start point of the line.
        /// @param end The end point of the line.
        /// @details This function draws a line from the start point to the end point.
        void drawLine(Vector2D start, Vector2D end) {
            draw::line(renderer,
                    center + frame->getLocalCoordinates(start),
                    center + frame->getLocalCoordinates(end));
        }

        /// @brief Draw a circle.
        /// @param center The center of the circle.
        /// @param radius The radius of the circle.
        /// @details This function draws a circle with the given center and radius.
        void drawCircle(Vector2D center, float radius) {
            draw::circle(renderer,
                    this->center + frame->getLocalCoordinates(center),
                    frame->getScale().x * radius);
        }

        /// @brief Draw an arrow.
        /// @param start The start point of the arrow.
        /// @param end The end point of the arrow.
        /// @details This function draws an arrow from the start point to the end point.
        void drawArrow(Vector2D start, Vector2D end) {
            draw::arrow(renderer,
                    center + frame->getLocalCoordinates(start),
                    center + frame->getLocalCoordinates(end));
        }

        /// @brief Draw a cross.
        /// @param center The center of the cross.
        /// @param radius The radius of the cross.
        /// @details This function draws a cross with the given center and radius.
        void drawCross(Vector2D center, float radius) {
            draw::cross(renderer,
                    this->center + frame->getLocalCoordinates(center),
                    frame->getScale().x * radius);
        }

        /// @brief Draw a rectangle.
        /// @param topleft The top left corner of the rectangle.
        /// @param bottomright The bottom right corner of the rectangle.
        /// @details This function draws a rectangle with the given top left and bottom right corners.
        void drawRect(Vector2D topleft, Vector2D bottomright) {
            draw::rect(renderer,
                    this->center + frame->getLocalCoordinates(topleft),
                    this->center + frame->getLocalCoordinates(bottomright));
        }
};

std::vector<Drawable*> Camera::drawables = std::vector<Drawable*>();
int Camera::cameras = 0;

Drawable::Drawable() {
    Camera::addDrawable(this);
}

Drawable::~Drawable() {
    Camera::removeDrawable(this);
}

#endif