// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#pragma once
#include "../rage.h"
#include "../audio/audio.h"

using namespace std;


class Sound
{
	public:
		// from data file
		string name;
		CRC32 id;
		
		// dynamic
		AudioPtr sound;
		
	public:
		Sound();
};


// Config file opts
extern cfg_opt_t sound_opts [];

// Item loading function handler
Sound* loadItemSound(cfg_t* cfg_item, Mod* mod);
