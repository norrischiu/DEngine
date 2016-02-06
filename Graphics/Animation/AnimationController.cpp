#include "AnimationController.h"

AnimationController::AnimationController(Skeleton* skeleton) : m_skeleton(skeleton)
{

}

void AnimationController::addAnimationSet(const std::string name, const AnimationSet& animationSet)
{
	std::pair<std::string, AnimationSet> t_animationSet = std::make_pair(name, animationSet);
	m_animationSets.insert(t_animationSet);
}

void AnimationController::setAnimationSet(const std::unordered_map<std::string, AnimationSet>& animationSet)
{
	m_animationSets = animationSet;
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

bool AnimationController::triggerAnimation(const std::string set_name, const float currTime)
{
	AnimationSet* animationSet = findAnimationSet(set_name);
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
	AnimationSet* animationSet = findAnimationSet(name);

	/*
	if (animationSet) {
		return animationSet->getNumAnimations();
	}

	return -1;
	*/

	return animationSet->getNumAnimations();
}

void AnimationController::update(const float delta_time)
{
	for (
		std::unordered_map<std::string, AnimationSet>::iterator it_s = m_animationSets.begin();
		it_s != m_animationSets.end();
		++it_s
	) {
		it_s->second.update(delta_time);		//update delta time first, then animate
		
		std::unordered_map<std::string, Animation>* animations = it_s->second.getAnimations();

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
