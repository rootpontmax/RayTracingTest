#pragma once
#include "../Core/OCLDefs.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class COCLTracer
{
public:
    COCLTracer( const int sizeX, const int sizeY );
    ~COCLTracer();
private:
    
    cl_context          m_context;
    cl_command_queue    m_queue;
    cl_mem              m_rayPosBuffer;
    cl_mem              m_rayDirBuffer;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
