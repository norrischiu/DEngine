#pragma once

class SolidHeightfieldBuilder
{
public:
	SolidHeightfieldBuilder();
	~SolidHeightfieldBuilder();

	void voxelize();
	bool isTraversable();
};

