#include "KingPiece.h"

// called to initialize the object
void KingPiece::Initiialize(ID3D11Device* pDevice, LitColourShader* pLitShader)
{
	pShader = pLitShader;

	SetSide(pDevice, Black);

	cylinder.InitializeGeometry(pDevice, Cylinder);
	cylinder.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	cone.InitializeGeometry(pDevice, Cone);
	cone.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	sphere.InitializeGeometry(pDevice, Sphere);
	sphere.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	cube.InitializeGeometry(pDevice, Cube);
	cube.InitializeInputLayout(pDevice, pShader->GetVertexShaderBinary(), pShader->GetVertexShaderBinarySize());

	bottom1Matrix = Matrix::CreateScale(Vector3(1.8f, 0.2f, 1.8f)) * Matrix::CreateTranslation(Vector3(0, -1.5f, 0));
	bottom2Matrix = Matrix::CreateScale(Vector3(1.6f, 0.4f, 1.6f)) * Matrix::CreateTranslation(Vector3(0, -1.2f, 0));
	bottom3Matrix = Matrix::CreateScale(Vector3(1.3f, 0.4f, 1.3f)) * Matrix::CreateTranslation(Vector3(0, -1.0f, 0));
	bodyMatrix = Matrix::CreateScale(Vector3(1.2f, 2.0f, 1.2f));
	top1Matrix = Matrix::CreateScale(1.4f, 0.4f, 1.4f) * Matrix::CreateTranslation(0, 0.9f, 0);
	top2Matrix = Matrix::CreateScale(1.3f, 1.5f, 1.3f) * Matrix::CreateRotationX(180 * XM_PI / 180) * Matrix::CreateTranslation(0, 0.8f, 0);
	top3Matrix = Matrix::CreateScale(0.7f) * Matrix::CreateTranslation(0, 1.5f, 0);
	top4Matrix = Matrix::CreateScale(0.2f, 1.0f, 0.2f) * Matrix::CreateTranslation(0, 2.0f, 0);
	top5Matrix = Matrix::CreateScale(0.5f, 0.2f, 0.2f) * Matrix::CreateTranslation(0, 2.2f, 0);

	float radius = 0.5f;
	float angleDelta = 360 / MAX_RINGS;

	Vector3 pos;
	pos.x = radius * cos(0);
	pos.y = 1.6f;
	pos.z = radius * sin(0);

	// Places squares in a circle
	for (int i = 0; i < MAX_RINGS; i++)
	{
		// Rotates around center at a distance of 0.8f
		ringMatrices[i] = Matrix::CreateScale(0.3f) * Matrix::CreateTranslation(pos) * Matrix::CreateRotationY((angleDelta * i) * XM_PI / 180);
	}
}

// called to draw the object
// The parent matrix allows the user to pass in a matrix to transform the entire piece
void KingPiece::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix)
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

	pShader->SetShaders(pDeviceContext, top1Matrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, top2Matrix*parentMatrix, viewMatrix, projMatrix);
	cone.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, top3Matrix*parentMatrix, viewMatrix, projMatrix);
	sphere.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, top4Matrix*parentMatrix, viewMatrix, projMatrix);
	cube.Draw(pDeviceContext);

	pShader->SetShaders(pDeviceContext, top5Matrix*parentMatrix, viewMatrix, projMatrix);
	cube.Draw(pDeviceContext);

	for (int i = 0; i < MAX_RINGS; i++)
	{
		pShader->SetShaders(pDeviceContext, ringMatrices[i] * parentMatrix, viewMatrix, projMatrix);
		sphere.Draw(pDeviceContext);
	}

	pDeviceContext->PSSetConstantBuffers(2, 1, &pBaseBuffer);
	pShader->SetShaders(pDeviceContext, bodyMatrix*parentMatrix, viewMatrix, projMatrix);
	cylinder.Draw(pDeviceContext);
}

// update the object
void KingPiece::Update(float deltaTime)
{
}