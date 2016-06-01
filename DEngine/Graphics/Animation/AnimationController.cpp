// Engine include
#include "AnimationController.h"
#include "BlendTree.h"
#include "Math\SQT.h"
#include "Math\simdmath.h"
#include "GameObject\GameObject.h"
#include "Event\EventQueue.h"
#include "Event\EngineEvent.h"

// C++ include
#include <stdio.h>
#include <windows.h>

#define C_STR(string, text)\
	(string + text).c_str()

namespace DE
{

AnimationController::AnimationController(Skeleton* skeleton) : m_skeleton(skeleton)
{
	m_ID = ComponentID;
	m_bPlaying = false;
}

void AnimationController::addAnimationSet(const char* set_name, AnimationSet* animationSet)
{
	m_animationSets.Add(set_name, animationSet);
}

void AnimationController::CreateAnimationSets(const char* fileName)
{
	std::string sFileName(fileName);
	sFileName = "../Assets/" + sFileName;
	FILE* pFile = fopen(C_STR(sFileName, "_animation.bufa"), "r");
	char c[256], clipName[256];
	int iNumJoints, iNumFrames;
	float quat[4], trans[3];
	float scale;
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%s", &clipName);
	fscanf(pFile, "%i", &iNumJoints);
	fscanf(pFile, "%i", &iNumFrames);
	AnimationSet* animSet = new AnimationSet(iNumFrames * 1.0f / 30.0f, iNumJoints);
	for (int i = 0; i < iNumJoints; ++i)
	{
		fscanf(pFile, "%s", &c);
		Animation* anim = new Animation(iNumFrames);
		for (int j = 0; j < iNumFrames; ++j)
		{
			fscanf(pFile, "%f %f %f %f", &quat[0], &quat[1], &quat[2], &quat[3]);
			fscanf(pFile, "%f %f %f", &trans[0], &trans[1], &trans[2]);
			fscanf(pFile, "%f", &scale);
			Quaternion q(quat);
			Vector3 t(trans[0], trans[1], trans[2]);
			SQT sqt(q, t, scale);
			anim->AddPose(sqt);
		}

		animSet->AddAnimation(anim);
	}
	addAnimationSet(clipName, animSet);
	fclose(pFile);
}

AnimationSet* AnimationController::getAnimationSet(const char* set_name)
{
	if (!m_animationSets.Contain(set_name)) 
	{
		return nullptr;
	}
	else 
	{
		return m_animationSets[set_name];
	}
}

void AnimationController::setActiveAnimationSet(const char* set_name, const bool active)
{
	AnimationSet* animationSet = getAnimationSet(set_name);

	if (animationSet) {
		animationSet->setActive(active);
		animationSet->reset();
	}
}

void AnimationController::Update(float deltaTime)
{
	m_bPlaying = false;
	m_animationSets.ForEachItem([deltaTime](AnimationSet* item)
	{
		item->update(deltaTime);
	});

	if (m_pASM->IsInTransition())
	{
		*m_skeleton->m_vGlobalPose[0] = SQT::LerpSQT(GetPoseFromState(m_pASM->m_pPrevState, 0), GetPoseFromState(m_pASM->m_pCurrState, 0), m_pASM->m_fBlendValue).Matrix();
		*m_skeleton->m_vWorldGlobalPose[0] = *m_pOwner->GetTransform() * *m_skeleton->m_vGlobalPose[0];
		const unsigned int size = m_skeleton->GetJointsCount();
		for (int i = 1; i < size; ++i)
		{
			const Joint& currJoint = m_skeleton->m_vJoints[i];
			*m_skeleton->m_vGlobalPose[i] = *m_skeleton->m_vGlobalPose[currJoint.m_iParent] * SQT::LerpSQT(GetPoseFromState(m_pASM->m_pPrevState, i), GetPoseFromState(m_pASM->m_pCurrState, i), m_pASM->m_fBlendValue).Matrix();
			*m_skeleton->m_vWorldGlobalPose[i] = *m_pOwner->GetTransform() * *m_skeleton->m_vGlobalPose[i];
		}
		m_bPlaying = true;
	}
	// Play animation without transition
	else
	{
		if (IsStateAnimationSetActive(m_pASM->m_pCurrState))
		{
			*m_skeleton->m_vGlobalPose[0] = GetPoseFromState(m_pASM->m_pCurrState, 0).Matrix();
			*m_skeleton->m_vWorldGlobalPose[0] = *m_pOwner->GetTransform() * *m_skeleton->m_vGlobalPose[0];
			const unsigned int size = m_skeleton->GetJointsCount();
			for (int i = 1; i < size; ++i)
			{
				const Joint& currJoint = m_skeleton->m_vJoints[i];
				*m_skeleton->m_vGlobalPose[i] = *m_skeleton->m_vGlobalPose[currJoint.m_iParent] * GetPoseFromState(m_pASM->m_pCurrState, i).Matrix();
				*m_skeleton->m_vWorldGlobalPose[i] = *m_pOwner->GetTransform() * *m_skeleton->m_vGlobalPose[i];
			}
			m_bPlaying = true;
		}
	}

	if (!m_bPlaying)
	{
		Handle hEvt(sizeof(Animation_END_Event));
		new (hEvt) Animation_END_Event;
		EventQueue::GetInstance()->Add(hEvt, GAME_EVENT);
	}
}

SQT AnimationController::GetPoseFromState(AnimationStateMachine::State* pState, const int jointIndex)
{
	if (pState->m_bUseBlendTree)
	{
		return GetPoseFromBlendTree(pState->m_BlendTree, jointIndex);
	}
	else
	{
		return GetPoseFromSingleSet(m_animationSets[(pState->m_sClipName)], jointIndex);
	}
}

SQT AnimationController::GetPoseFromSingleSet(AnimationSet * set, const int jointIndex)
{
	return set->m_vAnimations[jointIndex]->GetCurrentPose();
}

SQT AnimationController::GetPoseFromBlendTree(BlendTree * btree, const int jointIndex)
{
	int numClips = btree->m_vClipnames.Size();
	float factor = btree->m_fBlendFactor;
	AnimationSet* fromSet = nullptr;
	AnimationSet* toSet = nullptr;

	// Special case: blend factor larger than predefined range
	if (factor >= btree->m_vWeightings[numClips - 1])
	{
		fromSet = getAnimationSet(btree->m_vClipnames[numClips - 1]);
		toSet = getAnimationSet(btree->m_vClipnames[0]);
		factor = factor - btree->m_vWeightings[numClips - 1];
	}
	// Reduce many clips to binary lerp
	else
	{
		for (int i = 0; i < numClips; ++i)
		{
			if (factor < btree->m_vWeightings[i])
			{
				fromSet = getAnimationSet(btree->m_vClipnames[i - 1]);
				toSet = getAnimationSet(btree->m_vClipnames[i]);
				factor = factor - btree->m_vWeightings[i - 1];
				break;
			}
		}
	}
	fromSet->setActive(true);
	toSet->setActive(true);
	assert(fromSet != nullptr && toSet != nullptr);
	return SQT::LerpSQT(fromSet->m_vAnimations[jointIndex]->GetCurrentPose(), toSet->m_vAnimations[jointIndex]->GetCurrentPose(), factor);
}

bool AnimationController::IsStateAnimationSetActive(AnimationStateMachine::State * pState)
{
	if (pState->m_bUseBlendTree)
	{
		for (int i = 0; i < pState->m_BlendTree->m_vClipnames.Size(); ++i)
		{
			if (!getAnimationSet(pState->m_BlendTree->m_vClipnames[i])->isActive())
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return getAnimationSet(pState->m_sClipName)->isActive();
	}
}

AnimationController::~AnimationController()
{
}

};