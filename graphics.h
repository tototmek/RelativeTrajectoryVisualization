#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL2/SDL.h>
#include <Vector2D.h>

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

void renderSetColor(SDL_Renderer *renderer, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void renderDrawLine(SDL_Renderer *renderer, Vector2D start, Vector2D end) {
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void renderDrawCircle(SDL_Renderer *renderer, Vector2D center, float radius) {
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

void renderDrawArrow(SDL_Renderer *renderer, Vector2D start, Vector2D end) {
    renderDrawLine(renderer, start, end);
    Vector2D dir = end - start;
    dir /= dir.magnitude();
    Vector2D perp = dir.perpendicular();
    Vector2D arrow1 = end - dir * 10 + perp * 5;
    Vector2D arrow2 = end - dir * 10 - perp * 5;
    renderDrawLine(renderer, end, arrow1);
    renderDrawLine(renderer, end, arrow2);
}

void renderDrawCross(SDL_Renderer *renderer, Vector2D center, float radius) {
    renderDrawLine(renderer, center + Vector2D(-radius, -radius), center + Vector2D(radius, radius));
    renderDrawLine(renderer, center + Vector2D(-radius, radius), center + Vector2D(radius, -radius));
}

void renderDrawRect(SDL_Renderer *renderer, Vector2D center, float width, float height) {
    SDL_Rect rect;
    rect.x = center.x - width / 2;
    rect.y = center.y - height / 2;
    rect.w = width;
    rect.h = height;
    SDL_RenderDrawRect(renderer, &rect);
}

void renderDrawRect(SDL_Renderer *renderer, Vector2D topleft, Vector2D bottomright) {
    SDL_Rect rect;
    rect.x = topleft.x;
    rect.y = topleft.y;
    rect.w = bottomright.x - topleft.x;
    rect.h = bottomright.y - topleft.y;
    SDL_RenderDrawRect(renderer, &rect);
}

void renderClearScreen(SDL_Renderer *renderer, Color color) {
    renderSetColor(renderer, color);
    SDL_RenderClear(renderer);
}

#endif