#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>

#ifdef DEBUG_MODE
	#define WHERESTR "%s:%d\t"
	#define WHEREARG __FILE__, __LINE__
	#define DEBUGPRINT2(...) fprintf(stderr, __VA_ARGS__)
	#define DEBUG(_fmt, ...) DEBUGPRINT2(WHERESTR _fmt, WHEREARG, __VA_ARGS__)
#else
	#define DEBUG(_fmt, ...) /* nothing! */
#endif

#include "entity.h"
#include "unitclass.h"
#include "unit.h"
#include "player.h"
#include "areatypes.h"
#include "map.h"
#include "gamestate.h"

#include "game.h"
#include "video.h"
#include "util.h"
#include "spriteutil.h"

#include "SDL_rwops_zzip.h"
#include "SDL_rotozoom.h"

