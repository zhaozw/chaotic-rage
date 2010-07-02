#pragma once
#include <iostream>
#include <SDL.h>
#include "rage.h"

using namespace std;


class Particle: public Entity
{
	public:
		virtual EntityClass klass() const { return PARTICLE; }
		
	public:
		ParticleType* pt;
		
		// copied from particle type
		int speed;
		int lin_accel;
		int max_age;
		int unit_damage;
		int wall_damage;
		int unit_hits;
		int wall_hits;
		
		// general instance variables
		int age;
		
	protected:
		unsigned int animation_start;
		
	public:
		Particle(ParticleType *pt, GameState *st);
		~Particle();
		
	public:
		virtual SpritePtr getSprite();
		virtual void update(int delta);
};
