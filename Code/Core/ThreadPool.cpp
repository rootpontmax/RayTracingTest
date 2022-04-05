#include "ThreadPool.h"

namespace NCore
{
////////////////////////////////////////////////////////////////////////////////////////////////////
CThreadPool::CThreadPool( const size_t threadCount ) :
    m_balance( 0 ),
    m_bIsStopped( false )
{
    m_threadPool.reserve( threadCount );
    for( size_t i = 0; i < threadCount; ++i )
        m_threadPool.emplace_back( &CThreadPool::ThreadFunc, this, i );
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////
CThreadPool::CThreadPool() :
    m_balance( 0 ),
    m_bIsStopped( false )
{}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
CThreadPool::~CThreadPool()
{
    m_bIsStopped = true;
    m_condVar.notify_all();
    for( size_t i = 0; i < m_threadPool.size(); ++i )
        m_threadPool[i].join();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadPool::ThreadFunc( const size_t threadID )
{
    for( ; ; )
    {
        TTask currentTask;
        
        {
            std::unique_lock< std::mutex > guard( m_queueMutex );
                        
            m_condVar.wait( guard, [this]()
            {
                return ( !m_taskQueue.empty() || m_bIsStopped );
            });
                        
            if( m_bIsStopped || m_taskQueue.empty() )
                return;
                        
            currentTask = std::move( m_taskQueue.front() );
            m_taskQueue.pop();
        }
        currentTask( threadID );
                    
        std::unique_lock< std::mutex > guard( m_doneMutex );
        --m_balance;
        m_condVarDone.notify_one();
    }
    
    /*
    std::cout << "Thread func " << threadID << " started" << std::endl;
    
    for( ; ; )
    {
    }
    
    //std::unique_lock< std::mutex > guard( m_queueMutex );
    
    std::cout << "Thread func " << threadID << " finished" << std::endl;
    */
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadPool::Init( const size_t threadCount )
{
    auto threadFunc = [this]( const size_t threadID )
    {
        for( ; ; )
        {
            std::function< void( const size_t threadID ) > task;
            {
                std::unique_lock< std::mutex > guard( m_queueMutex );
                            
                m_condVar.wait( guard, [this]()
                {
                    return ( !m_taskQueue.empty() || m_bIsStopped );
                });
                            
                if( m_bIsStopped || m_taskQueue.empty() )
                    return;
                            
                task = std::move( m_taskQueue.front() );
                m_taskQueue.pop();
            }
            task( threadID );
                        
            std::unique_lock< std::mutex > guard( m_doneMutex );
            --m_balance;
            m_condVarDone.notify_one();
        }
    };
        
    m_threadPool.reserve( threadCount );
    for( size_t i = 0; i < threadCount; ++i )
        m_threadPool.emplace_back( threadFunc, i );
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadPool::WaitAllDone()
{
    std::unique_lock< std::mutex > guard( m_doneMutex );
    m_condVarDone.wait( guard, [this]()
    {
        return ( 0 == m_balance );
    });
}
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
