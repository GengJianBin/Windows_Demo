#include <iostream>
#include "utils/dbginterface.h"
#include "profiler/profilerthread.h"
#include "profiler/debugger.h"
#include <tlhelp32.h>
#include <thread>

HANDLE GetProcessHandleByPID(DWORD dwPID, DWORD dwDesiredAccess) {
    HANDLE hProcess = NULL;
    // ����OpenProcess�򿪽���
    hProcess = OpenProcess(
        dwDesiredAccess,  // ����Ȩ��
        FALSE,            // ������̳�
        dwPID             // Ŀ��PID
    );

    // ������
    if (hProcess == NULL) {
        DWORD dwError = GetLastError();
        switch (dwError) {
        case ERROR_ACCESS_DENIED:
            printf("����Ȩ�޲��㣨�����ԱȨ�޻�Ŀ������ܱ�����\n");
            break;
        case ERROR_INVALID_PARAMETER:
            printf("������Ч��PID��PIDΪ0�򳬳���Χ��\n");
            break;
        case ERROR_INVALID_HANDLE:
            printf("���󣺾����Ч��ϵͳ��Դ���㣩\n");
            break;
        default:
            printf("���󣺻�ȡ���ʧ�ܣ������룺%lu\n", dwError);
            break;
        }
    }
    else {
        printf("�ɹ���ȡ���̾�������ֵ��%p\n", hProcess);
    }
    return hProcess;
}

// �ͷ��߳̾������
void FreeThreadHandles(std::vector<HANDLE>& handles) {
    for (HANDLE hThread : handles) {
        if (hThread != NULL && hThread != INVALID_HANDLE_VALUE) {
            CloseHandle(hThread);
        }
    }
    handles.clear();
}

// ��ȡָ�������������̵߳ľ��
std::vector<HANDLE> GetAllThreadHandles(DWORD dwProcessId, DWORD dwDesiredAccess) {
    std::vector<HANDLE> threadHandles;
    HANDLE hSnapshot = INVALID_HANDLE_VALUE;

    // �����߳̿���
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("�����߳̿���ʧ�ܣ�������: %lu\n", GetLastError());
        return threadHandles;
    }

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    // ��ȡ��һ���߳���Ϣ
    if (!Thread32First(hSnapshot, &te32)) {
        printf("��ȡ�߳���Ϣʧ�ܣ�������: %lu\n", GetLastError());
        CloseHandle(hSnapshot);
        return threadHandles;
    }

    // ���������߳�
    do {
        // ����߳��Ƿ�����Ŀ�����
        if (te32.th32OwnerProcessID == dwProcessId) {
            // ���̻߳�ȡ���
            HANDLE hThread = OpenThread(dwDesiredAccess, FALSE, te32.th32ThreadID);
            if (hThread == NULL) {
                printf("���߳� %lu ʧ�ܣ�������: %lu\n", te32.th32ThreadID, GetLastError());
            }
            else {
                threadHandles.push_back(hThread);
                printf("�ɹ���ȡ�߳̾�����߳�ID: %lu, ���: %p\n", te32.th32ThreadID, hThread);
            }
        }
    } while (Thread32Next(hSnapshot, &te32));

    // �رտ��վ��
    CloseHandle(hSnapshot);
    return threadHandles;
}


int main() {
    std::cout << "Hello, CMake Project!" << std::endl;

	Debugger* debugger = NULL;
	DWORD pId = 30928;
    HANDLE pHandle = GetProcessHandleByPID(pId, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);

    DWORD dwDesiredAccess = THREAD_QUERY_INFORMATION | THREAD_SUSPEND_RESUME | THREAD_TERMINATE;
    std::vector<HANDLE> vecTheadHandles = GetAllThreadHandles(pId, dwDesiredAccess);

    SymbolInfo* pSymInfo =  new SymbolInfo();
	ProfilerThread* profilerthread = new ProfilerThread(
        pHandle,
        vecTheadHandles,
        pSymInfo,
		debugger
	);

    profilerthread->launch(THREAD_PRIORITY_TIME_CRITICAL);


    std::this_thread::sleep_for(std::chrono::seconds(3));

    profilerthread->commitSuicide();

    if (profilerthread->getDone() || profilerthread->getFailed()) {
         
    }
    else {
        std::cout << "success:" << profilerthread->getDone() << " fauled:" << profilerthread->getFailed() << std::endl;
    }

    if (pSymInfo) {
        delete pSymInfo;
        pSymInfo = nullptr;
    }

    profilerthread->waitFor(100);

    profilerthread->join();

    FreeThreadHandles(vecTheadHandles);

    std::wstring output_filename = profilerthread->getFilename();
    std::wcout << "filename is :" << output_filename << std::endl;

    if (profilerthread) {
        delete profilerthread;
        profilerthread = nullptr;
    }
    return 0;
}