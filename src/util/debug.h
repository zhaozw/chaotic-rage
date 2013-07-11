// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#pragma once
#include "../rage.h"

using namespace std;


// Debugging messages
// Not used anywhere near as often as they should be used
#ifdef DEBUG_OPTIONS
	#define WHEREARG __FILE__, __LINE__
	#define DEBUGPRINT2(...) write_debug(__VA_ARGS__)
	#define DEBUG(_sect, _fmt, ...) DEBUGPRINT2(_sect, WHEREARG, _fmt, __VA_ARGS__)
#else
	#define DEBUG(_sect, _fmt, ...) /* nothing! */
#endif


void write_debug(const char * sect, const char * file, int line, const char * fmt, ...);

void debug_enable(const char * sect);

bool debug_enabled(const char * sect);

void debug_lineno(bool enabled);

void debug_tofile(const char * filename);

void debug_save();
