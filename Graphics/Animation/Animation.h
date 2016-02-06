#include <unordered_map>
#include <string>
#include "Math\simdmath.h"
#include "Skeleton.h"

class Animation
{
public:
	Animation(const float length);
	~Animation();

	void animate(Skeleton* skleton, const float time);
	void addAnimationMatrix(const float time, const Matrix4& matrix);

private:
	std::vector<float> m_times;
	std::vector<Matrix4> m_animationMatrixs;
	float m_length;
};

