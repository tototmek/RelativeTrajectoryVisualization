#ifndef PHYSICS_H
#define PHYSICS_H
#include "Vector2D.h"
#include <vector>

class PhysicsBody {
    public:
        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;
        float mass;

        PhysicsBody() {
            position = Vector2D::zero();
            velocity = Vector2D::zero();
            acceleration = Vector2D::zero();
            mass = 1;
        }

        PhysicsBody(Vector2D position, Vector2D velocity, float mass) {
            this->position = position;
            this->velocity = velocity;
            this->mass = mass;
        }

        void update(float dt) {
            velocity += acceleration * dt;
            position += velocity * dt;
            acceleration = Vector2D::zero();
        }

        void applyForce(Vector2D force) {
            acceleration += force / mass;
        }

        void applyAcceleration(Vector2D acceleration) {
            acceleration += acceleration;
        }

        void setMass(float mass) {
            this->mass = mass;
        }

        float getMass() {
            return mass;
        }

        void setVelocity(Vector2D velocity) {
            this->velocity = velocity;
        }

        Vector2D getVelocity() {
            return velocity;
        }

        void setPosition(Vector2D position) {
            this->position = position;
        }

        Vector2D getPosition() {
            return position;
        }

        Vector2D getAcceleration() {
            return acceleration;
        }

        Vector2D getTotalForce() {
            return acceleration * mass;
        }
};

class PhysicsWorld {
    public:
        int ticks = 0;
        std::vector<PhysicsBody> bodies;

        PhysicsWorld() {
            bodies = std::vector<PhysicsBody>();
        }

        void addBody(PhysicsBody body) {
            bodies.push_back(body);
        }

        void removeBody(PhysicsBody body) {
            for (int i = 0; i < bodies.size(); i++) {
                if (&bodies[i] == &body) {
                    bodies.erase(bodies.begin() + i);
                    return;
                }
            }
        }

        void update(float dt) {
            ++ticks;
            for (int i = 0; i < bodies.size(); i++) {
                bodies[i].update(dt);
            }
        }
};

#endif