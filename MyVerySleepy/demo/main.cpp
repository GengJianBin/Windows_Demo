#include "utils/dbginterface.h"
#include "profiler/profilerthread.h"
#include "profiler/debugger.h"
#include "utils/osutils.h"
#include "profiler/symbolinfo.h"
#include "api/PerfProfInterface.h"
#include <tlhelp32.h>
#include <iostream>
#include <thread>

#define RUN_STATIC_LIBRARY 0

void symLogCallback(const wchar_t* text)
{
    std::cout << "sysLog: " << text << std::endl;
}

HANDLE GetProcessHandleByPID(DWORD dwPID, DWORD dwDesiredAccess) {
    HANDLE hProcess = NULL;
    hProcess = OpenProcess(
        dwDesiredAccess, 
        FALSE,            
        dwPID             
    );

    if (hProcess == NULL) {
        DWORD dwError = GetLastError();
        switch (dwError) {
            case ERROR_ACCESS_DENIED:
                printf("错误：权限不足（需管理员权限或目标进程受保护）\n");
                break;
            case ERROR_INVALID_PARAMETER:
                printf("错误：无效的PID（PID为0或超出范围）\n");
                break;
            case ERROR_INVALID_HANDLE:
                printf("错误：句柄无效（系统资源不足）\n");
                break;
            default:
                printf("错误：获取句柄失败，错误码：%lu\n", dwError);
                break;
        }
    } else {
        printf("成功获取进程句柄，句柄值：%p\n", hProcess);
    }
    return hProcess;
}

void FreeThreadHandles(std::vector<HANDLE>& handles) {
    for (HANDLE hThread : handles) {
        if (hThread != NULL && hThread != INVALID_HANDLE_VALUE) {
            CloseHandle(hThread);
        }
    }
    handles.clear();
}

std::vector<HANDLE> GetAllThreadHandles(DWORD dwProcessId, DWORD dwDesiredAccess) {
    std::vector<HANDLE> threadHandles;
    HANDLE hSnapshot = INVALID_HANDLE_VALUE;

    // 创建线程快照
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("创建线程快照失败，错误码: %lu\n", GetLastError());
        return threadHandles;
    }

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(hSnapshot, &te32)) {
        printf("获取线程信息失败，错误码: %lu\n", GetLastError());
        CloseHandle(hSnapshot);
        return threadHandles;
    }

    do {
        if (te32.th32OwnerProcessID == dwProcessId) {
            HANDLE hThread = OpenThread(dwDesiredAccess, FALSE, te32.th32ThreadID);
            if (hThread == NULL) {
                printf("打开线程 %lu 失败，错误码: %lu\n", te32.th32ThreadID, GetLastError());
            } else {
                threadHandles.push_back(hThread);
                printf("成功获取线程句柄，线程ID: %lu, 句柄: %p\n", te32.th32ThreadID, hThread);
            }
        }
    } while (Thread32Next(hSnapshot, &te32));

    //关闭句柄
    CloseHandle(hSnapshot);
    return threadHandles;
}

bool CaptureByStaticLib(DWORD pid){
     //1. set system log callback
    g_symLog = symLogCallback;

    //2. init symbol information
    InitSysInfo();

    //3. init dbghelp
    if (!dbgHelpInit()) {
        return false;
    }

	Debugger* debugger = NULL;
	
    HANDLE pHandle = GetProcessHandleByPID(pid, PROCESS_ALL_ACCESS);

    DWORD dwDesiredAccess = THREAD_ALL_ACCESS;
    std::vector<HANDLE> vecTheadHandles = GetAllThreadHandles(pid, dwDesiredAccess);

    bool bProfile = CanProfileProcess(pHandle);
    if (!bProfile) {
        std::cout << "This process can't be profile,this process id is " << pid << std::endl;
    }
    SymbolInfo* pSymInfo =  new SymbolInfo();

    if (pHandle)
    {
        pSymInfo->loadSymbols(pHandle, false);
    }

	ProfilerThread* profilerthread = new ProfilerThread(
        pHandle,
        vecTheadHandles,
        pSymInfo,
		debugger,
        L"C:\\Users\\13684\\AppData\\Roaming\\MysleepTestDir\\test20251010.sleepy"
	);

    profilerthread->launch(THREAD_PRIORITY_TIME_CRITICAL);


    std::this_thread::sleep_for(std::chrono::seconds(10));

    profilerthread->commitSuicide();

    if (profilerthread->getDone() || profilerthread->getFailed()) {
         
    }
    else {
        std::cout << "success:" << profilerthread->getDone() << " failed:" << profilerthread->getFailed() << std::endl;
    }

    profilerthread->waitFor(100);

    profilerthread->join();


    if (pSymInfo) {
        delete pSymInfo;
        pSymInfo = nullptr;
    }

    FreeThreadHandles(vecTheadHandles);

    std::wstring output_filename = profilerthread->getFilename();
    std::wcout << "filename is :" << output_filename << std::endl;

    if (profilerthread) {
        delete profilerthread;
        profilerthread = nullptr;
    }
    CloseHandle(pHandle);
}

bool CaptureByDynamicLib(DWORD pid){
    bool bRet = false;
    typedef PerfProfInterface* (*ExpCreatePerfProfIns)();
    typedef void (*ExpDestroyPerfProfIns)(PerfProfInterface*);
    ExpCreatePerfProfIns myExpCreatePerfProfIns = nullptr;
    ExpDestroyPerfProfIns myExpDestroyPerfProfIns = nullptr;
    HMODULE hDll =  nullptr;
    do{
        // 1. loadlibrary
        hDll = LoadLibraryA("PerfProf.dll");
        if (hDll == NULL) {
            std::cerr << "Load DLL failed!error:" << GetLastError() << std::endl;
            break;
        }

        // 2. get function address
        ExpCreatePerfProfIns myExpCreatePerfProfIns = (ExpCreatePerfProfIns)GetProcAddress(hDll, "CreatePerfProfIns");
        ExpDestroyPerfProfIns myExpDestroyPerfProfIns = (ExpDestroyPerfProfIns)GetProcAddress(hDll, "DestroyPerfProfIns");

        if (myExpCreatePerfProfIns == nullptr || myExpDestroyPerfProfIns == nullptr) {
            std::cerr << "get function addreee failed erro is " << GetLastError() << std::endl;
            FreeLibrary(hDll);  //release DLL
            break;
        }


        PerfProfInterface* pIns = myExpCreatePerfProfIns();
        if(pIns){
            pIns->set_capture_time(20);
            pIns->set_capture_path(L"C:\\Users\\13684\\AppData\\Roaming\\MysleepTestDir\\test20251017.sleepy");
            pIns->start_capture(pid);
            myExpDestroyPerfProfIns(pIns);
            bRet = true;
            break;
        }
    }while(false);
    if(hDll){
        FreeLibrary(hDll);
        hDll = nullptr;
    }
    return bRet; 
}

int main() {
    std::cout << "Hello, CMake Project!" << std::endl;

    std::cout << "Please Input The Process ID To Capture:" << std::endl;
    DWORD pId = 0;
    std::cin >> pId;
#if RUN_STATIC_LIBRARY
    CaptureByStaticLib();
#else
    CaptureByDynamicLib(pId);
#endif
    return 0;
}