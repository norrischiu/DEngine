cbuffer CONSTANT_BUFFER : register(b0)
{
	float4x4 gViewProj;
	float4x4 gView;
	float4	gWorldFrustumPlanes[6];
	float4	gEyePosW;
	float	gTexelCellSpaceU;
	float	gTexelCellSpacev;
};

struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBinormal : BINORMAL;
	float2 vTex : TEXCOORD0;
	float2 vBoundsY : BOUNDSY;
};

struct HS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vNormal : NORMAL;
	float4 vTangent : TANGENT;
	float4 vBinormal : BINORMAL;
	float2 vTex : TEXCOORD0;
	float2 vBoundsY : BOUNDSY;
};

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

float CalcTessFactor(float3 p)
{
	float gMinDist = 1.0f;
	float gMaxDist = 250.0f;
	float gMinTess = 1.0f;
	float gMaxTess = 6.0f;
	//float d = distance(p, gEyePosW);
	// max norm in xz plane (useful to see detail levels from a bird's eye).
	float d = max( abs(p.x-gEyePosW.x), abs(p.z-gEyePosW.z) );

	float dist = saturate((d - gMinDist) / (gMaxDist - gMinDist));

	//float s = saturate((400.0f - gMinDist) / (gMaxDist - gMinDist));

	return pow(2, (lerp(gMaxTess, gMinTess, dist)));
}

// Returns true if the box is completely behind (in negative half space) of plane.
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
	float3 n = abs(plane.xyz);

	float r = dot(extents, n);

	// signed distance from center point to plane.
	float s = dot(center, plane.xyz);

	return (s + r) < plane.w;
}

// Returns true if the box is completely outside the frustum.
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
	for (int i = 0; i < 6; ++i)
	{
		// If the box is completely behind any of the frustum planes
		// then it is outside the frustum.
		if (AabbBehindPlaneTest(center, extents, frustumPlanes[i]))
		{
			return true;
		}
	}

	return false;
}


// constant hull shader function, count the tessellation factors for each patch
PatchTess ConstantHS(InputPatch<VS_OUTPUT, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	float minY = patch[0].vBoundsY.x;
	float maxY = patch[0].vBoundsY.y;

	float minX = 1000000;
	float maxX = -1000000;
	float minZ = 10000000;
	float maxZ = -10000000;
	for (int i = 0; i < 4; i++)
	{
		if (patch[i].vPos.x > maxX)
			maxX = patch[i].vPos.x;
		if (patch[i].vPos.z > maxZ)
			maxZ = patch[i].vPos.z;
		if (patch[i].vPos.x < minX)
			minX = patch[i].vPos.x;
		if (patch[i].vPos.z < minZ)
			minZ = patch[i].vPos.z;
	}

//	float3 vMin = float3(patch[0].vPos.x, minY, patch[0].vPos.z);
//	float3 vMax = float3(patch[2].vPos.x, maxY, patch[2].vPos.z);
	float3 vMin = float3(minX, minY, minZ);
	float3 vMax = float3(maxX, maxY, maxZ);

	float3 boxCenter = 0.5f*(vMin + vMax);
	float3 boxExtents = 0.5f*(vMax - vMin);	

	if (AabbOutsideFrustumTest(boxCenter, boxExtents, gWorldFrustumPlanes))
	{
		pt.EdgeTess[0] = 0.0f;
		pt.EdgeTess[1] = 0.0f;
		pt.EdgeTess[2] = 0.0f;
		pt.EdgeTess[3] = 0.0f;

		pt.InsideTess[0] = 0.0f;
		pt.InsideTess[1] = 0.0f;
	}
	else
	{
		float4 e0 = 0.5f*(patch[0].vPos + patch[2].vPos);
		float4 e1 = 0.5f*(patch[0].vPos + patch[1].vPos);
		float4 e2 = 0.5f*(patch[1].vPos + patch[3].vPos);
		float4 e3 = 0.5f*(patch[2].vPos + patch[3].vPos);
		float4  c = 0.25f*(patch[0].vPos + patch[1].vPos + patch[2].vPos + patch[3].vPos);

		pt.EdgeTess[0] = CalcTessFactor(float3(e0.x, e0.y, e0.z));
		pt.EdgeTess[1] = CalcTessFactor(float3(e1.x, e1.y, e1.z));
		pt.EdgeTess[2] = CalcTessFactor(float3(e2.x, e2.y, e2.z));
		pt.EdgeTess[3] = CalcTessFactor(float3(e3.x, e3.y, e3.z));

		pt.InsideTess[0] = CalcTessFactor(float3(c.x, c.y, c.z));
		pt.InsideTess[1] = pt.InsideTess[0];
	}
	return pt;
}

// function
[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HS_OUTPUT HS(InputPatch<VS_OUTPUT, 4> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HS_OUTPUT OUT;

	// Pass through shader.
	OUT.vPos = p[i].vPos;
	OUT.vTex = p[i].vTex;
	OUT.vNormal = p[i].vNormal;
	OUT.vTangent = p[i].vTangent;
	OUT.vBinormal = p[i].vBinormal;
	OUT.vBoundsY = p[i].vBoundsY;

	return OUT;
}