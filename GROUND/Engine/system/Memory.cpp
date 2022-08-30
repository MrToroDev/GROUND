#include "Memory.h"
#include <glad/glad.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>

// Nvidia GPU Info
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049


int gr::Memory::GetBufferSize(unsigned int buffer)
{
    int size = 0;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    return size;
}

int gr::Memory::VRAMtotalSize()
{
    GLint kb = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &kb);
    return kb;
}

int gr::Memory::VRAMcurrentAvailable()
{
    GLint Usagekb = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &Usagekb);
    return Usagekb;
}

int gr::Memory::VRAMcurrentUsage()
{
    GLint Usagekb = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &Usagekb);
    GLint Totalkb = 0;
    glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &Totalkb);
    return Totalkb - Usagekb;
}

bool gr::Memory::IsProcessRunning(const std::wstring exec)
{
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, FindProcessId(exec));
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return ret == WAIT_TIMEOUT;
}

unsigned long gr::Memory::FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}
