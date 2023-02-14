#pragma once

#include "ChessPiece.h"

class RookPiece : public ChessPiece
{
public:

	RookPiece() : ChessPiece::ChessPiece() {}
	~RookPiece() { ChessPiece::~ChessPiece(); }

	// called to initialize the object
	void Initiialize(ID3D11Device* pDevice, LitColourShader* pLitShader);

	// called to draw the object
	void Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix);

	// update the object
	void Update(float deltaTime);

private:

	float rotation;

	// store each object
	IndexedPrimitive cylinder;
	IndexedPrimitive cube;

	Matrix bottom1Matrix;
	Matrix bottom2Matrix;
	Matrix bottom3Matrix;
	Matrix bodyMatrix;
	Matrix ring1Matrix;
	Matrix ring2Matrix;
	Matrix ring3Matrix;

	static const int MAX_COLUMNS = 7;
	Matrix ringColumnsMatrices[MAX_COLUMNS];
};

