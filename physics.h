#ifndef PHYSICS_H
#define PHYSICS_H
#include "Vector2D.h"
#include <vector>

/// @brief A physics body with position, velocity, acceleration, and mass.
/// @details This class is used to represent a physics body in a physics world.
/// It has a position, velocity, acceleration, and mass. It can be updated
/// with a time step, and it can be given a force to apply to it.
class PhysicsBody {
    public:
        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;
        float mass;

        /// @brief Create a physics body with default values.
        /// @details This function creates a physics body with default values. The default values are:
        /// - position: (0, 0)
        /// - velocity: (0, 0)
        /// - acceleration: (0, 0)
        /// - mass: 1
        /// @see PhysicsBody(Vector2D, Vector2D, float)
        PhysicsBody() {
            position = Vector2D::zero();
            velocity = Vector2D::zero();
            acceleration = Vector2D::zero();
            mass = 1;
        }

        /// @brief Create a physics body with the given values.
        /// @param position The position of the physics body.
        /// @param velocity The velocity of the physics body.
        /// @param mass The mass of the physics body.
        /// @details This function creates a physics body with the given values.
        /// @see PhysicsBody()
        PhysicsBody(Vector2D position, Vector2D velocity, float mass) {
            this->position = position;
            this->velocity = velocity;
            this->mass = mass;
        }

        /// @brief Update the physics body with a time step.
        /// @param dt The time step to update the physics body with.
        /// @details Calculates the new position and velocity of the physics body based on its acceleration. The acceleration is reset to zero after the update.
        void update(float dt) {
            velocity += acceleration * dt;
            position += velocity * dt;
            acceleration = Vector2D::zero();
        }

        /// @brief Apply a force to the physics body.
        /// @param force The force to apply to the physics body.
        /// @details This function calculates the acceleration of the physics body based on the given force and the mass of the physics body. The acceleration is added to the current acceleration of the physics body.
        void applyForce(Vector2D force) {
            acceleration += force / mass;
        }

        /// @brief Apply an acceleration to the physics body.
        /// @param acceleration The acceleration to apply to the physics body.
        /// @details This function adds the given acceleration to the current acceleration of the physics body. This is useful for adding gravity to the physics body.
        /// @note This function does not take the mass of the physics body into account.
        /// @see applyForce
        void applyAcceleration(Vector2D acceleration) {
            acceleration += acceleration;
        }

        /// @brief Set the mass of the physics body.
        /// @param mass The mass of the physics body.
        /// @details This function sets the mass of the physics body. The mass is used to calculate the acceleration of the physics body when a force is applied to it.
        /// @see applyForce
        void setMass(float mass) {
            this->mass = mass;
        }

        /// @brief Get the mass of the physics body.
        /// @return The mass of the physics body.
        /// @see setMass
        float getMass() {
            return mass;
        }

        /// @brief Set the velocity of the physics body.
        /// @param velocity The velocity of the physics body.
        /// @details This function sets the velocity of the physics body. The velocity is used to calculate the new position of the physics body when the physics body is updated.
        void setVelocity(Vector2D velocity) {
            this->velocity = velocity;
        }

        /// @brief Get the velocity of the physics body.
        /// @return The velocity of the physics body.
        /// @see setVelocity
        Vector2D getVelocity() {
            return velocity;
        }

        /// @brief Set the position of the physics body.
        /// @param position The position of the physics body.
        /// @details This function sets the position of the physics body. The position is used to calculate the new position of the physics body when the physics body is updated.
        void setPosition(Vector2D position) {
            this->position = position;
        }

        /// @brief Get the position of the physics body.
        /// @return The position of the physics body.
        /// @see setPosition
        Vector2D getPosition() {
            return position;
        }

        /// @brief Get the acceleration of the physics body.
        /// @return The acceleration of the physics body.
        Vector2D getAcceleration() {
            return acceleration;
        }

        /// @brief Get the total force applied to the physics body.
        /// @return The total force applied to the physics body.
        /// @details This function calculates the total force applied to the physics body by multiplying the acceleration of the physics body by the mass of the physics body. This is useful for visualizing the force applied to the physics body.
        Vector2D getTotalForce() {
            return acceleration * mass;
        }

        /// @brief Clone the physics body.
        /// @return A copy of the physics body.
        /// @details This function creates a copy of the physics body. The copy has the same position, velocity, and mass as the original physics body.
        PhysicsBody clone() {
            return PhysicsBody(position, velocity, mass);
        }
};


/// @brief A physics world that contains physics bodies.
/// @details This class is used to represent a physics world that contains physics bodies. It can be updated with a time step, and it can add and remove physics bodies.
class PhysicsWorld {
    public:
        /// @brief The number of physics ticks that have occurred.
        int ticks = 0;
        /// @brief The physics bodies in the physics world.
        std::vector<PhysicsBody> bodies;

        /// @brief Create an empty physics world.
        PhysicsWorld() {
            bodies = std::vector<PhysicsBody>();
        }

        /// @brief Add a physics body to the physics world.
        /// @param body The physics body to add to the physics world.
        void addBody(PhysicsBody body) {
            bodies.push_back(body);
        }

        /// @brief Remove a physics body from the physics world.
        /// @param body The physics body to remove from the physics world.
        void removeBody(PhysicsBody body) {
            for (int i = 0; i < bodies.size(); i++) {
                if (&bodies[i] == &body) {
                    bodies.erase(bodies.begin() + i);
                    return;
                }
            }
        }

        /// @brief Update the physics world with a time step.
        /// @param dt The time step to update the physics world with.
        /// @details Updates all the physics bodies in the physics world with the given time step.
        /// @see PhysicsBody::update(float dt)
        void update(float dt) {
            ++ticks;
            for (int i = 0; i < bodies.size(); i++) {
                bodies[i].update(dt);
            }
        }

        /// @brief Get a copy of the physics world.
        /// @return A copy of the physics world.
        /// @details This function creates a copy of the physics world. The physics bodies in the physics world are also copied.
        PhysicsWorld clone () {
            PhysicsWorld world;
            world.ticks = ticks;
            for (int i = 0; i < bodies.size(); i++) {
                world.addBody(bodies[i].clone());
            }
            return world;
        }
};

#endif