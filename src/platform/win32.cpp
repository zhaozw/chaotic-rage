// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#include <string>
#include <iostream>
#include "../rage.h"

using namespace std;



/**
* Returns the path for a directory which we can put some user data.
* The returned path includes a trailing slash.
*
* Example return value:
*    /home/josh/.chaoticrage/
**/
string getUserDataDir()
{
	return "C:/";
}


/**
* Reports a fatal error, and then exits.
* The message should not include a trailing newline.
**/
void reportFatalError(string msg)
{
	string out = "FATAL ERROR: ";
	out.append(msg);

	MessageBox(HWND_DESKTOP, out.c_str(), "Chaotic Rage", MB_OK);

	exit(1);
}
