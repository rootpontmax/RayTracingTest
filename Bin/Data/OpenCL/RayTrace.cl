
////////////////////////////////////////////////////////////////////////////////////////////////////
bool DoesHitSphere( float4 rayPos, float4 rayDir, float4 spherePos, float sphereRad, float tMin, float tMax, float *pRoot, float4 *pPos, float4 *pNor )
{
	const float radSqr = sphereRad * sphereRad;
	const float4 os = rayPos - spherePos;
    const float a = dot( rayDir, rayDir );
    const float halfB = dot( os, rayDir );
	const float c = dot( os, os ) - radSqr;
    const float d = halfB * halfB - a * c;
    if( d < 0.0f )
        return false;

    const float dSqrt = sqrt( d );
    float root = ( -halfB - dSqrt ) / a;
    if( root < tMin || root > tMax )
    {
        root = ( -halfB + dSqrt ) / a;
        if( root < tMin || root > tMax )
        {
            return false;
        }
    }

    float4 pos = rayPos + rayDir * root;
    float4 nor = ( pos - spherePos ) / sphereRad;

    *pRoot = root;
    *pPos = pos;
    *pNor = nor;
    
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
__kernel void traceRay( __global const float8 *rayPosDir,
						__global const float4 *spherePosAndRad,
						__global float4 *outCol,
						size_t sphereCount )
{
    // Get the index of the current element to be processed
    int i = get_global_id(0);

    const float4 rayPos = rayPosDir[i].lo;
    const float4 rayDir = rayPosDir[i].hi;

    float workRoot = 0.0f;
    float trueRoot = 1000.0f;
    float4 workPos;
    float4 workNor;
    float4 truePos;
    float4 trueNor;

    bool bWasFoundAnything = false;

    for( size_t sphereIndex = 0; sphereIndex < sphereCount; ++sphereIndex )
    {
    	float4 spPos = spherePosAndRad[sphereIndex];
    	float spRad = spPos.w;
    	spPos.w = 0.0f;
    	float tMin = 0.0f;
    	float tMax = 100.0f;
    	if( DoesHitSphere( rayPos, rayDir, spPos, spRad, tMin, tMax, &workRoot, &workPos, &workNor ) )
    	{
    		if( workRoot < trueRoot )
    		{
    			trueRoot = workRoot;
    			truePos = workPos;
    			trueNor = workNor;
    			bWasFoundAnything = true;
    		}
    	}

    }

    if( bWasFoundAnything )
    {
    	outCol[i] = trueNor * 0.5f + 0.5f;
    	outCol[i].w = 1.0f;
    }
    else
    {
    	outCol[i].x = 0.0f;
    	outCol[i].y = 0.0f;
    	outCol[i].z = 0.0f;
    	outCol[i].w = 1.0f;
    }


    // Do the operation
    //C[i] = A[i] + B[i];
    //*
    
    //*/

    /*
    outCol[i].s0 = rayPosDir[i].s4;
    outCol[i].s1 = rayPosDir[i].s5;
    outCol[i].s2 = rayPosDir[i].s6;
    outCol[i].s3 = rayPosDir[i].s7;
    */
}
////////////////////////////////////////////////////////////////////////////////////////////////////