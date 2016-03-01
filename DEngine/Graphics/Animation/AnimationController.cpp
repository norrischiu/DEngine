// Engine include
#include "AnimationController.h"
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

using namespace DE;

AnimationController::AnimationController(Skeleton* skeleton) : m_skeleton(skeleton)
{
	m_ID = ComponentID;
	m_bPlaying = false;
}

void AnimationController::addAnimationSet(std::string name, const AnimationSet& animationSet, const BlendMode blendMode)
{
	m_animationSets[name] = animationSet;
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
		Animation* anim = new Animation(c);
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
		animSet->m_vAnimations.push_back(anim);
	}
	addAnimationSet(clipName, *animSet);
	fclose(pFile);
}

void AnimationController::removeAnimationSet(const char* name)
{
	m_animationSets.erase(name);
}

std::unordered_map<std::string, AnimationSet>* AnimationController::getAnimationSets()
{
	return &m_animationSets;
}

void AnimationController::setAnimationSet(const std::unordered_map<std::string, AnimationSet>& animationSets)
{
	m_animationSets = animationSets;
}

AnimationSet* AnimationController::getAnimationSet(const char* name)
{
	auto t = m_animationSets.find(name);

	if (t == m_animationSets.end()) {
		return nullptr;
	}
	else {
		return &t->second;
	}

	return nullptr;
}

Animation* AnimationController::getAnimation(const char* set_name, const char* animaton_name)
{
	AnimationSet* animationSet = getAnimationSet(set_name);

	if (animationSet) {
		return animationSet->getAnimation(animaton_name);
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

bool AnimationController::isAnimationSetActive(const char* name)
{
	return getAnimationSet(name)->isActive();
}

void AnimationController::setActiveAnimationSet(const char* name, const bool active)
{
	AnimationSet* animationSet = getAnimationSet(name);

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

int AnimationController::getNumAnimations(const char* name)
{
	AnimationSet* animationSet = getAnimationSet(name);

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

	for (std::unordered_map<std::string, Transition>::iterator itr_transition = m_transition.begin(); itr_transition != m_transition.end();)
	{
		AnimationSet* fromClip = getAnimationSet(itr_transition->second.fromClip);
		AnimationSet* toClip = getAnimationSet(itr_transition->second.toClip);

		if (fromClip && fromClip->isActive() && toClip && toClip->isActive())
		{
			transitionClip.push_back(itr_transition->second.fromClip);
			transitionClip.push_back(itr_transition->second.toClip);

			fromClip->update(deltaTime);
			toClip->update(deltaTime);

			const float numerator = itr_transition->second.accuTime;
			const float denominator = itr_transition->second.duration;

			const float interpolant = numerator / denominator;

			static wchar_t s[256];
			swprintf(s, sizeof(s), L"numerator: %f, denominator: %f, interpolant: %f\n", numerator, denominator, interpolant);
			OutputDebugStringW(s);

			m_skeleton->m_vGlobalPose[0] = SQT::LerpSQT(fromClip->m_vAnimations[0]->GetCurrentPose(deltaTime), toClip->m_vAnimations[0]->GetCurrentPose(deltaTime), interpolant).Matrix();

			for (int i = 1; i < m_skeleton->m_vJoints.size(); ++i)
			{
				Joint* currJoint = m_skeleton->m_vJoints[i];
				m_skeleton->m_vGlobalPose[i] = m_skeleton->m_vGlobalPose[currJoint->m_iParent] * SQT::LerpSQT(fromClip->m_vAnimations[i]->GetCurrentPose(deltaTime), toClip->m_vAnimations[i]->GetCurrentPose(deltaTime), interpolant).Matrix();
			}

			itr_transition->second.accuTime += deltaTime;
			if (itr_transition->second.accuTime > itr_transition->second.duration)
			{
				itr_transition = m_transition.erase(itr_transition);
				fromClip->reset();
			}
			else
			{
				itr_transition++;
			}

			m_bPlaying = true;
		}
		else
		{
			itr_transition++;
		}
	}

	for (auto itr_clip : m_animationSets)
	{
		if (itr_clip.second.isActive() && std::find(transitionClip.begin(), transitionClip.end(), itr_clip.first) == transitionClip.end())
		{
			AnimationSet* animationSet = &m_animationSets[itr_clip.first];
			if (animationSet->isActive())
			{
				animationSet->update(deltaTime);		//update delta time first, then animate

				m_skeleton->m_vGlobalPose[0] = animationSet->m_vAnimations[0]->GetCurrentPose(deltaTime).Matrix();
				for (int i = 1; i < m_skeleton->m_vJoints.size(); ++i)
				{
					Joint* currJoint = m_skeleton->m_vJoints[i];
					m_skeleton->m_vGlobalPose[i] = m_skeleton->m_vGlobalPose[currJoint->m_iParent] * animationSet->m_vAnimations[i]->GetCurrentPose(deltaTime).Matrix();
				}
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

AnimationController::~AnimationController()
{
}
