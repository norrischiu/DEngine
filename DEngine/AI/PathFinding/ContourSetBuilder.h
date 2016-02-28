#pragma once

class ContourSetBuilder
{
public:
	ContourSetBuilder();
	~ContourSetBuilder();

	void searchRegionEdges();
	void vectorize();
	void optimize();
};

