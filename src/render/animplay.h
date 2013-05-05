// This file is part of Chaotic Rage (c) 2010 Josh Heidenreich
//
// kate: tab-width 4; indent-width 4; space-indent off; word-wrap off;

#pragma once
#include <iostream>
#include <SDL.h>
#include <map>
#include <glm/glm.hpp>
#include "../rage.h"
#include "assimpmodel.h"

using namespace std;


/**
* Represents no animation, just a single frame
**/
#define ANIMATION_NONE 255


/**
* Stores info about a single animation which is currently running
* Also used for static meshes.
**/
class AnimPlay
{
	friend class RenderOpenGL;

	private:
		GameState* st;
		AssimpModel* model;

		// Currently running animation
		AssimpAnimation* anim;
		unsigned int start_time;
		unsigned int start_frame;
		unsigned int end_frame;
		bool loop;

		// Current "move node"
		AssimpNode* move;
		glm::mat4 move_transform;

		// These are calculated by calcTransforms()
		// and then fed into the renderer
		map<AssimpNode*, glm::mat4> transforms;
		
		// Calculated bone transforms
		vector<glm::mat4> bone_transforms;
		
	public:
		AnimPlay(AssimpModel* model);

		void setAnimation(unsigned int animation, unsigned int start_frame, unsigned int end_frame, bool loop);
		void setAnimation(unsigned int animation, unsigned int start_frame, unsigned int end_frame);
		void setAnimation(unsigned int animation);
		void clearAnimation();
		
		void setMoveNode(string node);
		void clearMoveNode();

		AssimpModel* getModel() { return this->model; }
		bool isDynamic();

		void setMoveTransform(glm::mat4 transform);
		void calcTransformsStatic();
		void calcTransforms();
		void calcBoneTransforms();

	private:
		void calcTransformNodeStatic(AssimpNode* nd, glm::mat4 transform);
		void calcTransformNode(AssimpNode* nd, glm::mat4 transform, float animTick);
		unsigned int findFrameTime(vector<AssimpAnimKey>* keys, float animTick);
		float mixFactor(vector<AssimpAnimKey>* keys, unsigned int index, float animTick);
		
};
