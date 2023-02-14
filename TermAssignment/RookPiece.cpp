#include "RookPiece.h"


// called to initialize the object
void RookPiece::Initiialize(ID3D11Device* pDevice, LitColourShader* pLitShader)
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
	bodyMatrix = Matrix::CreateScale(Vector3(1.2f, 3.0f, 1.2f));
	ring1Matrix = Matrix::CreateScale(Vector3(1.8f, 0.6f, 1.8f)) * Matrix::CreateTranslation(Vector3(0, 1.3f, 0));
	ring2Matrix = Matrix::CreateScale(Vector3(1.6f, 0.4f, 1.6f)) * Matrix::CreateTranslation(Vector3(0, 1.0f, 0));
	ring3Matrix = Matrix::CreateScale(Vector3(1.3f, 0.4f, 1.3f)) * Matrix::CreateTranslation(Vector3(0, 0.8f, 0));

	float radius = 0.8f;
	float angleDelta = 360 / MAX_COLUMNS;

	Vector3 pos;
	pos.x = radius * cos(0);
	pos.y = 1.5f;
	pos.z = radius * sin(0);

	// Places squares in a circle
	for (int i = 0; i < MAX_COLUMNS; i++)
	{
		// Rotates around center at a distance of 0.8f
		ringColumnsMatrices[i] = Matrix::CreateScale(Vector3(0.2f, 0.7f, 0.4f)) * Matrix::CreateTranslation(pos) * Matrix::CreateRotationY((angleDelta * i) * XM_PI / 180);
	}

}

// called to draw the object
// The parent matrix allows the user to pass in a matrix to transform the entire piece
void RookPiece::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix)
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

	pShader->SetShaders(pDeviceContext, ring1Matrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, ring2Matrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, ring3Matrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	for (int i = 0; i < MAX_COLUMNS; i++)
	{
		pShader->SetShaders(pDeviceContext, ringColumnsMatrices[i] * parentMatrix, viewMatrix, projMatrix);
		cube.Draw(pDeviceContext);
	}

	pDeviceContext->PSSetConstantBuffers(2, 1, &pBaseBuffer);
	pShader->SetShaders(pDeviceContext, bodyMatrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);
}

// update the object
void RookPiece::Update(float deltaTime)
{

}
