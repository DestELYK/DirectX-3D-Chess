//
// AnimatedTransform.h
//	BGTD 9201 - Helper class that animates a transforms over time
//
#ifndef _ANIMATED_TRANSFORM_H
#define _ANIMATED_TRANSFORM_H
#include <d3d11_1.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

class AnimatedTransform
{
public:
	AnimatedTransform();

	// initialize the transform
	void Initialize(Matrix* mArray, int mIndex, Matrix endMatrix, float duration);

	// update the animating object
	void Update(float deltaTime);

	// check if we are done animating
	bool isDone() { return currentTime >= animationTime; }

	// Reset animation
	void Reset();

private:

	float		animationTime;
	float		currentTime;

	Matrix*		matricesArray;
	int			index;

	Matrix		startMatrix;
	Matrix		endMatrix;

};


#endif 
