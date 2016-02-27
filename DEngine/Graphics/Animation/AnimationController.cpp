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

void AnimationController::addAnimationSet(const std::string name, const AnimationSet& animationSet, const BlendMode blendMode)
{
	m_animationSets[name] = animationSet;

	if (m_blending.find(blendMode)==m_blending.end()) {
		std::vector<std::vector<std::string>> vec2;
		std::vector<std::string> vec1;
		vec1.push_back(name);
		vec2.push_back(vec1);
		m_blending[blendMode] = vec2;
	}
	else {
		switch (blendMode)
		{
			case ADDICTIVE_BLENDING:
				m_blending[blendMode][0].push_back(name);
				break;
			case CROSS_FADE_BLENDING:
			case FROZEN_BLENDING:
				for (auto itr : m_blending[blendMode])
				{
					if (itr.size() < 2) {
						itr.push_back(name);
					}
				}
				break;
		}
	}
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
	AnimationSet* animSet = new AnimationSet(0, iNumFrames * 1.0f / 30.0f);
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

void AnimationController::removeAnimationSet(const std::string name)
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

AnimationSet* AnimationController::getAnimationSet(const std::string name)
{
	auto t = m_animationSets.find(name);

	if (t == m_animationSets.end()) {
		return nullptr;
	} else {
		return &t->second;
	}
	
	return nullptr;
}

Animation* AnimationController::getAnimation(const std::string set_name, const std::string animaton_name)
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

void AnimationController::setSkeleton(Skeleton* const skeleton)
{
	m_skeleton = skeleton;
}

bool AnimationController::isAnimationSetActive(const std::string name)
{
	return getAnimationSet(name)->isActive();
}

void AnimationController::setActiveAnimationSet(const std::string name, const bool active)
{
	AnimationSet* animationSet = getAnimationSet(name);

	if (animationSet) {
		animationSet->setActive(active);
	}
}

bool AnimationController::triggerAnimation(const std::string set_name, const float currTime)
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

int AnimationController::getNumAnimations(const std::string name)
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

void AnimationController::setBlending(const std::vector<std::string> clipNames, const BlendMode blendMode)
{
	for (std::unordered_map<BlendMode, std::vector<std::vector<std::string>>>::iterator itr_blending = m_blending.begin(); itr_blending != m_blending.end(); ++itr_blending)
	{
		switch (itr_blending->first)
		{
			case BlendMode::ADDICTIVE_BLENDING:
				for (auto clipName : clipNames)
				{
					if (m_animationSets.find(clipName) != m_animationSets.end())
					{
						for (std::vector<std::string>::iterator itr_clipName_vec1 = itr_blending->second[0].begin(); itr_clipName_vec1 != itr_blending->second[0].end();)
						{
							if (*itr_clipName_vec1 == clipName) {
								itr_clipName_vec1 = itr_blending->second[0].erase(itr_clipName_vec1);
							} else {
								++itr_clipName_vec1;
							}
						}
					}
				}
				break;
			case BlendMode::FROZEN_BLENDING:
			case BlendMode::CROSS_FADE_BLENDING:
				if (clipNames.size() == 2 && m_animationSets.find(clipNames[0]) != m_animationSets.end() && m_animationSets.find(clipNames[1]) != m_animationSets.end())
				{
					for (std::vector<std::vector<std::string>>::iterator itr_clipName_vec2 = itr_blending->second.begin(); itr_clipName_vec2 != itr_blending->second.end();)
					{
						bool found = false;
						std::vector<std::string> vec;
						for (std::vector<std::string>::iterator itr_clipName_vec1 = (*itr_clipName_vec2).begin(); itr_clipName_vec1 != (*itr_clipName_vec2).end();)
						{
							for (auto clipName : clipNames)
							{
								if (clipName == *itr_clipName_vec1)
								{
									itr_clipName_vec1 = (*itr_clipName_vec2).erase(itr_clipName_vec1);
									found = true;
								}
								else {
									vec.push_back(*itr_clipName_vec1);
									++itr_clipName_vec1;
								}
							}
						}

						if (found) {
							itr_clipName_vec2 = itr_blending->second.erase(itr_clipName_vec2);
							for (auto clipName : vec)
							{
								itr_blending->second[0].push_back(clipName);
							}
						} else {
							++itr_clipName_vec2;
						}
					}
				}
				break;
		}
	}

	m_blending[blendMode].push_back(clipNames);
}

void AnimationController::Update(float deltaTime)
{
	m_bPlaying = false;

	for (auto itr_blending: m_blending)
	{
		BlendMode blendMode = itr_blending.first;
		switch (blendMode)
		{
			case BlendMode::ADDICTIVE_BLENDING:
				for (auto clipName : itr_blending.second[0])
				{
					AnimationSet* animationSet = &m_animationSets[clipName];
					if (animationSet->isActive())
					{
						animationSet->update(deltaTime);		//update delta time first, then animate

						m_skeleton->m_vGlobalPose[0] = animationSet->m_vAnimations[0]->GetCurrentPose(deltaTime).Matrix();
						for (int i = 1; i < m_skeleton->m_vJoints.size(); ++i)
						{
							Joint* currJoint = m_skeleton->m_vJoints[i];
							m_skeleton->m_vGlobalPose[i] = m_skeleton->m_vGlobalPose[currJoint->m_iParent] * animationSet->m_vAnimations[i]->GetCurrentPose(deltaTime).Matrix();
						}
						m_bPlaying = true;
					}
				}
				break;
			case BlendMode::CROSS_FADE_BLENDING:
			case BlendMode::FROZEN_BLENDING:
				for (auto clipNames : itr_blending.second)
				{
					if (clipNames.size() == 2)
					{
						AnimationSet* fromClip = &m_animationSets[clipNames[0]];
						AnimationSet* toClip = &m_animationSets[clipNames[1]];

						if (fromClip->isActive() && toClip->isActive()) {
							fromClip->update(deltaTime);
							toClip->update(deltaTime);

							const float interpolant = (1.0f * fromClip->m_vAnimations[0]->getCurrentKeyframe() / fromClip->m_vAnimations[0]->getNumKeyframes());

							if (!fromClip->isLooping() && floor(interpolant) == 1) {
								fromClip->setActive(false);
							}

							/*
							static wchar_t s[64];
							swprintf(s, 64, L"interpolant: %f\n", interpolant);
							OutputDebugStringW(s);
							*/

							m_skeleton->m_vGlobalPose[0] = SQT::LerpSQT(fromClip->m_vAnimations[0]->GetCurrentPose(deltaTime), toClip->m_vAnimations[0]->GetCurrentPose(deltaTime), interpolant).Matrix();

							for (int i = 1; i < m_skeleton->m_vJoints.size(); ++i)
							{
								Joint* currJoint = m_skeleton->m_vJoints[i];
								m_skeleton->m_vGlobalPose[i] = m_skeleton->m_vGlobalPose[currJoint->m_iParent] * SQT::LerpSQT(fromClip->m_vAnimations[i]->GetCurrentPose(deltaTime), toClip->m_vAnimations[i]->GetCurrentPose(deltaTime), interpolant).Matrix();
							}

						} else {
							for (auto clipName : itr_blending.second[0])
							{
								AnimationSet* animationSet = &m_animationSets[clipName];
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
							}
						}
						m_bPlaying = true;
					}
				}
				break;
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
