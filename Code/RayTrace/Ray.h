////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../Core/Vec3.h"

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SRay
{
    SRay( const Vec3& p, const Vec3& d ) :
        pos( p ),
        dir( d )
    {}
    
    Vec3 PointAt( const float coef ) const
    {
        return pos + dir * coef;
    }
    
    Vec3    pos;
    Vec3    dir;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace