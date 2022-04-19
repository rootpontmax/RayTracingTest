#pragma once

#include <vector>

namespace NRayTrace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
struct IHitable;
class CSphere;
////////////////////////////////////////////////////////////////////////////////////////////////////
class CScene
{
public:
    typedef std::vector< const IHitable* >  THitableArray;
    typedef std::vector< const CSphere* >   TSphereArray;
    
    void AddSphere( const CSphere *pSphere );
    
    
    const THitableArray& GetHitables() const { return m_hitables; }
    const TSphereArray& GetSpheres() const { return m_spheres; }
    
private:
    
    THitableArray   m_hitables;
    TSphereArray    m_spheres;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
