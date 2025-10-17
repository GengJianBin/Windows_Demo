#include "PerfProfImp.h"

PerfProfInterface* CreatePerfProfIns(){
    PerfProfImp* pIns = new PerfProfImp();
    int iRet = pIns->init();
    if(iRet != 0){
        return nullptr;
    }
    else{
        return pIns;
    }
}

void DestroyPerfProfIns(PerfProfInterface* pInst){
    if(pInst){
        delete pInst;
        pInst = nullptr;
    }
}

PerfProfImp::PerfProfImp(){
    file_saved_path = L"D:\\test.sleepy";
}

PerfProfImp::~PerfProfImp(){

}

int PerfProfImp::start_capture(unsigned long pid){
    std::cout << "start_capture enter" << std::endl;
    
	Debugger* debugger = NULL;
	
    HANDLE pHandle = this->get_process_handle_by_pid(pid, PROCESS_ALL_ACCESS);

    DWORD dwDesiredAccess = THREAD_ALL_ACCESS;
    std::vector<HANDLE> vecTheadHandles = this->get_all_thread_handles(pid, dwDesiredAccess);

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
        file_saved_path
	);

    profilerthread->launch(THREAD_PRIORITY_TIME_CRITICAL);


    std::this_thread::sleep_for(std::chrono::seconds(capture_time_));

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

    free_thread_handles(vecTheadHandles);

    std::wstring output_filename = profilerthread->getFilename();
    std::wcout << "filename is :" << output_filename << std::endl;

    if (profilerthread) {
        delete profilerthread;
        profilerthread = nullptr;
    }
    CloseHandle(pHandle);
    return 0;
}

void PerfProfImp::set_capture_time(uint64_t sec){
    capture_time_ = sec;
}

void PerfProfImp::set_capture_path(std::wstring path){
    file_saved_path = path;
}

void PerfProfImp::sym_log_callback(const wchar_t* text)
{
    std::cout << "sysLog: " << text << std::endl;
}

int PerfProfImp::init(){
    int iRet = 0;
    do{
        //1. set system log callback
        g_symLog = PerfProfImp::sym_log_callback;
        //2. init symbol information
        InitSysInfo();

        //3. init dbghelp
        if (!dbgHelpInit()) {
            iRet = -1;
        }
    }while(false);
    return iRet;
}

HANDLE PerfProfImp::get_process_handle_by_pid(DWORD dwPID, DWORD dwDesiredAccess) {
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

void PerfProfImp::free_thread_handles(std::vector<HANDLE>& handles) {
    for (HANDLE hThread : handles) {
        if (hThread != NULL && hThread != INVALID_HANDLE_VALUE) {
            CloseHandle(hThread);
        }
    }
    handles.clear();
}

std::vector<HANDLE> PerfProfImp::get_all_thread_handles(DWORD dwProcessId, DWORD dwDesiredAccess) {
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