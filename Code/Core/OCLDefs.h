#pragma once

#include "Assert.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
#define OCL_ASSERT( x )                                                                         \
    {                                                                                           \
        const cl_int res = x;                                                                   \
        if( CL_SUCCESS != res )                                                                 \
        {                                                                                       \
            switch( res )                                                                       \
            {                                                                                   \
                case CL_INVALID_PLATFORM:                                                       \
                    BREAK( "CL_INVALID_PLATFORM" );                                             \
                    break;                                                                      \
                case CL_INVALID_DEVICE_TYPE:                                                    \
                    BREAK( "CL_INVALID_DEVICE_TYPE" );                                          \
                    break;                                                                      \
                case CL_INVALID_DEVICE:                                                         \
                    BREAK( "CL_INVALID_DEVICE" );                                               \
                    break;                                                                      \
                case CL_INVALID_VALUE:                                                          \
                    BREAK( "CL_INVALID_VALUE" );                                                \
                    break;                                                                      \
                case CL_DEVICE_NOT_FOUND:                                                       \
                    BREAK( "CL_DEVICE_NOT_FOUND" );                                             \
                    break;                                                                      \
                case CL_DEVICE_NOT_AVAILABLE:                                                   \
                    BREAK( "CL_DEVICE_NOT_AVAILABLE" );                                         \
                    break;                                                                      \
                case CL_OUT_OF_HOST_MEMORY:                                                     \
                    BREAK( "CL_OUT_OF_HOST_MEMORY" );                                           \
                    break;                                                                      \
                default:                                                                        \
                    BREAK( "Unknown type of OpenCL error" );                                    \
                    break;                                                                      \
            }                                                                                   \
        }                                                                                       \
    }
#else // DEBUG
#define  OCL_ASSERT(x) x
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
