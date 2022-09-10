#include <Windows.h>
#include <TlHelp32.h>

DWORD m_dwPID = 0x0;
HANDLE m_hProc = 0;

template <typename T>
T Read(DWORD address)
{
	T buffer;
	ReadProcessMemory(m_hProc, (BYTE*)address, &buffer, sizeof(T), 0);
	return buffer;
}

template <typename T>
void Write(DWORD address, T buffer)
{
    WriteProcessMemory(m_hProc, (BYTE*)address, &buffer, sizeof(T), 0);
}

bool GetProcess(const wchar_t *szName)
{
    auto hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 ProcEntry;
    ProcEntry.dwSize = sizeof(ProcEntry);

    while (Process32Next(hProcess, &ProcEntry))
    {
        if (!wcscmp(ProcEntry.szExeFile, szName))
        {
            CloseHandle(hProcess);
            m_dwPID = ProcEntry.th32ProcessID;
            m_hProc = OpenProcess(0x38, 0, m_dwPID);
            return true;
        }
    }

    CloseHandle(hProcess);

    return false;
}

DWORD GetModule(const wchar_t *szModule)
{
    auto hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwPID);
    MODULEENTRY32 ModEntry;
    ModEntry.dwSize = sizeof(ModEntry);

    while (Module32Next(hModule, &ModEntry))
    {
        if (!wcscmp(ModEntry.szModule, szModule))
        {
            CloseHandle(hModule);
            return reinterpret_cast<DWORD>(ModEntry.modBaseAddr);
        }
    }

    CloseHandle(hModule);

    return 0x0;
}