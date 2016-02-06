#include <string>
#include <unordered_map>
#include "Animation.h"

class AnimationSet
{
public:
	AnimationSet();
	AnimationSet(const float currTime, const bool looping);
	~AnimationSet();

	void addAnimation(const std::string name, const Animation& animation);
	void removeAnimation(const std::string name);
	Animation* findAnimation(const std::string name);

	float getCurrTime();
	int getNumAnimations() const;
	bool isActive() const;
	void setActive(const bool active);
	void update(const float time);

private:
	std::unordered_map<std::string, Animation> m_animations;
	float m_currTime;
	bool m_active;
};

