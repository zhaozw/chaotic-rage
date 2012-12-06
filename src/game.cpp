// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#include <iostream>
#include <SDL.h>
#include <math.h>
#include "rage.h"

using namespace std;


static void handleEvents(GameState *st);


// Mouse movement, including keyboard simulation
static int game_x[MAX_LOCAL], game_y[MAX_LOCAL];
static int net_x[MAX_LOCAL], net_y[MAX_LOCAL];
static int mk_down_x[MAX_LOCAL], mk_down_y[MAX_LOCAL];


/**
* The main game loop
**/
void gameLoop(GameState *st, Render *render)
{
	int start = 0, delta = 0, net_time = 0, net_timestep = 50;
	
	for (int i = 0; i < MAX_LOCAL; i++) {
		game_x[i] = game_y[i] = net_x[i] = net_y[i] = mk_down_x[i] = mk_down_y[i] = 0;
	}

	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_WarpMouse(400, 300);
	SDL_EnableUNICODE(0);

	start = SDL_GetTicks();

	st->render->preGame();
	st->render->loadHeightmap();

	for (unsigned int i = 0; i < st->num_local; i++) {
		st->local_players[i]->hud->removeAllDataTables();
	}

	if (st->client == NULL) {
		st->curr_map->loadDefaultEntities();
	}

	st->start();
	st->setMouseGrab(true);
	st->logic->raise_gamestart();
	
	if (st->client == NULL) {
		for (unsigned int i = 0; i < st->num_local; i++) {
			st->logic->raise_playerjoin(st->local_players[i]->slot);
			st->local_players[i]->hud->removeAllDataTables();
		}
	}


	cout << "\n\n\n\n\n";
	
	st->running = true;
	while (st->running) {
		delta = SDL_GetTicks() - start;
		start = SDL_GetTicks();
		
		st->logic->update(delta);
		st->update(delta);
		handleEvents(st);
		
		if (st->getMouseGrab()) {
			if (st->local_players[0]->p) st->local_players[0]->p->angleFromMouse(game_x[0], game_y[0], delta);		// one of these two is correct...(this one or the one 15 lines below)
			if (st->local_players[1]->p) st->local_players[1]->p->angleFromMouse(game_x[1], game_y[1], delta);
			game_x[0] = game_y[0] = 0;
			game_x[1] = game_y[1] = 0;
			
			SDL_WarpMouse(400, 300);
		}
		
		net_time += delta;
		if (net_time > net_timestep) {
			net_time -= net_timestep;
			
			if (st->client) {
				if (st->local_players[0]->p && st->getMouseGrab()) {
					//st->local_players[0]->p->angleFromMouse(net_x, net_y, net_timestep);
					st->client->addmsgKeyMouseStatus(net_x[0], net_y[0], net_timestep, st->local_players[0]->p->packKeys());
					net_x[0] = net_y[0] = 0;
				}
				st->client->update();
			}
			
			if (st->server) {
				st->server->update();
			}
		}
		
		st->render->render();
		st->audio->play();
		
		SDL_Delay(10);
	}
	
	cout << "\n\n\n\n\n";
	
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_EnableUNICODE(1);

	st->render->postGame();
	st->render->freeHeightmap();
	st->audio->stopAll();
	st->physics->postGame();
	
	st->clear();
}


