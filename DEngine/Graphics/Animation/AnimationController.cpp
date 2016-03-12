// Engine include
#include "AnimationController.h"
#include "Math\SQT.h"
#include "Math\simdmath.h"
#include "GameObject\GameObject.h"
#include "Event\EventQueue.h"
#include "Event\EngineEvent.h"
#include "../HUD/HUD.h"

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
	m_animationSets[set_name] = animationSet;
}

void AnimationController::CreateAnimationSets(const char* fileName)
{
	std::string sFileName(fileName);
	sFileName = "../Assets/" + sFileName;
	FILE* pFile = fopen(C_STR(sFileName, "_animation.bufa"), "r");
	char c[256], clipName[256];
	int iNumJoints, iNumFrames;
	float quat[4], trans[3]; float transform[16];
	float scale;
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%s", &clipName);
	fscanf(pFile, "%i", &iNumJoints);
	fscanf(pFile, "%i", &iNumFrames);
	AnimationSet* animSet = new AnimationSet(0.0f, iNumFrames * 1.0f / 30.0f);
	for (int i = 0; i < iNumJoints; ++i)
	{
		fscanf(pFile, "%s", &c);
		Animation* anim = new Animation;
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

void AnimationController::removeAnimationSet(const char* set_name)
{
	m_animationSets.erase(set_name);
}

std::unordered_map<std::string, AnimationSet*>* AnimationController::getAnimationSets()
{
	return &m_animationSets;
}

void AnimationController::setAnimationSet(const std::unordered_map<std::string, AnimationSet*>& animationSets)
{
	m_animationSets = animationSets;
}

AnimationSet* AnimationController::getAnimationSet(const char* set_name)
{
	auto t = m_animationSets.find(set_name);

	if (t == m_animationSets.end()) {
		return nullptr;
	}
	else {
		return t->second;
	}

	return nullptr;
}

Skeleton* AnimationController::getSkeleton()
{
	return m_skeleton;
}

void AnimationController::setSkeleton(Skeleton* skeleton)
{
	m_skeleton = skeleton;
}

bool AnimationController::isAnimationSetActive(const char* set_name)
{
	return getAnimationSet(set_name)->isActive();
}

void AnimationController::setActiveAnimationSet(const char* set_name, const bool active)
{
	AnimationSet* animationSet = getAnimationSet(set_name);

	if (animationSet) {
		animationSet->setActive(active);
		animationSet->reset();
	}
}

bool AnimationController::triggerAnimation(const char* set_name, const float currTime)
{
	AnimationSet* animationSet = getAnimationSet(set_name);
	Animation* animation = nullptr;

	if (animationSet && !animationSet->isActive()) {
		animationSet->setActive(true);
		animationSet->setCurrTime(currTime);
	}

	return false;
}

int AnimationController::getNumAnimationSets() const
{
	return m_animationSets.size();
}

int AnimationController::getNumAnimations(const char* set_name)
{
	AnimationSet* animationSet = getAnimationSet(set_name);

	/*
	if (animationSet) {
	return animationSet->getNumAnimations();
	}

	return -1;
	*/

	return animationSet->getNumAnimations();
}

void AnimationController::setBlending(const char* fromClip, const char* toClip, const BlendMode blendMode, const float duration)
{
	for (std::unordered_map<std::string, Transition>::iterator itr = m_transition.begin(); itr != m_transition.end();)
	{
		if (strcmp(itr->second.fromClip, fromClip) == 0 ||
			strcmp(itr->second.fromClip, toClip) == 0 ||
			strcmp(itr->second.toClip, fromClip) == 0 ||
			strcmp(itr->second.toClip, toClip) == 0
		) {
			itr = m_transition.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	m_transition.insert({ (std::string(fromClip) + '#' + std::string(toClip)), Transition(fromClip, toClip, blendMode, duration) });
}

void AnimationController::Update(float deltaTime)
{
	std::vector<std::string> transitionClip;
	m_bPlaying = false;
	int num = 0;

	for (std::unordered_map<std::string, Transition>::iterator itr_transition = m_transition.begin(); itr_transition != m_transition.end();)
	{
		AnimationSet* fromClip = getAnimationSet(itr_transition->second.fromClip);
		AnimationSet* toClip = getAnimationSet(itr_transition->second.toClip);

		if (fromClip && toClip && fromClip->isActive() && toClip->isActive())
		{
			switch (itr_transition->second.blendMode)
			{
				case BlendMode::FROZEN_BLENDING:
				case BlendMode::CROSS_FADE_BLENDING:
					transitionClip.push_back(itr_transition->second.fromClip);
					transitionClip.push_back(itr_transition->second.toClip);

					fromClip->update(deltaTime);
					toClip->update(deltaTime);

					const float interpolant = itr_transition->second.accuTime / itr_transition->second.duration;

//					((TextBox*) HUD::getInstance()->getHUDElementById("debug1"))->setText("from %s to %s, interpolant: %f\n", itr_transition->second.fromClip, itr_transition->second.toClip, interpolant);
					*m_skeleton->m_vGlobalPose[0] = SQT::LerpSQT(fromClip->m_vAnimations[0]->GetCurrentPose(deltaTime), toClip->m_vAnimations[0]->GetCurrentPose(deltaTime), interpolant).Matrix();
					*m_skeleton->m_vWorldGlobalPose[0] = *m_pOwner->GetTransform() * *m_skeleton->m_vGlobalPose[0];

					for (int i = 1; i < m_skeleton->m_vJoints.size(); ++i)
					{
						const Joint& currJoint = m_skeleton->m_vJoints[i];
						*m_skeleton->m_vGlobalPose[i] = *m_skeleton->m_vGlobalPose[currJoint.m_iParent] * SQT::LerpSQT(fromClip->m_vAnimations[i]->GetCurrentPose(deltaTime), toClip->m_vAnimations[i]->GetCurrentPose(deltaTime), interpolant).Matrix();
						*m_skeleton->m_vWorldGlobalPose[i] = *m_pOwner->GetTransform() * *m_skeleton->m_vGlobalPose[i];
					}

					itr_transition->second.accuTime += deltaTime;
					if (itr_transition->second.accuTime > itr_transition->second.duration)
					{
						setActiveAnimationSet(itr_transition->second.fromClip, false);
						itr_transition = m_transition.erase(itr_transition);
					}
					else
					{
						itr_transition++;
					}
					break;
			}

			num++;
			m_bPlaying = true;
		}
		else
		{
			itr_transition++;
		}
	}

	for (auto itr_clip : m_animationSets)
	{
		if (itr_clip.second->isActive() && std::find(transitionClip.begin(), transitionClip.end(), itr_clip.first) == transitionClip.end())
		{
			AnimationSet* animationSet = m_animationSets[itr_clip.first];
			if (animationSet->isActive())
			{
				animationSet->update(deltaTime);		//update delta time first, then animate

				*m_skeleton->m_vGlobalPose[0] = animationSet->m_vAnimations[0]->GetCurrentPose(deltaTime).Matrix();
				*m_skeleton->m_vWorldGlobalPose[0] = *m_pOwner->GetTransform() * *m_skeleton->m_vGlobalPose[0];

//				((TextBox*) HUD::getInstance()->getHUDElementById("debug1"))->setText("%s\n", itr_clip.first.c_str());

				for (int i = 1; i < m_skeleton->m_vJoints.size(); ++i)
				{
					const Joint& currJoint = m_skeleton->m_vJoints[i];
					*m_skeleton->m_vGlobalPose[i] = *m_skeleton->m_vGlobalPose[currJoint.m_iParent] * animationSet->m_vAnimations[i]->GetCurrentPose(deltaTime).Matrix();
					*m_skeleton->m_vWorldGlobalPose[i] = *m_pOwner->GetTransform() * *m_skeleton->m_vGlobalPose[i];
				}
			}

				num++;
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

AnimationController::~AnimationController()
{
	for (auto itr = m_animationSets.begin(); itr != m_animationSets.end();)
	{
		if (itr->second)
		{
			delete itr->second;
			itr = m_animationSets.erase(itr);
		}
		else {
			itr++;
		}
	}

	m_animationSets.clear();
}

};