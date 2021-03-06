// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#pragma once
#include "../rage.h"

using namespace std;


enum HUDLabelAlign {
	ALIGN_LEFT = 1,
	ALIGN_CENTER = 2,
	ALIGN_RIGHT = 3
};


class HUDLabel
{
	public:
		float x;
		float y;
		float width;
		HUDLabelAlign align;
		string data;
		bool visible;
		float r;
		float g;
		float b;
		float a;

	public:
		HUDLabel(float x, float y, string data)
			: x(x), y(y), width(0.0f), align(ALIGN_LEFT), data(data), visible(true),
			  r(1.0f), g(1.0f), b(1.0f), a(1.0f)
			{};
};
