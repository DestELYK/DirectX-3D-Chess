#pragma once
#include "Model.h"

enum eChessSide
{
	Black,
	White
};

class ChessPiece :
	public Model
{
public:
	ChessPiece() : Model::Model() {}
	~ChessPiece() { Model::~Model(); }

	// Changes material based on side
	void SetSide(ID3D11Device* pDevice, eChessSide side);
};

