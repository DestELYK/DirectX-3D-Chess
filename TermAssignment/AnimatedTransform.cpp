//
// AnimatedTransform.cpp
//	BGTD 9201 - Helper class that animates a transforms over time
//
#include "AnimatedTransform.h"

//----------------------------------------------------------------------------------------------
// Linear Interpolate Matrices, decomposing if possible to give nondistorting results
//----------------------------------------------------------------------------------------------
static Matrix LerpMatrices(Matrix& start, Matrix& end, float t)
{
	Vector3 scaleA, scaleB;
	Quaternion rotA, rotB;
	Vector3 transA, transB;

	Matrix result;
	if (start.Decompose(scaleA, rotA, transA) && end.Decompose(scaleB, rotB, transB))
	{
		Vector3 scale = Vector3::Lerp(scaleA, scaleB, t);
		Quaternion rot = Quaternion::Lerp(rotA, rotB, t);
		Vector3 trans = Vector3::Lerp(transA, transB, t);
		result = Matrix::CreateScale(scale)*Matrix::CreateFromQuaternion(rot)*Matrix::CreateTranslation(trans);
	}
	else
	{
		result = Matrix::Lerp(start, end, t);
	}
	return result;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
AnimatedTransform::AnimatedTransform()
{
	currentTime = 0;
	animationTime = 0;
	matricesArray = nullptr;
}

//----------------------------------------------------------------------------------------------
// initialize the transform
//----------------------------------------------------------------------------------------------
void AnimatedTransform::Initialize(Matrix* mArray, int mIndex, Matrix mEndMatrix, float duration)
{
	currentTime = 0;
	animationTime = duration;
	matricesArray = mArray;
	index = mIndex;
	startMatrix = matricesArray[index];
	endMatrix = mEndMatrix;
}

//----------------------------------------------------------------------------------------------
// update the animating object
//----------------------------------------------------------------------------------------------
void AnimatedTransform::Update(float deltaTime)
{
	if (matricesArray != nullptr && currentTime < animationTime)
	{
		currentTime += deltaTime;

		// get the interpolation from 0 - 1
		float t = currentTime / animationTime;
		if (t > 1) t = 1;

		matricesArray[index] = LerpMatrices(startMatrix, endMatrix, t);
	}
}

void AnimatedTransform::Reset()
{
	if (!matricesArray) return;

	currentTime = 0;
	matricesArray[index] = startMatrix;
}

