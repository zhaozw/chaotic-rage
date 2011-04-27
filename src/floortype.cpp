// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#include <iostream>
#include <SDL.h>
#include <confuse.h>
#include <zzip/zzip.h>
#include "rage.h"

using namespace std;


/* Functions */
FloorType* loadFloorType(cfg_t *cfg_areatype, Mod * mod);


/* Config file definition */
// Damage
static cfg_opt_t damage_opts[] =
{
	CFG_INT((char*) "health", 0, CFGF_NONE),
	CFG_STR((char*) "model", (char*)"", CFGF_NONE),
	CFG_END()
};

// Areatype section
static cfg_opt_t areatype_opts[] =
{
	CFG_STR((char*) "name", (char*)"", CFGF_NONE),
	CFG_STR((char*) "image", (char*)"", CFGF_NONE),
	CFG_STR((char*) "model", (char*)"", CFGF_NONE),
	CFG_INT((char*) "stretch", 0, CFGF_NONE),		// 0 = tile, 1 = stretch
	CFG_INT((char*) "wall", 0, CFGF_NONE),			// 0 = ground, 1 = wall
	CFG_INT((char*) "ground_type", -1, CFGF_NONE),	// Ground to place underneath this wall
	CFG_STR_LIST((char*) "walk_sounds", 0, CFGF_NONE),
	CFG_SEC((char*) "damage", damage_opts, CFGF_MULTI),
	CFG_END()
};

// Main config
static cfg_opt_t opts[] =
{
	CFG_SEC((char*) "areatype", areatype_opts, CFGF_MULTI),
	CFG_END()
};




FloorType::FloorType()
{
	this->surf = NULL;
	this->ground_type = NULL;
}


/**
* Loads the area types
**/
vector<FloorType*> * loadAllFloorTypes(Mod * mod)
{
	vector<FloorType*> * areatypes = new vector<FloorType*>();
	
	char *buffer;
	cfg_t *cfg, *cfg_areatype;
	
	
	// Load + parse the config file
	buffer = mod->loadText("areatypes/areatypes.conf");
	if (buffer == NULL) {
		return NULL;
	}
	
	cfg = cfg_init(opts, CFGF_NONE);
	cfg_parse_buf(cfg, buffer);
	
	free(buffer);
	
	
	int num_types = cfg_size(cfg, "areatype");
	if (num_types == 0) return NULL;
	
	// Process area type sections
	int j;
	for (j = 0; j < num_types; j++) {
		cfg_areatype = cfg_getnsec(cfg, "areatype", j);
		
		FloorType* at = loadFloorType(cfg_areatype, mod);
		if (at == NULL) {
			cerr << "Bad area type at index " << j << endl;
			return NULL;
		}
		
		areatypes->push_back(at);
		at->id = areatypes->size() - 1;
	}
	
	// If there was sprite errors, exit the game
	if (mod->st->render->wasLoadSpriteError()) {
		cerr << "Error loading area types; game will now exit.\n";
		exit(1);
	}
	
	return areatypes;
}


/**
* Loads a single area type
**/
FloorType* loadFloorType(cfg_t *cfg_areatype, Mod * mod)
{
	FloorType* at;
	string filename;
	int size;
	
	filename = "areatypes/";
	filename.append(cfg_getstr(cfg_areatype, "image"));
	filename.append("-fr0.png");
	
	at = new FloorType();
	at->name = cfg_getstr(cfg_areatype, "name");
	at->surf = mod->st->render->loadSprite(filename.c_str(), mod);
	at->stretch = cfg_getint(cfg_areatype, "stretch");
	at->wall = cfg_getint(cfg_areatype, "wall");
	
	char * tmp = cfg_getstr(cfg_areatype, "model");
	if (tmp != NULL) {
		at->model = mod->getAnimModel(tmp);
	}
	
	// Load damage states
	size = cfg_size(cfg_areatype, "damage");
	for (int j = 0; j < size; j++) {
		cfg_t *cfg_damage = cfg_getnsec(cfg_areatype, "damage", j);
		
		char * tmp = cfg_getstr(cfg_damage, "model");
		if (tmp == NULL) return NULL;
		
		FloorTypeDamage * dam = new FloorTypeDamage();
		
		dam->health = cfg_getint(cfg_damage, "health");
		dam->model = mod->getAnimModel(tmp);
		
		at->damage_models.push_back(dam);
	}
	
	// Load walk sounds
	size = cfg_size(cfg_areatype, "walk_sounds");
	for (int j = 0; j < size; j++) {
		filename = "areatypes/";
		filename.append(cfg_getnstr(cfg_areatype, "walk_sounds", j));
		filename.append(".wav");
		
		AudioPtr sound = mod->st->audio->loadSound(filename, mod);
		if (sound == NULL) return NULL;
		at->walk_sounds.push_back(sound);
	}
	
	// TODO: move to after the mod has loaded
	//FloorType *ground = mod->getFloorType(cfg_getint(cfg_areatype, "ground_type"));
	//if (ground != NULL && ground->wall == 0) {
	//	at->ground_type = ground;
	//}
	
	return at;
}

