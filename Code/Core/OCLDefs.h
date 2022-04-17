#pragma once

#include "Assert.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
static void ShowErrorOpenCL( cl_int errorCode )
{
    switch( errorCode )
    {
        case CL_INVALID_PLATFORM:
            BREAK( "CL_INVALID_PLATFORM" );
            break;
            
        case CL_INVALID_DEVICE_TYPE:
            BREAK( "CL_INVALID_DEVICE_TYPE" );
            break;
            
        case CL_INVALID_DEVICE:
            BREAK( "CL_INVALID_DEVICE" );
            break;
            
        case CL_INVALID_VALUE:
            BREAK( "CL_INVALID_VALUE" );
            break;
            
        case CL_DEVICE_NOT_FOUND:
            BREAK( "CL_DEVICE_NOT_FOUND" );
            break;
            
        case CL_DEVICE_NOT_AVAILABLE:
            BREAK( "CL_DEVICE_NOT_AVAILABLE" );
            break;
            
        case CL_OUT_OF_HOST_MEMORY:
            BREAK( "CL_OUT_OF_HOST_MEMORY" );
            break;
            
        default:
            BREAK( "Unknown type of OpenCL error" );
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
#define OCL_ASSERT( x )                                                                         \
    {                                                                                           \
        const cl_int res = x;                                                                   \
        if( CL_SUCCESS != res )                                                                 \
        {                                                                                       \
            ShowErrorOpenCL( res );                                                             \
        }                                                                                       \
    }
#else // DEBUG
#define  OCL_ASSERT(x) x
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
#define OCL_VERIFY( pointer, errorCode )                                                        \
    {                                                                                           \
        if( pointer && CL_SUCCESS != errorCode )                                                \
            ShowErrorOpenCL( errorCode );                                                       \
    }
#else // DEBUG
#define OCL_VERIFY( pointer, errorCode )
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
