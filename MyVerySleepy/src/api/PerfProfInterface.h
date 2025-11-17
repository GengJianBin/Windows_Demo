#ifndef __PERF_PROF_INTERFACE_H_
#define __PERF_PROF_INTERFACE_H_

#include <string>

class PerfProfInterface{
public:
    virtual int start_capture(unsigned long pid) = 0;
    virtual void set_capture_time(uint64_t sec) = 0;
    virtual void set_capture_path(std::wstring path) = 0;
};

extern "C" __declspec(dllexport) PerfProfInterface* CreatePerfProfIns();
extern "C" __declspec(dllexport) void DestroyPerfProfIns(PerfProfInterface* pInst);

#endif //__PERF_PROF_INTERFACE_H_