#pragma once
class Blending
{
public:
	enum BlendMode {
		ADDICTIVE_BLENDING,
		CROSS_FADE_BLENDING,
		FROZEN_BLENDING
	};

	Blending();
	~Blending();
};

