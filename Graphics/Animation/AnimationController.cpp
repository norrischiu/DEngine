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

AnimationSet* AnimationController::findAnimationSet(const std::string name)
{
	auto t = m_animationSets.find(name);

	if (t == m_animationSets.end()) {
		return nullptr;
	} else {
		return &t->second;
	}
	
	return nullptr;
}

Animation* AnimationController::findAnimation(const std::string set_name, const std::string animaton_name)
{
	AnimationSet* animationSet = findAnimationSet(set_name);

	if (animationSet) {
		return animationSet->findAnimation(animaton_name);
	}

	return nullptr;
}


void AnimationController::setActiveAnimationSet(const std::string name, const bool active)
{
	AnimationSet* animationSet = findAnimationSet(name);

	if (animationSet) {
		animationSet->setActive(active);
	}
}

bool AnimationController::isAnimationSetActive(const std::string name)
{
	AnimationSet* animationSet = findAnimationSet(name);

	/*
	if (animationSet) {
		return animationSet->isActive();
	}

	return false;
	*/

	return animationSet->isActive();
}

bool AnimationController::triggerAnimation(const std::string set_name, const std::string animaton_name)
{
	AnimationSet* animationSet = findAnimationSet(set_name);
	Animation* animation = nullptr;

	if (animationSet) {
		animation = animationSet->findAnimation(animaton_name);
		if (animation) {
			animation->animate(m_skeleton, animationSet->getCurrTime());
			return true;
		}
	}

	return false;
}

int AnimationController::getNumAnimationSets() const
{
	return m_animationSets.size();
}

int AnimationController::getNumAnimations(const std::string name)
{
	AnimationSet* animationSet = findAnimationSet(name);

	/*
	if (animationSet) {
		return animationSet->getNumAnimations();
	}

	return -1;
	*/

	return animationSet->getNumAnimations();
}

void AnimationController::update(const float time)
{
	for (
		std::unordered_map<std::string, AnimationSet>::iterator it = m_animationSets.begin();
		it != m_animationSets.end();
		++it
	) {
		it->second.update(time);
	}
}

AnimationController::~AnimationController()
{
}
