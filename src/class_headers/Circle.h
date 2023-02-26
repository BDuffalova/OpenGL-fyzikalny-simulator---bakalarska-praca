#define NUM_SEGMENTS 36
#define NUM_VERTICES (NUM_SEGMENTS + 2) * 3

class Circle {
private:
	float vx;
	float vy;
	float x;
	float y;
	float vertices[NUM_VERTICES];

public:
	Circle();
	~Circle();
	void initCircle();
	float* getVertices();
};
