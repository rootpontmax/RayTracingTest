__kernel void traceRay( __global const float8 *rayPosDir,
						__global const float4 *spherePosAndRad,
						__global float4 *outCol )//,
						//int sphereCount )
{
    // Get the index of the current element to be processed
    int i = get_global_id(0);


    // Do the operation
    //C[i] = A[i] + B[i];
    outCol[i].s0 = rayPosDir[i].s4 * 0.5f + 0.5f;
    outCol[i].s1 = rayPosDir[i].s5 * 0.5f + 0.5f;
    outCol[i].s2 = rayPosDir[i].s6 * 0.5f + 0.5f;
    outCol[i].s3 = rayPosDir[i].s7 * 0.5f + 0.5f;
}