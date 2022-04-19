#pragma once
#include <chrono>

namespace NCore
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class CTimer
{
public:
    CTimer();
    void    Start();
    void    Stop();
    float   GetMS() const { return m_durationMS.count(); }
private:
    
    std::chrono::high_resolution_clock::time_point  m_timePointA;
    std::chrono::high_resolution_clock::time_point  m_timePointB;
    std::chrono::duration<float, std::milli>        m_durationMS;
    bool                                            m_bIsWorking;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
