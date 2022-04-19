#include "OCLTracer.h"

#include "../Core/FileSystem.h"
#include "../Core/File.h"
#include "../Core/Color.h"

#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"

#include <iostream>

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
static const int MAX_SPHERE_COUNT = 10;
////////////////////////////////////////////////////////////////////////////////////////////////////
COCLTracer::COCLTracer( const int sizeX, const int sizeY ) :
    m_sizeX( sizeX ),
    m_sizeY( sizeY ),
    m_fullX( static_cast< float >( sizeX - 1 ) ),
    m_fullY( static_cast< float >( sizeY - 1 ) ),
    m_bufferSize( sizeX * sizeY ),
    m_context( nullptr ),
    m_queue( nullptr ),
    m_program( nullptr ),
    m_kernel( nullptr ),
    m_rayPosDirBuffer( nullptr ),
    m_outColBuffer( nullptr ),
    m_sceneSphereBuffer( nullptr )
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

    VERIFY( InitBuffers() );
    VERIFY( InitProgram( deviceID ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
COCLTracer::~COCLTracer()
{
    OCL_ASSERT( clFlush( m_queue ) );
    OCL_ASSERT( clFinish( m_queue ) );
    OCL_ASSERT( clReleaseKernel( m_kernel ) );
    OCL_ASSERT( clReleaseProgram( m_program ) );
    OCL_ASSERT( clReleaseMemObject( m_sceneSphereBuffer ) );
    OCL_ASSERT( clReleaseMemObject( m_outColBuffer ) );
    OCL_ASSERT( clReleaseMemObject( m_rayPosDirBuffer ) );
    OCL_ASSERT( clReleaseCommandQueue( m_queue ) );
    OCL_ASSERT( clReleaseContext( m_context ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void COCLTracer::SetData( const CCamera& camera, const CScene& scene )
{
    // Camera
    cl_float8 *pPosDir = m_rayPosDirArray.get();
    for( int x = 0; x < m_sizeX; ++x )
        for( int y = 0; y < m_sizeY; ++y )
        {
            const int offset = y * m_sizeX + x;
            const float u = static_cast< float >( x ) / m_fullX;
            const float v = static_cast< float >( y ) / m_fullY;
            const NRayTrace::SRay ray = camera.GetRay( u, v );
            
            pPosDir[offset].s0 = ray.pos.x;
            pPosDir[offset].s1 = ray.pos.y;
            pPosDir[offset].s2 = ray.pos.z;
            pPosDir[offset].s3 = 1.0f;
            
            pPosDir[offset].s4 = ray.dir.x;
            pPosDir[offset].s5 = 0.0f;//ray.dir.y;
            pPosDir[offset].s6 = 0.0f;//ray.dir.z;
            pPosDir[offset].s7 = 1.0f;
        }
    
    // Scene
    cl_float4 *pSphere = m_sceneSphereArray.get();
    for( size_t i = 0; i < scene.GetSpheres().size(); ++i )
    {
        const Vec3& pos = scene.GetSpheres()[i]->GetPos();
        const float rad = scene.GetSpheres()[i]->GetRad();
        pSphere[i].x = pos.x;
        pSphere[i].y = pos.y;
        pSphere[i].z = pos.z;
        pSphere[i].w = rad;
    }
    
    // Write data to cl_mem
    OCL_ASSERT( clEnqueueWriteBuffer( m_queue, m_rayPosDirBuffer, CL_TRUE, 0, sizeof( cl_float8 ) * m_bufferSize, pPosDir, 0, nullptr, nullptr ) );
    OCL_ASSERT( clEnqueueWriteBuffer( m_queue, m_sceneSphereBuffer, CL_TRUE, 0, sizeof( cl_float4 ) * scene.GetSpheres().size(), pSphere, 0, nullptr, nullptr ) );
    
    // Transfer data to OpenCL
    OCL_ASSERT( clSetKernelArg( m_kernel, 0, sizeof( cl_mem ), (void*)&m_rayPosDirBuffer ) );
    OCL_ASSERT( clSetKernelArg( m_kernel, 1, sizeof( cl_mem ), (void*)&m_sceneSphereBuffer ) );
    OCL_ASSERT( clSetKernelArg( m_kernel, 2, sizeof( cl_mem ), (void*)&m_outColBuffer ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void COCLTracer::Trace()
{
    const size_t globalSize = m_bufferSize;
    const size_t localSize = 64;
    OCL_ASSERT( clEnqueueNDRangeKernel( m_queue, m_kernel, 1, nullptr, &globalSize, &localSize, 0, nullptr, nullptr ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void COCLTracer::GetBuffer( uint8_t *pBuffer )
{
    cl_float4 *pColor = m_outColArray.get();
    
    // Get output color buffer
    OCL_ASSERT( clEnqueueReadBuffer( m_queue, m_outColBuffer, CL_TRUE, 0, sizeof( cl_float4 ) * m_bufferSize, pColor, 0, nullptr, nullptr ) );
    
    // Write it to color
    for( size_t i = 0; i < m_bufferSize; ++i )
    {
        const SColor color( pColor[i].x, pColor[i].y, pColor[i].z, 1.0f );
        
        const size_t offset = i * 3;
        pBuffer[offset    ] = color.GetByteR();
        pBuffer[offset + 1] = color.GetByteG();
        pBuffer[offset + 2] = color.GetByteB();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool COCLTracer::InitBuffers()
{
    // Create arrays
    m_rayPosDirArray = std::unique_ptr< cl_float8 >( new cl_float8[m_bufferSize] );
    m_sceneSphereArray = std::unique_ptr< cl_float4 >( new cl_float4[MAX_SPHERE_COUNT] );
    m_outColArray = std::unique_ptr< cl_float4 >( new cl_float4[m_bufferSize] );
    
    // Create buffers
    cl_int errorCode = 0;
    m_rayPosDirBuffer = clCreateBuffer( m_context, CL_MEM_READ_ONLY, sizeof( cl_float8 ) * m_bufferSize, nullptr, &errorCode );
    OCL_VERIFY( m_rayPosDirBuffer, errorCode );
    m_sceneSphereBuffer = clCreateBuffer( m_context, CL_MEM_READ_ONLY, sizeof( cl_float4 ) * MAX_SPHERE_COUNT, nullptr, &errorCode );
    OCL_VERIFY( m_sceneSphereBuffer, errorCode );
    m_outColBuffer = clCreateBuffer( m_context, CL_MEM_WRITE_ONLY, sizeof( cl_float4 ) * m_bufferSize, nullptr, &errorCode );
    OCL_VERIFY( m_outColBuffer, errorCode );
    
    
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool COCLTracer::InitProgram( const cl_device_id& deviceID )
{
    // Load file and define its size
    const std::string filename = NCore::FileSystem().GetResourceFileName( "Data/OpenCL/RayTrace.cl" );
    NCore::CFile file( filename.c_str() );
    CHECK( file.IsOpen(), "Can't open OpenCL file", return false );
    
    std::unique_ptr< char[] > buffer( new char[file.GetSize() + 1] );
    const char *pCode = buffer.get();
    CHECK( file.Load( buffer.get() ), "Can't load OpenCL file to buffer", return false );
    
    // Create
    cl_int errorCode = 0;
    m_program = clCreateProgramWithSource ( m_context, 1, &pCode, nullptr, &errorCode );
    OCL_VERIFY( m_program, errorCode );
    
    // Build
    const cl_int res = clBuildProgram ( m_program, 1, &deviceID, nullptr, nullptr, nullptr );
    if( res != CL_SUCCESS )
    {
        size_t logSize = 0;
        OCL_ASSERT( clGetProgramBuildInfo( m_program, deviceID, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize ) );
        std::unique_ptr< char[] > log( new char[logSize + 1] );
        char *pLog = log.get();
        pLog[logSize] = 0;
        OCL_ASSERT( clGetProgramBuildInfo( m_program, deviceID, CL_PROGRAM_BUILD_LOG, logSize, pLog, nullptr ) );
        std::cout << "Log:" << std::endl;
        std::cout << pLog << std::endl;
        return false;
    }
    
    // Get kernel
    m_kernel = clCreateKernel( m_program, "traceRay", &errorCode );
    OCL_VERIFY( m_kernel, errorCode );
    
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
