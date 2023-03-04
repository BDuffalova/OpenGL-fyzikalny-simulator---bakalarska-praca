#pragma once
#include "Settings.h"

class Table {
private:
	float vertices[NUM_VERTICES_CIRCLE] = {
		TABLE_OFFSET_LEFT, TABLE_OFFSET_LEFT, 0.f,
		TABLE_OFFSET_LEFT + WIDTH_TABLE, TABLE_OFFSET_LEFT, 0.f,
		TABLE_OFFSET_LEFT + WIDTH_TABLE, TABLE_OFFSET_LEFT + HEIGHT_TABLE, 0.f,
		TABLE_OFFSET_LEFT, TABLE_OFFSET_LEFT + HEIGHT_TABLE, 0.f
	};
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
