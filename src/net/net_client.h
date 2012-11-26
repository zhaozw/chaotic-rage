// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <list>
#include "../rage.h"
#include "net.h"

using namespace std;



class NetClient {
	friend class NetClientSeqPred;
	
	private:
		GameState * st;
		IPaddress ipaddress;
		Uint16 code;
		UDPsocket sock;
		list<NetMsg> messages;
		SeqNum seq;			// TODO: is this big enough?
		
		NetClientSeqPred * seq_pred;
		
	public:
		NetClient(GameState * st);
		~NetClient();
		
	public:
		void update();
		bool attemptJoinGame(string address, int port);

	public:
		// One method for each outgoing network message the client sends out
		void addmsgInfoReq();
		void addmsgJoinReq();
		void addmsgJoinAck();
		void addmsgChat();
		void addmsgKeyMouseStatus(int x, int y, int delta, Uint8 k);
		void addmsgQuit();
		
	public:
		// One method for each incoming network message from the server
		unsigned int handleInfoResp(Uint8 *data, unsigned int size);
		unsigned int handleJoinAcc(Uint8 *data, unsigned int size);
		unsigned int handleJoinRej(Uint8 *data, unsigned int size);
		unsigned int handleDataCompl(Uint8 *data, unsigned int size);
		unsigned int handleChat(Uint8 *data, unsigned int size);
		unsigned int handleUnitAdd(Uint8 *data, unsigned int size);
		unsigned int handleUnitUpdate(Uint8 *data, unsigned int size);
		unsigned int handleUnitRem(Uint8 *data, unsigned int size);
		unsigned int handleWallUpdate(Uint8 *data, unsigned int size);
		unsigned int handleWallRem(Uint8 *data, unsigned int size);
		unsigned int handlePlayerDrop(Uint8 *data, unsigned int size);
		unsigned int handlePlayerQuit(Uint8 *data, unsigned int size);

	private:
		void bind(string address, int port);
};

class NetClientSeqPred
{
	public:
		NetClient *client;
		bool operator() (const NetMsg& value) { return (this->client->seq > value.seq); }
		NetClientSeqPred(NetClient *c) { this->client = c; }
};



