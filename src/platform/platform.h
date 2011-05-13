// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#pragma once
#include <string>
#include <iostream>
#include "../rage.h"

using namespace std;


/**
* Some help for some compilers
**/
#ifdef _WIN32
	#include <windows.h>
	static inline double round(double val) { return floor(val + 0.5); }
	#define or ||
	#define and &&
#endif


/**
* Returns the path for a directory which we can put some user data.
* The returned path includes a trailing slash.
*
* Example return value:
*    /home/josh/.chaoticrage/
**/
string getUserDataDir();


/**
* Reports a fatal error, and then exits.
* The message should not include a trailing newline.
**/
void reportFatalError(string msg);

