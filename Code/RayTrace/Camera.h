#pragma once

#include "Ray.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class CCamera
{
public:
    CCamera( const float aspect, const float near );
    SRay GetRay( const float u, const float v ) const;
    
private:
    Vec3 m_pos;
    Vec3 m_vecU;
    Vec3 m_vecV;
    Vec3 m_lowerLeftCorner;
    float m_near;
    float m_aspect;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
