////////////////////////////////////////////////////////////////////////////////////////////////////
// Класс пула потоков, которые выполняют какую-либо работу.                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
//#include "Core.h"
#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <cassert>

namespace NCore
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class CThreadPool// : public NCore::CNonCopyAble
{
public:
    CThreadPool( const size_t threadCount );
    CThreadPool();
    ~CThreadPool();
    
    //void    Init( const size_t threadCount );
    void    WaitAllDone();
    
    // Добавление работы в пул
    template< typename F, typename... Args >
    auto Add( F&& f, Args&&... args )  ->std::future< decltype( f( 0, args... ) ) >
    {
        auto task = std::make_shared< std::packaged_task< decltype(f(0, args...))(const size_t)>>(
                std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Args>(args)...));
        auto res = task->get_future();
        {
            std::unique_lock< std::mutex > guard( m_queueMutex );
            assert( !m_bIsStopped );
            m_taskQueue.emplace( [task]( const size_t threadID ) { (*task)( threadID ); } );
            ++m_balance;
        }
        m_condVar.notify_one();        
        return res;
    }
    
private:
    
    void    ThreadFunc( const size_t threadID );
    
    typedef std::function< void( const size_t threadID ) > TTask;
    
    
    std::vector< std::thread >              m_threadPool;
    std::queue< TTask >                     m_taskQueue;
    std::mutex                              m_queueMutex;
    std::condition_variable                 m_condVar;
    std::mutex                              m_doneMutex;
    std::condition_variable                 m_condVarDone;
    std::atomic<int>                        m_balance;
    std::atomic<bool>                       m_bIsStopped;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
} // end of namespace
