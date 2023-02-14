//
// Model of a pawn
//
//  BGTD 9201
//



#ifndef _PAWN_H
#define _PAWN_H

#include "ChessPiece.h"

using namespace DirectX;

class PawnPiece : public ChessPiece
{
public:

	PawnPiece() : ChessPiece::ChessPiece() {}
	~PawnPiece() { ChessPiece::~ChessPiece(); }

	// called to initialize the object
	void Initiialize(ID3D11Device* pDevice, LitColourShader* pLitShader);

	// called to draw the object
	void Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix);

	// update the object
	void Update(float deltaTime);

private:

	// store each object
	IndexedPrimitive cylinder;
	IndexedPrimitive cone;
	IndexedPrimitive sphere;

	Matrix bottom1Matrix;
	Matrix bottom2Matrix;
	Matrix bottom3Matrix;
	Matrix bodyMatrix;
	Matrix topMatrix;

};

#endif