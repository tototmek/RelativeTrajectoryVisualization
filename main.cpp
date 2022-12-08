#include "physics.h"
#include "Vector2D.h"
#include "graphics.h"
#include "Frame2D.h"
#include <iostream>
#include <SDL2/SDL.h>
#define PRINT(x) std::cout << #x << " = " << x << std::endl
#define PI 3.14159265358979323846

// Function to initialize SDL
void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    *window = SDL_CreateWindow(
        "Simulation",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );
    if (window == NULL)
        printf("Could not create window: %s\n", SDL_GetError());
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
}

// Function to initialize the world with bodies
void initWorld(PhysicsWorld &world) {
    world.addBody(PhysicsBody(Vector2D(320, 240), Vector2D(25, 0), 10));
    world.addBody(PhysicsBody(Vector2D(320, 60), Vector2D(-250, 0), 1));
}

int main(int argc, char *argv[]) {
    // Initialize SDL
    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);

    // Initialize the world
    PhysicsWorld world;
    initWorld(world);
    Frame2D globalFrame = Frame2D(NULL, Vector2D(0, 0), 0, Vector2D(1, 1));
    Frame2D localFrame = Frame2D(&globalFrame, Vector2D(0, 0), 0, Vector2D(1, 1));
    Vector2D crossPosition = Vector2D(100, 0); 
    
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

        renderClearScreen(renderer, Color::black());

        // Update the world
        world.update(deltaTime);
        for (int i = 0; i < world.bodies.size(); i++) {
            for (int j = i + 1; j < world.bodies.size(); j++) {
                PhysicsBody &body1 = world.bodies[i];
                PhysicsBody &body2 = world.bodies[j];
                Vector2D distance = body2.getPosition() - body1.getPosition();
                float distanceMagnitude = distance.magnitude();
                float forceMagnitude = 10000000 / (distanceMagnitude * distanceMagnitude);
                Vector2D force = distance.normalized() * forceMagnitude;
                body1.applyForce(force);
                body2.applyForce(-force);
            }
        }

        // Draw the world
        renderSetColor(renderer, Color::white());
        for (PhysicsBody &body : world.bodies)
            renderDrawCircle(renderer, body.getPosition(), body.getMass() * 3);
        renderSetColor(renderer, Color::white());
        for (PhysicsBody &body : world.bodies)
            renderDrawArrow(renderer, body.getPosition(), body.getPosition() + body.getVelocity() * 0.33);
        renderSetColor(renderer, Color::yellow());
        for (PhysicsBody &body : world.bodies)
            renderDrawArrow(renderer, body.getPosition(), body.getPosition() + body.getTotalForce() * 0.33);
        
        // Test Frame2D
        localFrame.setPosition(world.bodies[1].getPosition());
        localFrame.setRotation(-world.bodies[1].getVelocity().angle());
        renderSetColor(renderer, Color::red());
        renderDrawArrow(renderer, localFrame.getGlobalCoordinates(Vector2D(0, 0)), localFrame.getGlobalCoordinates(Vector2D(50, 0)));
        renderSetColor(renderer, Color::green());
        renderDrawArrow(renderer, localFrame.getGlobalCoordinates(Vector2D(0, 0)), localFrame.getGlobalCoordinates(Vector2D(0, 50)));
        renderSetColor(renderer, Color::red());
        renderDrawArrow(renderer, globalFrame.getGlobalCoordinates(Vector2D(0, 0)), globalFrame.getGlobalCoordinates(Vector2D(50, 0)));
        renderSetColor(renderer, Color::green());
        renderDrawArrow(renderer, globalFrame.getGlobalCoordinates(Vector2D(0, 0)), globalFrame.getGlobalCoordinates(Vector2D(0, 50)));
        renderDrawCross(renderer, localFrame.getGlobalCoordinates(crossPosition), 10);


        // Render the screen and delay the loop
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}