#include <windows.h>

extern "C" __declspec(dllexport) void* __stdcall Direct3DCreate8(unsigned int version)
{
    OutputDebugStringA("[OFP-D3DX OpenGL] DLL loaded successfully!");
    return (void*)0xDEADBEEF;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        OutputDebugStringA("[OFP-D3DX OpenGL] Attached to game process");
        break;
    case DLL_PROCESS_DETACH:
        OutputDebugStringA("[OFP-D3DX OpenGL] Detached from game process");
        break;
    }
    return TRUE;
}