/**
* Handles local events (keyboard, mouse)
**/
static void handleEvents(GameState *st)
{
	SDL_Event event;
	
	static int screenshot_num = 0;
	
	
	
	while (SDL_PollEvent(&event)) {
		// General keys
		if (event.type == SDL_QUIT) {
			st->running = false;
			
		} else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					if (! st->hasDialog("quit")) {
						st->addDialog(new DialogQuit(st));
					}
					break;

				case SDLK_PRINT:
					{
						screenshot_num++;
						char buf[50];
						sprintf(buf, "%i", screenshot_num);
				
						string filename = getUserDataDir();
						filename.append("screenshot");
						filename.append(buf);
						filename.append(".bmp");
						((Render3D*) st->render)->saveScreenshot(filename);
				
						filename = "screenshot";
						filename.append(buf);
						filename.append(".bmp");
						st->HUDMessage(ALL_SLOTS, "Saved ", filename);
					}
					break;

				case SDLK_F5:
					st->HUDMessage(ALL_SLOTS, "Config reload: ", st->mm->reloadAttrs() ? "success" : "failure");
					break;
						
				case SDLK_F6:
					st->setMouseGrab(! st->getMouseGrab());
					st->HUDMessage(ALL_SLOTS, "Reset-mouse ", st->getMouseGrab() ? "on" : "off");
					break;
						
				case SDLK_F7:
					st->render->enablePhysicsDebug();
					st->HUDMessage(ALL_SLOTS, "Physics debug on");
					break;
					
				case SDLK_F8:
					st->render->disablePhysicsDebug();
					st->HUDMessage(ALL_SLOTS, "Physics debug off");
					break;
					
				default: break;
			}
		}


		if (st->hasDialogs()) {
			st->guiinput->pushInput(event);
			continue;
		}


		// TODO: More dynamic
		
		// One player, player one
		if (st->num_local == 1 && st->local_players[0]->p != NULL) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_w: st->local_players[0]->p->keyPress(Player::KEY_UP); break;
					case SDLK_a: st->local_players[0]->p->keyPress(Player::KEY_LEFT); break;
					case SDLK_s: st->local_players[0]->p->keyPress(Player::KEY_DOWN); break;
					case SDLK_d: st->local_players[0]->p->keyPress(Player::KEY_RIGHT); break;
					case SDLK_e: st->local_players[0]->p->keyPress(Player::KEY_USE); break;
					case SDLK_q: st->local_players[0]->p->keyPress(Player::KEY_LIFT); break;
					case SDLK_t: st->local_players[0]->p->keyPress(Player::KEY_SPECIAL); break;
					default: break;
				}
			
			} else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
					case SDLK_w: st->local_players[0]->p->keyRelease(Player::KEY_UP); break;
					case SDLK_a: st->local_players[0]->p->keyRelease(Player::KEY_LEFT); break;
					case SDLK_s: st->local_players[0]->p->keyRelease(Player::KEY_DOWN); break;
					case SDLK_d: st->local_players[0]->p->keyRelease(Player::KEY_RIGHT); break;
					case SDLK_e: st->local_players[0]->p->keyRelease(Player::KEY_USE); break;
					case SDLK_q: st->local_players[0]->p->keyRelease(Player::KEY_LIFT); break;
					case SDLK_t: st->local_players[0]->p->keyRelease(Player::KEY_SPECIAL); break;
					default: break;
				}
			
			} else if (event.type == SDL_MOUSEMOTION) {
				game_x[0] += event.motion.x - 400;
				net_x[0] += event.motion.x - 400;
				game_y[0] += event.motion.y - 300;
				net_y[0] += event.motion.y - 300;
			
			} else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				st->local_players[0]->hud->eventClick();
				st->local_players[0]->p->keyPress(Player::KEY_FIRE);
			
			} else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
				st->local_players[0]->p->keyRelease(Player::KEY_FIRE);

			} else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
				st->local_players[0]->p->keyPress(Player::KEY_MELEE);
			
			} else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {
				st->local_players[0]->p->keyRelease(Player::KEY_MELEE);

			} else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_WHEELUP) {
				st->local_players[0]->hud->eventUp();
			
			} else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_WHEELDOWN) {
				st->local_players[0]->hud->eventDown();

			}

		} // end one player, one


		// Two players, player one
		if (st->num_local == 2 && st->local_players[0]->p != NULL) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_w: st->local_players[0]->p->keyPress(Player::KEY_UP); break;
					case SDLK_a: st->local_players[0]->p->keyPress(Player::KEY_LEFT); break;
					case SDLK_s: st->local_players[0]->p->keyPress(Player::KEY_DOWN); break;
					case SDLK_d: st->local_players[0]->p->keyPress(Player::KEY_RIGHT); break;
					case SDLK_e: st->local_players[0]->p->keyPress(Player::KEY_USE); break;
					case SDLK_q: st->local_players[0]->p->keyPress(Player::KEY_LIFT); break;
					case SDLK_t: st->local_players[0]->p->keyPress(Player::KEY_SPECIAL); break;
					case SDLK_k: mk_down_x[0] = -10; break;
					case SDLK_p: mk_down_x[0] = 10; break;
					case SDLK_9: mk_down_y[0] = -10; break;
					case SDLK_l: mk_down_y[0] = 10; break;
					case SDLK_o: st->local_players[0]->hud->eventClick(); st->local_players[0]->p->keyPress(Player::KEY_FIRE); break;
					case SDLK_i: st->local_players[0]->p->keyPress(Player::KEY_MELEE); break;
					case SDLK_j: st->local_players[0]->hud->eventUp(); break;
					case SDLK_m: st->local_players[0]->hud->eventDown(); break;
					default: break;
				}
			
			} else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
					case SDLK_w: st->local_players[0]->p->keyRelease(Player::KEY_UP); break;
					case SDLK_a: st->local_players[0]->p->keyRelease(Player::KEY_LEFT); break;
					case SDLK_s: st->local_players[0]->p->keyRelease(Player::KEY_DOWN); break;
					case SDLK_d: st->local_players[0]->p->keyRelease(Player::KEY_RIGHT); break;
					case SDLK_e: st->local_players[0]->p->keyRelease(Player::KEY_USE); break;
					case SDLK_q: st->local_players[0]->p->keyRelease(Player::KEY_LIFT); break;
					case SDLK_t: st->local_players[0]->p->keyRelease(Player::KEY_SPECIAL); break;
					case SDLK_k: mk_down_x[0] = 0; break;
					case SDLK_p: mk_down_x[0] = 0; break;
					case SDLK_9: mk_down_y[0] = 0; break;
					case SDLK_l: mk_down_y[0] = 0; break;
					case SDLK_o: st->local_players[0]->p->keyRelease(Player::KEY_FIRE); break;
					case SDLK_i: st->local_players[0]->p->keyRelease(Player::KEY_MELEE); break;
					default: break;
				}
			}

		} // Two players, player one


		// Two players, player two
		// Keypad: Move = 8, 4, 5, 6; Action = 9; Lift = 7
		if (st->num_local == 2 && st->local_players[1]->p != NULL) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_KP8: st->local_players[1]->p->keyPress(Player::KEY_UP); break;
					case SDLK_KP4: st->local_players[1]->p->keyPress(Player::KEY_LEFT); break;
					case SDLK_KP5: st->local_players[1]->p->keyPress(Player::KEY_DOWN); break;
					case SDLK_KP6: st->local_players[1]->p->keyPress(Player::KEY_RIGHT); break;
					case SDLK_KP9: st->local_players[1]->p->keyPress(Player::KEY_USE); break;
					case SDLK_KP7: st->local_players[1]->p->keyPress(Player::KEY_LIFT); break;
					case SDLK_KP1: st->local_players[1]->p->keyPress(Player::KEY_SPECIAL); break;
					default: break;
				}
			
			} else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
					case SDLK_KP8: st->local_players[1]->p->keyRelease(Player::KEY_UP); break;
					case SDLK_KP4: st->local_players[1]->p->keyRelease(Player::KEY_LEFT); break;
					case SDLK_KP5: st->local_players[1]->p->keyRelease(Player::KEY_DOWN); break;
					case SDLK_KP6: st->local_players[1]->p->keyRelease(Player::KEY_RIGHT); break;
					case SDLK_KP9: st->local_players[1]->p->keyRelease(Player::KEY_USE); break;
					case SDLK_KP7: st->local_players[1]->p->keyRelease(Player::KEY_LIFT); break;
					case SDLK_KP1: st->local_players[1]->p->keyRelease(Player::KEY_SPECIAL); break;
					default: break;
				}
			
			} else if (event.type == SDL_MOUSEMOTION) {
				game_x[1] += event.motion.x - 400;
				net_x[1] += event.motion.x - 400;
				game_y[1] += event.motion.y - 300;
				net_y[1] += event.motion.y - 300;
			
			} else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				st->local_players[1]->hud->eventClick();
				st->local_players[1]->p->keyPress(Player::KEY_FIRE);
			
			} else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
				st->local_players[1]->p->keyRelease(Player::KEY_FIRE);

			} else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
				st->local_players[1]->p->keyPress(Player::KEY_MELEE);
			
			} else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {
				st->local_players[1]->p->keyRelease(Player::KEY_MELEE);

			} else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_WHEELUP) {
				st->local_players[1]->hud->eventUp();
			
			} else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_WHEELDOWN) {
				st->local_players[1]->hud->eventDown();

			}

		} // Two players, player two


	} // end while


	// Process simulated mouse movements
	if (st->num_local == 2 && st->local_players[0]->p != NULL) {
		game_x[0] += mk_down_x[0];
		net_x[0] += mk_down_x[0];
		game_y[0] += mk_down_y[0];
		net_y[0] += mk_down_y[0];
	}
}


