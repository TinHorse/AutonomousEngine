#pragma once

struct Animation
{
	int index;
	int numFrames;
	int animationSpeed;
	Animation() = default;
	Animation(int ind, int nFrames, int mSpeed)
	{
		index = ind;
		numFrames = nFrames;
		animationSpeed = mSpeed;
	}
};