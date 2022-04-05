#include "Camera.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
CCamera::CCamera( const float aspect, const float near ) :
    m_aspect( aspect ),
    m_near( near )
{
    const float viewPortSizeY = 2.0f;
    const float viewPortSizeX = viewPortSizeY * m_aspect;
    
    m_pos = Vec3( 0.0f, 0.0f, 0.0f );
    m_vecU = Vec3( viewPortSizeX, 0.0f, 0.0f );
    m_vecV = Vec3( 0.0f, viewPortSizeY, 0.0f );
    m_lowerLeftCorner = m_pos - m_vecU * 0.5f - m_vecV * 0.5f - Vec3( 0.0f, 0.0f, m_near );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SRay CCamera::GetRay( const float u, const float v ) const
{
    const Vec3 dir = m_lowerLeftCorner + u * m_vecU + v * m_vecV;
    return NRayTrace::SRay( m_pos, dir );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
