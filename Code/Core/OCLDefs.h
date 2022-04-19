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
        case CL_BUILD_PROGRAM_FAILURE:
            BREAK( "CL_BUILD_PROGRAM_FAILURE" );
            break;
            
        case CL_COMPILER_NOT_AVAILABLE:
            BREAK( "CL_COMPILER_NOT_AVAILABLE" );
            break;
            
        case CL_DEVICE_NOT_AVAILABLE:
            BREAK( "CL_DEVICE_NOT_AVAILABLE" );
            break;
            
        case CL_DEVICE_NOT_FOUND:
            BREAK( "CL_DEVICE_NOT_FOUND" );
            break;
            
        case CL_INVALID_BINARY:
            BREAK( "CL_INVALID_BINARY" );
            break;
            
        case CL_INVALID_BUILD_OPTIONS:
            BREAK( "CL_INVALID_BUILD_OPTIONS" );
            break;
            
        case CL_INVALID_COMMAND_QUEUE:
            BREAK( "CL_INVALID_COMMAND_QUEUE" );
            break;
            
        case CL_INVALID_CONTEXT:
            BREAK( "CL_INVALID_CONTEXT" );
            break;
            
        case CL_INVALID_DEVICE:
            BREAK( "CL_INVALID_DEVICE" );
            break;
            
        case CL_INVALID_DEVICE_TYPE:
            BREAK( "CL_INVALID_DEVICE_TYPE" );
            break;
            
        case CL_INVALID_GLOBAL_OFFSET:
            BREAK( "CL_INVALID_GLOBAL_OFFSET" );
            break;
            
        case CL_INVALID_EVENT_WAIT_LIST:
            BREAK( "CL_INVALID_EVENT_WAIT_LIST" );
            break;
            
        case CL_INVALID_KERNEL:
            BREAK( "CL_INVALID_KERNEL" );
            break;
            
        case CL_INVALID_KERNEL_ARGS:
            BREAK( "CL_INVALID_KERNEL_ARGS" );
            break;
            
        case CL_INVALID_KERNEL_DEFINITION:
            BREAK( "CL_INVALID_KERNEL_DEFINITION" );
            break;
            
        case CL_INVALID_KERNEL_NAME:
            BREAK( "CL_INVALID_KERNEL_NAME" );
            break;
            
        case CL_INVALID_MEM_OBJECT:
            BREAK( "CL_INVALID_MEM_OBJECT" );
            break;
            
        case CL_INVALID_OPERATION:
            BREAK( "CL_INVALID_OPERATION" );
            break;
            
        case CL_INVALID_PLATFORM:
            BREAK( "CL_INVALID_PLATFORM" );
            break;
            
        case CL_INVALID_PROGRAM:
            BREAK( "CL_INVALID_PROGRAM" );
            break;
            
        case CL_INVALID_PROGRAM_EXECUTABLE:
            BREAK( "CL_INVALID_PROGRAM_EXECUTABLE" );
            break;
            
        case CL_INVALID_WORK_DIMENSION:
            BREAK( "CL_INVALID_WORK_DIMENSION" );
            break;
            
        case CL_INVALID_WORK_GROUP_SIZE:
            BREAK( "CL_INVALID_WORK_GROUP_SIZE" );
            break;
            
        case CL_INVALID_WORK_ITEM_SIZE:
            BREAK( "CL_INVALID_WORK_ITEM_SIZE" );
            break;
            
        case CL_INVALID_VALUE:
            BREAK( "CL_INVALID_VALUE" );
            break;
            
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            BREAK( "CL_MEM_OBJECT_ALLOCATION_FAILURE" );
            break;
            
        case CL_OUT_OF_HOST_MEMORY:
            BREAK( "CL_OUT_OF_HOST_MEMORY" );
            break;
            
        case CL_OUT_OF_RESOURCES:
            BREAK( "CL_OUT_OF_RESOURCES" );
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
        if( !pointer || CL_SUCCESS != errorCode )                                               \
            ShowErrorOpenCL( errorCode );                                                       \
    }
#else // DEBUG
#define OCL_VERIFY( pointer, errorCode )
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
