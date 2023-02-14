#pragma once

#include "DirectX.h"
#include "IndexedPrimitive.h"
#include "LitColourShader.h"
#include <d3d11_1.h>
#include <SimpleMath.h>

using namespace DirectX;

struct MaterialBuffer
{
	Color ambient;
	Color diffuse;
	Color spec; // alpha in w component

};

class Model
{
public:
	Model();
	~Model();

	// called to initialize the object
	virtual void Initiialize(ID3D11Device* pDevice, LitColourShader* pLitShader) = 0;

	// called to draw the object
	virtual void Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& parentMatrix, const Matrix& viewMatrix, const Matrix& projMatrix) = 0;

	// update the object
	virtual void Update(float deltaTime) = 0;

	void SetTextures(ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* spec) {
		pDiffuse = diffuse;
		pSpec = spec;
	}

protected:
	ID3D11Buffer* MakeMaterialBuffer(ID3D11Device* pDevice, Color ambient, Color diffuse, Color spec, float specPower);

	ID3D11ShaderResourceView* pDiffuse;
	ID3D11ShaderResourceView* pSpec;

	LitColourShader* pShader;

	ID3D11Buffer* pBaseBuffer;
	ID3D11Buffer* pOuterBuffer;
};

