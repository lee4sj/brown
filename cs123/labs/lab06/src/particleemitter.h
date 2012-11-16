#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "common.h"
#include <QtOpenGL>

class ParticleEmitter
{
    /**
      * Basic definition for a particle. You should not need to modify this.
      */
    struct __attribute__ ((aligned (16))) Particle
    {
        /**
          * Determines whether or not this particle is active. If it is not active,
          * it should not be drawn or modified by updateParticles().
          */
        bool active;
        /**
          * The amount of life remaining. When the particle is first born, it should
          * have a life of 1.0. Each step this should decrease by decay (i.e.
          * life -= decay). When this value reaches zero, the particle should reset.
          */
        float life;
        /**
          * The amount the life of this particle decreases per step. See the documentation
          * for Particle.life.
          */
        float decay;
        /**
          * The particle's color. Currently the same for all particles; however, it would be
          * possible to have randomly generated colors or colors based on time. Used when
          * drawing the particle.
          */
        float3 color;
        /**
          * The particle's current position in 3D space. Updated every step based on
          * the particle's velocity.
          */
        float3 pos;
        /**
          * The direction this particle is currently moving. The velocity of any particle
          * in the system is: velocity = ParticleEmitter.m_speed * Particle.dir
          */
        float3 dir;
        /**
          * The force acting on this particle (e.g. from gravity). At each update step,
          * Particle.dir += Particle.force.
          */
        float3 force;
    };


public:
    ParticleEmitter(GLuint textureId = 0,
                    float3 color = float3(1.0f, 0.5f, 0.2f),
                    float3 velocity = float3(0.0f, 0.0001f, 0.0f),
                    float3 force = float3(0.0f, 0.0001f, 0.0f),
                    float scale = .5f,
                    float fuzziness = 50.0f,
                    float speed = 50.0f / 10000.0f,
                    unsigned maxParticles = 10000);

    ~ParticleEmitter();

    /**
      * You need to fill this in.
      *
      * Draws each particle as a small, texture-mapped square of side-length m_scale.
      * Each square should be in the X/Y plane at Z = the particle's position's Z-coordinate.
      */
    void drawParticles();
    /**
      * You need to fill this in.
      *
      * Resets the particle at the given index to its initial state. Should reset the
      * position, direction, force, color, life, and decay of the particle.
      */
    void resetParticle(unsigned i);
    /**
      * Resets all particles in this emitter to their initial states
      */
    void resetParticles();
    /**
      * You need to fill this in.
      *
      * Performs one step of the particle simulation. Should perform all physics
      * calculations and maintain the life property of each particle.
      */
    void updateParticles();

    // Do not modify the following functions (they are integrated with the UI)
    /**
      * Gets a mutable reference to the force being applied to all particles in the
      * simulation.
      */
    inline float3& force() { return m_force; }
    /**
      * Gets a mutable reference to the initial velocity of each particle in simulation.
      * Updating this value only affects particles emitted afterward.
      */
    inline float3& velocity() { return m_velocity; }
    /**
      * Gets a mutable reference to the color all particles are emitted with.
      */
    inline float3& color() { return m_color; }
    /**
      * Gets the speed at which particles in this simulation move.
      */
    inline float& speed() { return m_speed; }
    /**
      * Gets the ID of the OpenGL texture that is drawn once for each particle in the
      * scene.
      */
    inline GLuint texture() { return m_textureID; }

protected:
    /** Contains all particles in the scene, active or inactive */
    Particle *m_particles;
    /** The maximum number of particles in the scene (same as the size of the m_particles array) */
    unsigned m_maxParticles;
    /** The ID of the OpenGL texture that is used to draw particles */
    GLuint m_textureID;
    /** The speed that particles move in the simulation */
    float m_speed;
    /** The randomness of particle original direction */
    float m_fuzziness;
    /** The size of each particle (i.e. the length of each particle's square) */
    float m_scale;
    /** The color of all the particles in the scene */
    float3 m_color;
    /** The initial velocity of every particle emitted into the scene */
    float3 m_velocity;
    /** The force vector applied to every particle emitted into the scene */
    float3 m_force;
    int numTimes;
};


#endif // PARTICLEEMITTER_H
