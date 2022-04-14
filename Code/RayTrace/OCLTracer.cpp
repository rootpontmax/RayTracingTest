#include "OCLTracer.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
COCLTracer::COCLTracer() :
    m_context( nullptr )
{
    // Platform
    cl_platform_id platformID = nullptr;
    cl_uint platformCount = 0;
    OCL_ASSERT( clGetPlatformIDs( 1, &platformID, &platformCount ) );
    
    // Device
    cl_device_id deviceID = nullptr;
    cl_uint deviceCount = 0;
    OCL_ASSERT( clGetDeviceIDs( platformID, CL_DEVICE_TYPE_GPU, 1, &deviceID, &deviceCount ) );
    
    // Context
    cl_int errorCode = 0;
    m_context = clCreateContext( nullptr, 1, &deviceID, nullptr, nullptr, &errorCode );
    if( m_context && CL_SUCCESS != errorCode )
    {
        BREAK( "OpenCL can't create context" );
        return;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
COCLTracer::~COCLTracer()
{
    OCL_ASSERT( clReleaseContext( m_context ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace

