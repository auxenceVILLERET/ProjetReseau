#ifndef THREAD_H_DEFINED
#define THREAD_H_DEFINED
#include <functional>
#include <windows.h>

class Thread
{
public:
    Thread();
    ~Thread();

    void Init();
    DWORD WINAPI ThreadProcedure(LPVOID lpParam);
    
private:
    std::function<void()> m_threadFunction;
};

#endif
