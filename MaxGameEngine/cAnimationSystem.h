#pragma once
#include <string>
#include <iostream>
#include "cMesh.h"
#include "sModelDrawInfo.h"
#include "cAnimationCommands.h"

class cAnimationSystem
{
public:
	cAnimationSystem();
	~cAnimationSystem();

	void RunBoneAnimation(cMesh* animMesh, AnimationInfo& animationDetails, Node& nodeAnim, float deltaTime);
	void UpdateAnimations(float deltaTime);
	bool AddAnimationCommand(cMesh* animMesh, sModelDrawInfo* animModel, std::string animationName, float frameRateIncrement, bool loopAnimation, bool releasePrevAnimations);
	void UnloopAnimationCommands(std::string meshnName);
	void LoopAnimation(std::string meshName, std::string animationName);

private:
	cAnimationCommands* mAnimationCommandsManager = NULL;

	glm::mat4 AnimationTransformations(NodeAnimation* nodeAnim, float dt);
	float FindValueAfterEasingEffect(double startKeyFrameTime, double endKeyFrameTime, EasingType easeType, float dt);
};
