#include "AnimationController.h"

AnimationController::AnimationController(Skeleton* skeleton) : m_skeleton(skeleton)
{

}

void AnimationController::addAnimationSet(const std::string name, const AnimationSet& animationSet)
{
	std::pair<std::string, AnimationSet> t_animationSet = std::make_pair(name, animationSet);
	m_animationSets.insert(t_animationSet);
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

std::unordered_map<std::string, Animation>* AnimationController::getAnimations(const std::string set_name)
{
	AnimationSet* animationSet = getAnimationSet(set_name);
	return animationSet->getAnimations();
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

std::vector<AnimationSet*>& AnimationController::getActiveAnimationSets()
{
	std::vector<AnimationSet*> activeAnimationSets;

	for (
		std::unordered_map<std::string, AnimationSet>::iterator it = m_animationSets.begin();
		it != m_animationSets.end();
		++it
	) {
		if (it->second.isActive()) {
			activeAnimationSets.push_back(&it->second);
		}
	}

	return activeAnimationSets;
}

void AnimationController::update(const float delta_time)
{
	std::vector<AnimationSet*> activeAnimationSets = getActiveAnimationSets();

	for (
		std::vector<AnimationSet*>::iterator it_s = activeAnimationSets.begin();
		it_s != activeAnimationSets.end();
		++it_s
	) {
		(*it_s)->update(delta_time);		//update delta time first, then animate
		
		std::unordered_map<std::string, Animation>* animations = (*it_s)->getAnimations();

		for (
			std::unordered_map<std::string, Animation>::iterator it_a = animations->begin();
			it_a != animations->end();
			++it_a
		) {
			const Animation& animation = it_a->second;
			m_skeleton->updateSkeletonNode(animation.getNodeName(), animation.getCurrentMatrix());
		}
	}
}

AnimationController::~AnimationController()
{
}
