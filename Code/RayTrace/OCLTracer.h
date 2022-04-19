#pragma once

#include <memory>
#include "../Core/OCLDefs.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class CCamera;
class CScene;
////////////////////////////////////////////////////////////////////////////////////////////////////
class COCLTracer
{
public:
    COCLTracer( const int sizeX, const int sizeY );
    ~COCLTracer();
    
    void SetData( const CCamera& camera, const CScene& scene );
    void Trace();
    void GetBuffer( uint8_t *pBuffer );
    
private:
    
    bool    InitBuffers();
    bool    InitProgram( const cl_device_id& deviceID );
        
    const int           m_sizeX;
    const int           m_sizeY;
    const float         m_fullX;
    const float         m_fullY;
    const size_t        m_bufferSize;
    
    cl_context          m_context;
    cl_command_queue    m_queue;
    cl_program          m_program;
    cl_kernel           m_kernel;
    cl_mem              m_rayPosDirBuffer;
    cl_mem              m_outColBuffer;
    cl_mem              m_sceneSphereBuffer;
    
    // Buffers
    std::unique_ptr< cl_float8 >    m_rayPosDirArray;
    std::unique_ptr< cl_float4 >    m_sceneSphereArray;
    std::unique_ptr< cl_float4 >    m_outColArray;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
