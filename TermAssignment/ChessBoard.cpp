#include "ChessBoard.h"

void ChessBoard::Initiialize(ID3D11Device * pDevice, LitColourShader * pLitShader)
{
	pShader = pLitShader;

	pBaseBuffer = MakeMaterialBuffer(pDevice, Colors::White.v, Colors::White.v, Colors::White.v, 30);

	cube.InitializeGeometry(pDevice, Cube);
	cube.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	boardMatrix = Matrix::CreateScale(30.0f, 1.0f, 30.0f);
}

void ChessBoard::Draw(ID3D11DeviceContext * pDeviceContext, const Matrix & parentMatrix, const Matrix & viewMatrix, const Matrix & projMatrix)
{
	pDeviceContext->PSSetShaderResources(0, 1, &pDiffuse);
	pDeviceContext->PSSetShaderResources(1, 1, &pDiffuse);
	pDeviceContext->PSSetShaderResources(2, 1, &pDiffuse);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pBaseBuffer);
	pShader->SetShaders(pDeviceContext, boardMatrix * parentMatrix, viewMatrix, projMatrix);
	cube.Draw(pDeviceContext);
}

void ChessBoard::Update(float deltaTime)
{
}
