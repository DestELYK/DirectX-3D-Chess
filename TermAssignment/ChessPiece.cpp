#include "ChessPiece.h"

void ChessPiece::SetSide(ID3D11Device* pDevice, eChessSide side)
{
	if (pBaseBuffer) pBaseBuffer->Release();
	if (pOuterBuffer) pOuterBuffer->Release();

	// Changes material based on side
	switch (side)
	{
	case Black:
		pBaseBuffer = MakeMaterialBuffer(pDevice, Color(0.3f, 0.3f, 0.3f), Color(0.4f, 0.4f, 0.4f), Colors::White.v, 60);
		pOuterBuffer = MakeMaterialBuffer(pDevice, Color(0.2f, 0.2f, 0.2f), Color(0.2f, 0.2f, 0.2f), Colors::White.v, 60);
		break;
	case White:
		pBaseBuffer = MakeMaterialBuffer(pDevice, Color(1, 1, 1), Color(1, 1, 1), Colors::White.v, 60);
		pOuterBuffer = MakeMaterialBuffer(pDevice, Color(0.8f, 0.8f, 0.8f), Color(0.8f, 0.8f, 0.8f), Colors::White.v, 60);
		break;
	}
}
