// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#include <string>
#include <SDL.h>
#include <confuse.h>
#include "../rage.h"
#include "serverconfig.h"


using namespace std;


static cfg_opt_t config_opts[] =
{
	CFG_STR((char*) "map", (char*)"nettest", CFGF_NONE),
	CFG_STR((char*) "gametype", (char*)"test", CFGF_NONE),
	CFG_INT((char*) "port", 17778, CFGF_NONE),
	CFG_END()
};



/**
* Load the config
* Then save again straight away, to fill the file with defaults
**/
ServerConfig::ServerConfig()
{
	this->load();
}


/**
* Loads the client config
**/
void ServerConfig::load()
{
	cfg_t *cfg;
	string filename;

	// Load config
	filename = getUserDataDir().append("server.conf");
	cfg = cfg_init(config_opts, CFGF_NONE);

	if (cfg_parse(cfg, filename.c_str()) == CFG_FILE_ERROR) {
		this->savedefault();
	}

	this->map = cfg_getstr(cfg, "map");
	this->gametype = cfg_getstr(cfg, "gametype");
	this->port = cfg_getint(cfg, "port");

	cfg_free(cfg);
}


/**
* Save the config
**/
void ServerConfig::save()
{
	cfg_t *cfg;
	string filename;

	// Load config
	filename = getUserDataDir().append("server.conf");
	cfg = cfg_init(config_opts, CFGF_NONE);
	cfg_parse(cfg, filename.c_str());

	// Fill from variables
	cfg_setstr(cfg, "map", this->map.c_str());
	cfg_setstr(cfg, "gametype", this->gametype.c_str());
	cfg_setint(cfg, "port", this->port);

	// Write to file
	FILE * fp = fopen(filename.c_str(), "w");
	if (fp != NULL) {
		cfg_print(cfg, fp);
		fclose(fp);
	}

	cfg_free(cfg);
}


void ServerConfig::savedefault()
{
	string filename;

	char * def = (char*)
		"#\n"
		"# This is the configuration file for the Chaotic Rage dedicated server.\n"
		"#\n"
		"\n"
		"# The map to play on\n"
		"map = nettest\n"
		"\n"
		"# The gametype to play\n"
		"gametype = test\n"
		"\n"
		"# The server will listen on all interfaces on this UDP port.\n"
		"# The default port is 17778; this is for historical reasons\n"
		"# because Chaotic Rage classic uses 17777.\n"
		"port = 17778\n"
		;

	filename = getUserDataDir().append("server.conf");
	FILE * fp = fopen(filename.c_str(), "w");
	if (fp != NULL) {
		fwrite(def, strlen(def), 1, fp);
		fclose(fp);
	}
}
