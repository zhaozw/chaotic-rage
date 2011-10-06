// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#pragma once
#include <iostream>
#include <SDL.h>
#include "rage.h"

using namespace std;


#define WEAPON_SOUND_BEGIN 1
#define WEAPON_SOUND_REPEAT 2
#define WEAPON_SOUND_END 3
#define WEAPON_SOUND_RELOAD 4


class WeaponTypeSound
{
	public:
		int id;
		Sound * snd;
		int type;
};

class WeaponType
{
	public:
		int id;
		string name;
		string title;
		
		// TODO: Not used anymore, rejig for effects maybe?
		ParticleGenType *pg;
		
		ParticleType *pt;
		int angle_range;
		unsigned int rate;
		bool continuous;
		int magazine_limit;
		int belt_limit;
		
		vector<WeaponTypeSound*> sounds;
		
	public:
		WeaponType();
		
	public:
		Sound* getSound(int type);
};


// Config file opts
extern cfg_opt_t weapontype_opts [];

// Item loading function handler
WeaponType* loadItemWeaponType(cfg_t* cfg_item, Mod* mod);

