#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "sModelDrawInfo.h"
#include "cMesh.h"

struct sAnimationCommandInfoInQueue
{
	std::string mModelName;
	std::string mAnimationName;
	
	float mTotalAnimationDuration = 0.f;
	float mIncrementFrameRate = 0.f;
	float mAnimationTime = 0.f;
	float mTicksPerSecond = 0.f;

	bool bAnimationCompleted = false;
	bool bAnimationLooped = false;

	cMesh* mMeshModel;
	sModelDrawInfo* mModelInfo;
	AnimationInfo mAnimationToBePlayed;
};

struct sMeshWithAnimations
{
	std::string mMeshName;

	std::vector <sAnimationCommandInfoInQueue*> mAnimationInfoInQueueList;
};

class cAnimationCommands
{
public:
	std::vector <sMeshWithAnimations*> mMeshWithAnimationsList;

	void AddAnimation(std::string meshName, std::string animationName, float frameRateIncrement, float animationDuration, float ticksPerSecond,
		cMesh* meshModel, sModelDrawInfo* modelInfo, AnimationInfo& animationToBePlayed, bool loopAnimation);
	void DeleteAnimation(std::string meshName, sAnimationCommandInfoInQueue* animationCommandInfoInQueue);
	void ReleasePreviousAnimation(std::string meshName);
	void UnloopAllAnimationCommands(std::string meshName);
	void LoopAnimationCommand(std::string meshName, std::string animName);

private:
	sMeshWithAnimations* FindMeshAnimationList(std::string meshName);
};

