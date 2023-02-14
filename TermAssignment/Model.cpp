#include "Model.h"

// Helper to make a buffer from the given materials
//
ID3D11Buffer* Model::MakeMaterialBuffer(ID3D11Device* pDevice, Color ambient, Color diffuse, Color spec, float specPower)
{
	MaterialBuffer mat;
	mat.ambient = ambient;
	mat.diffuse = diffuse;
	mat.spec = spec;
	mat.spec.w = specPower;

	// Create the constant buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(MaterialBuffer);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &mat;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	ID3D11Buffer* pConstantBuffer;
	pDevice->CreateBuffer(&bufferDesc, &data, &pConstantBuffer);
	return pConstantBuffer;
}

Model::Model()
{
	pShader = nullptr;
	pDiffuse = nullptr;
	pSpec = nullptr;
	pBaseBuffer = nullptr;
	pOuterBuffer = nullptr;
}

Model::~Model()
{
	if (pDiffuse) pDiffuse->Release();
	if (pSpec) pSpec->Release();
	if (pBaseBuffer) pBaseBuffer->Release();
	if (pOuterBuffer) pOuterBuffer->Release();
}
