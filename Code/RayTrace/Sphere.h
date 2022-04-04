////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../Core/Vec3.h"
#include "Hitable.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class CSphere : public IHitable
{
public:
    CSphere( const Vec3& pos, const float rad ) :
        m_pos( pos ),
        m_rad( rad ),
        m_radSqr( rad * rad )
    {}
    
    // IHitable
    bool HasHit( SHitInfo *pInfo, const SRay& ray, const float tMin, const float tMax ) const override;
    
private:
    const Vec3 m_pos;
    const float m_rad;
    const float m_radSqr;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
