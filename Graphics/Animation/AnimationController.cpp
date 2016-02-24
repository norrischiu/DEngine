#include "AnimationController.h"
#include "Math\SQT.h"
#include "Math\simdmath.h"
#include "GameObject\GameObject.h"
#include <stdio.h>

#define C_STR(string, text)\
		(string + text).c_str()

AnimationController::AnimationController(Skeleton* skeleton) : m_skeleton(skeleton)
{
	m_ID = ComponentID;
}

void AnimationController::addAnimationSet(const std::string name, const AnimationSet& animationSet)
{
	std::pair<std::string, AnimationSet> t_animationSet = std::make_pair(name, animationSet);
	m_animationSets.insert(t_animationSet);
}

void AnimationController::CreateAnimationSets(const char* fileName)
{
	std::string sFileName(fileName);
	FILE* pFile = fopen(C_STR(sFileName, "_animation.bufa"), "r");
	char c[256], clipName[256];
	int iNumJoints, iNumFrames;
	float quat[4], trans[3]; float transform[16];
	float scale;
	fscanf(pFile, "%s", &c);
	fscanf(pFile, "%s", &clipName);
	fscanf(pFile, "%i", &iNumJoints);
	fscanf(pFile, "%i", &iNumFrames);
	AnimationSet* animSet = new AnimationSet(0);
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
	AnimationSet* animationSet = getAnimationSet(name);

	/*
	if (animationSet) {
		return animationSet->isActive();
	}

	return false;
	*/

	return animationSet->isActive();
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

void AnimationController::Update(float deltaTime)
{
	for (
		std::unordered_map<std::string, AnimationSet>::iterator it_s = m_animationSets.begin();
		it_s != m_animationSets.end();
		++it_s
		) {
		if (it_s->second.isActive())
		{
			it_s->second.update(deltaTime);		//update delta time first, then animate

			m_skeleton->m_vGlobalPose[0] = it_s->second.m_vAnimations[0]->GetCurrentPose().Matrix();
			for (int i = 0; i < m_skeleton->m_vJoints.size(); ++i)
			{
				Joint* currJoint = m_skeleton->m_vJoints[i];
				if (currJoint->m_iParent != -1)
				{
					m_skeleton->m_vGlobalPose[i] = m_skeleton->m_vGlobalPose[currJoint->m_iParent] * it_s->second.m_vAnimations[i]->GetCurrentPose().Matrix();
				}
			}
		}
	}
}

AnimationController::~AnimationController()
{
}
