// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#include <iostream>
#include <SDL.h>
#include <confuse.h>
#include <zzip/zzip.h>
#include "rage.h"

using namespace std;


// It is expected you will change the HUD code if you change these values
#define WEAPON_LARGE_W 125
#define WEAPON_LARGE_H 125


/* Functions */
WeaponType* loadWeaponType(cfg_t *cfg_weapon, Mod * mod);


/* Config file definition */
// Weapon section
static cfg_opt_t weapon_opts[] =
{
	CFG_STR((char*) "name", 0, CFGF_NONE),
	CFG_INT((char*) "particlegen", -1, CFGF_NONE),
	CFG_END()
};

// Main config
static cfg_opt_t opts[] =
{
	CFG_SEC((char*) "weapon", weapon_opts, CFGF_MULTI),
	CFG_END()
};




WeaponType::WeaponType()
{
	this->pg = NULL;
	this->icon_large = NULL;
}


/**
* Loads the area types
**/
vector<WeaponType*> * loadAllWeaponTypes(Mod * mod)
{
	vector<WeaponType*> * weapons = new vector<WeaponType*>();
	
	char *buffer;
	cfg_t *cfg, *cfg_weapon;
	
	
	// Load + parse the config file
	buffer = mod->loadText("weapontypes/weapontypes.conf");
	if (buffer == NULL) {
		return NULL;
	}
	
	cfg = cfg_init(opts, CFGF_NONE);
	cfg_parse_buf(cfg, buffer);
	
	free(buffer);
	
	
	int num_types = cfg_size(cfg, "weapon");
	if (num_types == 0) return NULL;
	
	// Process area type sections
	int j;
	for (j = 0; j < num_types; j++) {
		cfg_weapon = cfg_getnsec(cfg, "weapon", j);
		
		WeaponType* wt = loadWeaponType(cfg_weapon, mod);
		if (wt == NULL) {
			cerr << "Bad weapon type at index " << j << endl;
			return NULL;
		}
		
		weapons->push_back(wt);
		wt->id = weapons->size() - 1;
	}
	
	// If there was sprite errors, exit the game
	if (mod->st->render->wasLoadSpriteError()) {
		cerr << "Error loading weapon types; game will now exit.\n";
		exit(1);
	}
	
	return weapons;
}


/**
* Loads a single area type
**/
WeaponType* loadWeaponType(cfg_t *cfg_weapon, Mod * mod)
{
	WeaponType* wt;
	string filename;
	
	wt = new WeaponType();
	
	if (cfg_getint(cfg_weapon, "particlegen") != -1) {
		wt->pg = mod->getParticleGenType(cfg_getint(cfg_weapon, "particlegen"));
	}
	
	// Load large icon
	filename = getDataDirectory(DF_WEAPONS);
	filename.append(cfg_getstr(cfg_weapon, "name"));
	filename.append("/icon_large.png");
	wt->icon_large = mod->st->render->loadSprite(filename, mod);
	
	if (mod->st->render->getSpriteWidth(wt->icon_large) != WEAPON_LARGE_W && mod->st->render->getSpriteHeight(wt->icon_large) != WEAPON_LARGE_H) {
		cout << "Bad image size for large icon\n";
		return NULL;
	}
	
	return wt;
}

