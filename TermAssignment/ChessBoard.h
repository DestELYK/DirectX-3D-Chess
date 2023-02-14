#pragma once

#include "Model.h"

class ChessBoard : public Model
{
public:

	ChessBoard() : Model::Model() {}
	~ChessBoard() { Model::~Model(); }

	// called to initialize the object
	void Initiialize(ID3D11Device* pDevice, LitColourShader* pLitShader);

	// called to draw the object
	void Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix);

	// update the object
	void Update(float deltaTime);

private:
	IndexedPrimitive cube;

	Matrix boardMatrix;
};

