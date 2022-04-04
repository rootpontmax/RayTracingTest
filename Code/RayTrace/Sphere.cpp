#include "Sphere.h"
#include "../Core/Assert.h"
#include "Ray.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSphere::HasHit( SHitInfo *pInfo, const SRay& ray, const float tMin, const float tMax ) const
{
    ASSERT( pInfo, "Pointer must be valid. Output value" );
    
    const Vec3 os = ray.pos - m_pos;
    const float a = ray.dir.LengthSqr();
    const float halfB = Dot( os, ray.dir );
    const float c = os.LengthSqr() - m_radSqr;
    const float d = halfB * halfB - a * c;
    if( d < 0.0f )
        return false;
    
    const float dSqrt = sqrt( d );
    float root = ( -halfB - dSqrt ) / a;
    if( root < tMin || root > tMax )
    {
        root = ( -halfB + dSqrt ) / a;
        if( root < tMin || root > tMax )
            return false;
    }
    
    pInfo->t = root;
    pInfo->pos = ray.PointAt(root);
    pInfo->nor = ( pInfo->pos - m_pos ) / m_rad;
    
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
