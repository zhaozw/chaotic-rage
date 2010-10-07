// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#pragma once
#include <iostream>
#include <SDL.h>
#include "rage.h"

using namespace std;


class WeaponType
{
	public:
		ParticleGenType *pg;
		SpritePtr icon_large;
		
		// dynamic
		int id;
		
	public:
		WeaponType();
};


vector<WeaponType*> * loadAllWeaponTypes(Mod * mod);
