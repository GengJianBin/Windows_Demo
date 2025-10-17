
#pragma once
#ifndef __PERF_PROF_IMP_H_
#define __PERF_PROF_IMP_H_

#include "PerfProfInterface.h"
#include "utils/dbginterface.h"
#include "profiler/profilerthread.h"
#include "profiler/debugger.h"
#include "utils/osutils.h"
#include "profiler/symbolinfo.h"
#include <tlhelp32.h>
#include <thread>
#include <iostream>

class PerfProfImp : public PerfProfInterface{
public:
    PerfProfImp();
    virtual ~PerfProfImp();
    int init();

    //override PerfProfInterface
    virtual int start_capture(unsigned long pid) override;
    virtual void set_capture_time(uint64_t sec) override;
    virtual void set_capture_path(std::wstring path) override;
private:
    static void sym_log_callback(const wchar_t* text);

    HANDLE get_process_handle_by_pid(DWORD dwPID, DWORD dwDesiredAccess);
    void free_thread_handles(std::vector<HANDLE>& handles);
    std::vector<HANDLE> get_all_thread_handles(DWORD dwProcessId, DWORD dwDesiredAccess);

    uint64_t capture_time_ = 10;
    std::wstring file_saved_path = L"";
};



#endif//__PERF_PROF_IMP_H_