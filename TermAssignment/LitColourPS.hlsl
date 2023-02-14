 //
// Pixel shader for an unlit vertex colour shader
//

#include "VertexPositionNormalTexture.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
	// The interpolated color 
	//
	float3 normal = normalize(input.WorldNormal.xyz);




	float4 materialAmbient = matAmbient * texAmbient.Sample(Sampler, input.UV);
	float4 materialDiffuse = matDiffuse * texDiffuse.Sample(Sampler, input.UV);
	float4 materialSpecular = matSpecular * texSpecular.Sample(Sampler, input.UV);
	float materialSpecPower = matSpecular.a;

	float4 finalColor =
		AmbientLighting(materialAmbient, ambientColour)
		+ DiffuseLighting(directionalLightVector.xyz, normal, directionalLightColor, materialDiffuse)
		+ DiffuseLighting(directionalLightVector2.xyz, normal, directionalLightColor2, materialDiffuse)
		+ DiffuseLighting(directionalLightVector3.xyz, normal, directionalLightColor3, materialDiffuse)
		+ SpecularLightingBlinnPhong(directionalLightVector.xyz, normal, worldCameraPos.xyz, input.WorldPosition.xyz, specularLightColor, materialSpecular, materialSpecPower)
		+ SpecularLightingBlinnPhong(directionalLightVector2.xyz, normal, worldCameraPos.xyz, input.WorldPosition.xyz, specularLightColor2, materialSpecular, materialSpecPower)
	    + SpecularLightingBlinnPhong(directionalLightVector3.xyz, normal, worldCameraPos.xyz, input.WorldPosition.xyz, specularLightColor3, materialSpecular, materialSpecPower);

	finalColor.a = 1.0f;
	return finalColor;
}