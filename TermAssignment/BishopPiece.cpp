#include "BishopPiece.h"

// called to initialize the object
void BishopPiece::Initiialize(ID3D11Device* pDevice, LitColourShader* pLitShader)
{
	pShader = pLitShader;

	SetSide(pDevice, Black);

	cylinder.InitializeGeometry(pDevice, Cylinder);
	cylinder.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	cube.InitializeGeometry(pDevice, Cube);
	cube.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	sphere.InitializeGeometry(pDevice, Sphere);
	sphere.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	bottom1Matrix = Matrix::CreateScale(1.8f, 0.2f, 1.8f) * Matrix::CreateTranslation(0, -1.5f, 0);
	bottom2Matrix = Matrix::CreateScale(1.6f, 0.4f, 1.6f) * Matrix::CreateTranslation(0, -1.2f, 0);
	bottom3Matrix = Matrix::CreateScale(1.3f, 0.4f, 1.3f) * Matrix::CreateTranslation(0, -1.0f, 0);
	bodyMatrix = Matrix::CreateScale(1.2f, 1.5f, 1.2f);
	ringMatrix = Matrix::CreateScale(1.4f, 0.4f, 1.4f) * Matrix::CreateTranslation(0, 0.6f, 0);
	top1Matrix = Matrix::CreateScale(1.0f, 0.3f, 1.0f) * Matrix::CreateTranslation(0, 0.8f, 0);
	top2Matrix = Matrix::CreateScale(1.2f) * Matrix::CreateTranslation(0, 1.2f, 0);
	top3Matrix = Matrix::CreateScale(0.5f) * Matrix::CreateTranslation(0, 1.8f, 0);
}

// called to draw the object
// The parent matrix allows the user to pass in a matrix to transform the entire piece
void BishopPiece::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix)
{
	// set all 3 to the diffuse
	pDeviceContext->PSSetShaderResources(0, 1, &pDiffuse);
	pDeviceContext->PSSetShaderResources(1, 1, &pDiffuse);
	pDeviceContext->PSSetShaderResources(2, 1, &pSpec);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pOuterBuffer);
	pShader->SetShaders(pDeviceContext, bottom1Matrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, bottom2Matrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, bottom3Matrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, ringMatrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, top1Matrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, top2Matrix*parentMatrix, viewMatrix, projMatrix);
	sphere.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, top3Matrix*parentMatrix, viewMatrix, projMatrix);
	sphere.Draw(pDeviceContext);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pBaseBuffer);
	pShader->SetShaders(pDeviceContext, bodyMatrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);
}

// update the object
void BishopPiece::Update(float deltaTime)
{
}