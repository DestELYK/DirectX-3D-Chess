#include "ChessPiece.h"

using namespace DirectX;

class KingPiece : public ChessPiece
{
public:

	KingPiece() : ChessPiece::ChessPiece() {}
	~KingPiece() { ChessPiece::~ChessPiece(); }

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
	IndexedPrimitive cube;

	Matrix bottom1Matrix;
	Matrix bottom2Matrix;
	Matrix bottom3Matrix;

	Matrix bodyMatrix;

	Matrix top1Matrix;
	Matrix top2Matrix;
	Matrix top3Matrix;
	Matrix top4Matrix;
	Matrix top5Matrix;

	static const int MAX_RINGS = 8;
	Matrix ringMatrices[MAX_RINGS];

};