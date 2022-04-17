#include "OCLTracer.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
COCLTracer::COCLTracer( const int sizeX, const int sizeY ) :
    m_context( nullptr ),
    m_queue( nullptr )
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
    OCL_VERIFY( m_context, errorCode );

    // Command queue
    m_queue = clCreateCommandQueue( m_context, deviceID, 0, &errorCode );
    OCL_VERIFY( m_queue, errorCode );
    
    // Create buffers
    const size_t bufferSize = sizeX * sizeY;
    m_rayPosBuffer = clCreateBuffer( m_context, CL_MEM_READ_ONLY, sizeof( float ) * 3 * bufferSize, nullptr, &errorCode );
    OCL_VERIFY( m_rayPosBuffer, errorCode );
    m_rayDirBuffer = clCreateBuffer( m_context, CL_MEM_READ_ONLY, sizeof( float ) * 3 * bufferSize, nullptr, &errorCode );
    OCL_VERIFY( m_rayDirBuffer, errorCode );

    //;
    //m_rayDirBuffer;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
COCLTracer::~COCLTracer()
{
    OCL_ASSERT( clReleaseMemObject( m_rayDirBuffer ) );
    OCL_ASSERT( clReleaseMemObject( m_rayPosBuffer ) );
    OCL_ASSERT( clReleaseCommandQueue( m_queue ) );
    OCL_ASSERT( clReleaseContext( m_context ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace

