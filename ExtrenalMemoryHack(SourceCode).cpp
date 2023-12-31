#include<Windows.h>
#include<tlhelp32.h>
bool GetPid(const wchar_t* targetProcess, DWORD* procID)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap && snap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(pe);
        if (Process32First(snap, &pe))
        {
            do
            {
                if (!wcscmp(pe.szExeFile, targetProcess))
                {
                    CloseHandle(snap);
                    *procID = pe.th32ProcessID;
                    return true;
                }
            } while (Process32Next(snap, &pe));
        }
    }
    return false;
}
char* GetModuleBase(const wchar_t* ModuleName, DWORD procID){
    MODULEENTRY32 ModuleEntry = { 0 };
    HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

    if (!SnapShot) return NULL;

    ModuleEntry.dwSize = sizeof(ModuleEntry);

    if (!Module32First(SnapShot, &ModuleEntry)) return NULL;

    do
    {
        if (!wcscmp(ModuleEntry.szModule, ModuleName))
        {
            CloseHandle(SnapShot);
            return (char*)ModuleEntry.modBaseAddr;
        }
    } while (Module32Next(SnapShot, &ModuleEntry));

    CloseHandle(SnapShot);
    return NULL;
}
int main() {

        int  NEW_MONEY_VALUE=9999;
        DWORD ProcId;
        GetPid(L"hl.exe", &ProcId);
        char* DLLBaseAddress = GetModuleBase(L"client.dll", ProcId);
        DWORD BaseAdress = (DWORD)DLLBaseAddress;
        HANDLE PRocesscs = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcId);
        ReadProcessMemory(PRocesscs, (PBYTE*)BaseAdress, NULL, 4, 0);
        ReadProcessMemory(PRocesscs, (PBYTE*)(BaseAdress + 0x1254D4), NULL, 4, 0);
        while (true) {
            WriteProcessMemory(PRocesscs, (PBYTE*)(BaseAdress + 0x1254D4), &NEW_MONEY_VALUE, 4, 0);
        }
        
}
