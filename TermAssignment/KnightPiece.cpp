#include "KnightPiece.h"

// called to initialize the object
void KnightPiece::Initiialize(ID3D11Device* pDevice, LitColourShader* pLitShader)
{
	pShader = pLitShader;

	SetSide(pDevice, Black);

	cylinder.InitializeGeometry(pDevice, Cylinder);
	cylinder.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	cube.InitializeGeometry(pDevice, Cube);
	cube.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	bottom1Matrix = Matrix::CreateScale(Vector3(1.8f, 0.2f, 1.8f)) * Matrix::CreateTranslation(Vector3(0, -1.5f, 0));
	bottom2Matrix = Matrix::CreateScale(Vector3(1.6f, 0.4f, 1.6f)) * Matrix::CreateTranslation(Vector3(0, -1.2f, 0));
	bottom3Matrix = Matrix::CreateScale(Vector3(1.3f, 0.4f, 1.3f)) * Matrix::CreateTranslation(Vector3(0, -1.0f, 0));
	bodyMatrix = Matrix::CreateScale(Vector3(0.6f, 1.5f, 0.8f));
	maneMatrix = Matrix::CreateScale(Vector3(0.7f, 1.2f, 1.2f)) * Matrix::CreateRotationX(45 * XM_PI / 180) * Matrix::CreateTranslation(0, 0.8f, -0.2f);
	faceMatrix = Matrix::CreateScale(Vector3(0.7f, 0.9f, 1.2f)) * Matrix::CreateTranslation(0, 0.8f, 0.2f);
}

// called to draw the object
// The parent matrix allows the user to pass in a matrix to transform the entire piece
void KnightPiece::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix)
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

	pShader->SetShaders(pDeviceContext, maneMatrix*parentMatrix, viewMatrix, projMatrix);
	cube.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, faceMatrix*parentMatrix, viewMatrix, projMatrix);
	cube.Draw(pDeviceContext);

	pDeviceContext->PSSetConstantBuffers(2, 1, &pBaseBuffer);
	pShader->SetShaders(pDeviceContext, bodyMatrix*parentMatrix, viewMatrix, projMatrix);
	cube.Draw(pDeviceContext);
}

// update the object
void KnightPiece::Update(float deltaTime)
{
}