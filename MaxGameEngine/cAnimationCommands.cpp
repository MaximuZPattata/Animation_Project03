#include "cAnimationCommands.h"

void cAnimationCommands::AddAnimation(std::string meshName, std::string animName, float frameRateIncrement, float animationDuration, float ticksPerSecond,
	cMesh* meshModel, sModelDrawInfo* modelInfo, AnimationInfo& animationToBePlayed, bool loopAnimation)
{
	sAnimationCommandInfoInQueue* newAnimationInfo = new sAnimationCommandInfoInQueue();
	
	newAnimationInfo->mAnimationName = animName;
	newAnimationInfo->mModelName = meshName;
	newAnimationInfo->mIncrementFrameRate = frameRateIncrement;
	newAnimationInfo->mTotalAnimationDuration = animationDuration;
	newAnimationInfo->mMeshModel = meshModel;
	newAnimationInfo->mModelInfo = modelInfo;
	newAnimationInfo->mAnimationToBePlayed = animationToBePlayed;
	newAnimationInfo->bAnimationLooped = loopAnimation;
	newAnimationInfo->mTicksPerSecond = ticksPerSecond;

	//----------------Grouping all animations for same mesh in one list-----------------------------
	
	for (sMeshWithAnimations* currentMeshList : mMeshWithAnimationsList)
	{
		if (currentMeshList->mMeshName == meshName)
		{
			currentMeshList->mAnimationInfoInQueueList.push_back(newAnimationInfo);

			return;
		}
	}

	//--------------Creating new mesh animation list if not already exist---------------------------

	sMeshWithAnimations* newMeshListWithAnimation = new sMeshWithAnimations();

	newMeshListWithAnimation->mMeshName = meshName;
	newMeshListWithAnimation->mAnimationInfoInQueueList.push_back(newAnimationInfo);

	this->mMeshWithAnimationsList.push_back(newMeshListWithAnimation);
}

// Function releases all previous animation commands under this mesh name from the list
void cAnimationCommands::ReleasePreviousAnimation(std::string meshName)
{
	sMeshWithAnimations* currentMeshList = this->FindMeshAnimationList(meshName);

	if (currentMeshList != NULL)
	{
		for (sAnimationCommandInfoInQueue* currentCommand : currentMeshList->mAnimationInfoInQueueList)
			delete currentCommand;

		currentMeshList->mAnimationInfoInQueueList.clear();
	}
}

// Function deletes this specific animation command under this mesh name from the list
void cAnimationCommands::DeleteAnimation(std::string meshName, sAnimationCommandInfoInQueue* animationInfoInQueue)
{
	sMeshWithAnimations* currentMeshList = this->FindMeshAnimationList(meshName);
	
	if (currentMeshList == NULL)
	{
		std::cout << "ERROR : COULDN'T FIND MESH ANIMATION LIST FOR THIS MESH NAME : " << meshName << std::endl;

		return;
	}
	
	currentMeshList->mAnimationInfoInQueueList.erase(std::remove(currentMeshList->mAnimationInfoInQueueList.begin(), currentMeshList->mAnimationInfoInQueueList.end(), 
		animationInfoInQueue), currentMeshList->mAnimationInfoInQueueList.end());

	delete animationInfoInQueue;
}

// Function finds the animation list for this mesh name
sMeshWithAnimations* cAnimationCommands::FindMeshAnimationList(std::string meshName)
{
	for (sMeshWithAnimations* currentMeshList : this->mMeshWithAnimationsList)
	{
		if (currentMeshList->mMeshName == meshName)
			return currentMeshList;
	}

	return NULL;
}

// Function to unloop an existing animation command
void cAnimationCommands::UnloopAllAnimationCommands(std::string meshName)
{
	sMeshWithAnimations* currentMeshList = this->FindMeshAnimationList(meshName);

	if (currentMeshList == NULL)
	{
		std::cout << "ERROR : COULDN'T FIND MESH ANIMATION LIST FOR THIS MESH NAME : " << meshName << std::endl;

		return;
	}

	for (sAnimationCommandInfoInQueue* currentCommand : currentMeshList->mAnimationInfoInQueueList)
	{
		if(currentCommand->bAnimationLooped)
			currentCommand->bAnimationLooped = false;
	}
}

void cAnimationCommands::LoopAnimationCommand(std::string meshName, std::string animName)
{
	sMeshWithAnimations* currentMeshList = this->FindMeshAnimationList(meshName);

	if (currentMeshList == NULL)
	{
		std::cout << "ERROR : COULDN'T FIND MESH ANIMATION LIST FOR THIS MESH NAME : " << meshName << std::endl;

		return;
	}

	for (sAnimationCommandInfoInQueue* currentCommand : currentMeshList->mAnimationInfoInQueueList)
	{
		if (currentCommand->mAnimationName == animName)
		{
			if (!currentCommand->bAnimationLooped)
			{
				currentCommand->bAnimationLooped = true;

				return;
			}
		}
	}

	std::cout << "ERROR : COULDN'T FIND ANIMATION NAME TO LOOP IN LIST : " << animName << std::endl;
}