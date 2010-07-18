// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#include <iostream>
#include <SDL.h>
#include "rage.h"

using namespace std;


EntityType::EntityType()
{
	this->actions = NULL;
}

EntityType::~EntityType()
{
	delete(this->actions);
}


/**
* Executes a set of actions for a given event for a given entity
*
* TODO: make arguments be an EVENT object.
**/
void EntityType::doActions(Entity * entity, EventType event)
{
	unsigned int i;
	
	if (this->actions == NULL) return;
	
	GameState *st = entity->getGameState();
	
	DEBUG("Event %i from entity %p (type = %i, x = %i, y = %i, del = %i)\n", event, entity, entity->klass(), entity->x, entity->y, entity->del);
	
	for (i = 0; i < this->actions->size(); i++) {
		Action *ac = this->actions->at(i);
		
		if (ac->event != event) continue;
		
		switch (ac->type) {
			case ADD_PGENERATOR:
				{
					ParticleGenerator *pg = new ParticleGenerator(getParticleGenTypeByID(ac->args[0]), st);
					pg->x = entity->x;
					pg->y = entity->y;
					st->addParticleGenerator(pg);
				}
				break;
				
			default:
				break;
		}
	}
}
