// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#include <string>
#include <confuse.h>
#include "../rage.h"
#include "../gamestate.h"
#include "../render_opengl/render_opengl.h"
#include "../render_opengl/render_opengl_settings.h"
#include "../render/render_debug.h"
#include "../render/render_null.h"
#include "../audio/audio_sdlmixer.h"
#include "../audio/audio_null.h"
#include "../mod/mod_manager.h"
#include "../physics_bullet.h"
#include "clientconfig.h"
#include "cmdline.h"


using namespace std;


static cfg_opt_t config_opts[] =
{
	CFG_INT((char*) "gl-msaa", 1, CFGF_NONE),
	CFG_INT((char*) "gl-tex-filter", 4, CFGF_NONE),
	
	// Deprecated; only in place to not break existing confings
	CFG_INT((char*) "render", 0, CFGF_NONE),

	CFG_END()
};



/**
* Load the config
* Then save again straight away, to fill the file with defaults
**/
ClientConfig::ClientConfig()
{
	this->load();
}


/**
* Loads the client config
**/
void ClientConfig::load()
{
	cfg_t *cfg;
	string filename = getUserDataDir().append("client.conf");

	// Load config
	cfg = cfg_init(config_opts, CFGF_NONE);
	cfg_parse(cfg, filename.c_str());
	
	this->gl = new RenderOpenGLSettings();
	this->gl->msaa = cfg_getint(cfg, "gl-msaa");
	this->gl->tex_filter = cfg_getint(cfg, "gl-tex-filter");
	
	cfg_free(cfg);
}


/**
* Save the config
**/
void ClientConfig::save()
{
	string filename = getUserDataDir().append("client.conf");

	FILE * fp = fopen(filename.c_str(), "w");
	if (fp == NULL) {
		return;
	}

	// TODO: Save config _properly_
	// Okay so the correct way is the libConfuse cfg_print function
	// but it wasn't working, and I'm getting too tired to keep trying
	fprintf(fp, "# Config file for CR\n");
	fprintf(fp, "gl-msaa = %i\n", this->gl->msaa);
	fprintf(fp, "gl-tex-filter = %i\n", this->gl->tex_filter);

	fclose(fp);
}


/**
* Init rendering engine based on config options
**/
void ClientConfig::initRender(GameState *st)
{
	if (st->cmdline->render == "debug") {
		new RenderDebug(st);
		
	} else if (st->cmdline->render == "null") {
		new RenderNull(st);
		
	} else {
		new RenderOpenGL(st, this->gl);
	}
	
	// TODO: Load these settings from a config file
	st->render->setScreenSize(1000, 700, false);
}


/**
* Init audio engine based on config options
**/
void ClientConfig::initAudio(GameState *st)
{
	if (st->cmdline->audio == "null") {
		new AudioNull(st);
		
	} else {
		new AudioSDLMixer(st);
	}
}


/**
* Init physics engine based on config options
**/
void ClientConfig::initPhysics(GameState *st)
{
	new PhysicsBullet(st);
}


/**
* Init mods engine based on config options
**/
void ClientConfig::initMods(GameState *st)
{
	new ModManager(st);
}
