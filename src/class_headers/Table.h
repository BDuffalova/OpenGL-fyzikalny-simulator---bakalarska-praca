#pragma once
#include "Settings.h"

class Table {
private:
	float* vertices;
	//float vertices[NUM_VERTICES_CIRCLE] = {
	//	0.f +1 , 0.f + 1, 0.f,
	//	WIDTH_TABLE + 1, 0.f + 1, 0.f,
	//	WIDTH_TABLE + 1, HEIGHT_TABLE + 1, 0.f,
	//	0.f + 1, HEIGHT_TABLE + 1, 0.f
	//};

public:
	Table();
	~Table();
	float* getVertices();
};
