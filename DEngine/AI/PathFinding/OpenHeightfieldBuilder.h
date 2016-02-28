#pragma once

class OpenHeightfieldBuilder
{
public:
	OpenHeightfieldBuilder();
	~OpenHeightfieldBuilder();

	void createDistanceField();
	void watershed();
	void filterSmallRegions();
	void cleanNullRegionBorders();
};

