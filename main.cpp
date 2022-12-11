#include "physics.h"
#include "Vector2D.h"
#include "graphics.h"
#include "Frame2D.h"
#include <iostream>
#include <SDL2/SDL.h>
#define PRINT(x) std::cout << #x << " = " << x << std::endl
#define PI 3.14159265358979323846
#define GRAVITATIONAL_CONSTANT 66700000

// Function to initialize the world with bodies
void initWorld(PhysicsWorld &world) {
    world.addBody(PhysicsBody(Vector2D(320, 240), Vector2D(40, 0), 10));
    world.addBody(PhysicsBody(Vector2D(320, 60), Vector2D(-400, 0), 1));
}

// Class extenting drawable used to draw a physics body
class BodyDrawable : public Drawable {
    private:
        PhysicsBody *body;
    public:
        BodyDrawable(PhysicsBody *body) {
            this->body = body;
            this->depth = 1;
        }
        void draw(Camera *camera) {
            Vector2D position = body->getPosition();
            float radius = body->getMass();
            camera->setDrawColor(Color::white());
            camera->drawCircle(position, radius);
            camera->setDrawColor(Color::red());
            camera->drawArrow(position, position + body->getVelocity());
        }
};

// Class extending drawable used to a background grid
class GridDrawable : public Drawable {
    private:
        int width;
        int height;
        int spacing;
    public:
        GridDrawable(int width, int height, int spacing) {
            this->width = width;
            this->height = height;
            this->spacing = spacing;
            this->depth = 10;
        }
        void draw(Camera *camera) {
            camera->setDrawColor(Color::darkGray());
            for (int x = 0; x < width; x += spacing) {
                camera->drawLine(Vector2D(x, 0), Vector2D(x, height));
            }
            for (int y = 0; y < height; y += spacing) {
                camera->drawLine(Vector2D(0, y), Vector2D(width, y));
            }
        }
};

class TrajectoryDrawable : public Drawable {
    private:
        PhysicsBody *body;
        std::vector<Vector2D> points;
    public:
        TrajectoryDrawable(PhysicsBody *body) {
            this->body = body;
            this->depth = 4;
        }
        void draw(Camera *camera) {
            camera->setDrawColor(Color::gray());
            if (points.size() < 2) {
                return;
            }
            for (int i = 0; i < points.size() - 1; ++i) {
                camera->drawLine(points[i], points[i + 1]);
            }
        }
        void addPoint(Vector2D point) {
            points.push_back(point);
        }
        void clear() {
            points.clear();
        }
};

void applyGravitationalForces(float strength, PhysicsWorld &world) {
    for (int i = 0; i < world.bodies.size(); i++) {
        for (int j = i + 1; j < world.bodies.size(); j++) {
            PhysicsBody &body1 = world.bodies[i];
            PhysicsBody &body2 = world.bodies[j];
            Vector2D distance = body2.getPosition() - body1.getPosition();
            float distanceMagnitude = distance.magnitude();
            float forceMagnitude = strength / (distanceMagnitude * distanceMagnitude);
            Vector2D force = distance.normalized() * forceMagnitude;
            body1.applyForce(force);
            body2.applyForce(-force);
        }
    }
}

int main(int argc, char *argv[]) {
    // Initialize SDL
    Camera camera("Simulation", NULL, 1000, 1000);

    // Initialize the world
    PhysicsWorld world;
    initWorld(world);
    Frame2D globalFrame = Frame2D(NULL, Vector2D(0, 0), 0, Vector2D(1, 1));
    Frame2D cameraFrame = Frame2D(&globalFrame, Vector2D(0, 0), 0, Vector2D(1, 1));
    camera.setFrame(&cameraFrame);
    BodyDrawable bodyDrawable1(&world.bodies[0]);
    BodyDrawable bodyDrawable2(&world.bodies[1]);
    GridDrawable gridDrawable(1000, 1000, 100);
    TrajectoryDrawable trajectoryDrawable1(&world.bodies[1]);
    
    // Main loop
    bool running = true;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;
    float deltaTime;
    while (running) {
        // Calculate delta time
        currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Handle SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update the world
        world.update(deltaTime);
        applyGravitationalForces(GRAVITATIONAL_CONSTANT, world);

        trajectoryDrawable1.clear();
        PhysicsWorld world_copy = world.clone();
        for (int i=0; i<100; i++) {
            world_copy.update(20/world_copy.bodies[1].getVelocity().magnitude());
            applyGravitationalForces(GRAVITATIONAL_CONSTANT, world_copy);
            trajectoryDrawable1.addPoint(world_copy.bodies[1].getPosition());
        }

        // Draw the world
        cameraFrame.setPosition(
            Vector2D::lerp(
                cameraFrame.getPosition(),
                world.bodies[1].getPosition(),
                0.01));
        camera.render();


        SDL_Delay(5);
    }
    SDL_Quit();
    return 0;
}