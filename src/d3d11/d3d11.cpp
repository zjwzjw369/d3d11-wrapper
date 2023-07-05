/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "d3d11/main.h"
#include "string"
BOOL IsWow64()
{
	typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = nullptr;
	BOOL bIsWow64 = FALSE;
	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}


namespace d3d11 {

HMODULE chain = NULL;
FARPROC functions[func_count];

void hook_exports()
{
	if (IsWow64())
	{
		chain = LoadLibrary("C:\\Windows\\SysWOW64\\d3d11.dll");
	}
	else
	{
		chain = LoadLibrary("C:\\Windows\\System32\\d3d11.dll");
	}

	std::string rdcpath = "C:\\Program Files\\RenderDoc\\renderdoc.dll";
	HMODULE renderdoc = LoadLibrary(rdcpath.c_str());
	if (!renderdoc) {
		MessageBox(NULL, "Unable to locate renderdoc (or compatible library to chain)", "ERROR: Failed to load original d3d11.dll", NULL);
		exit(0);
	}
}

} // namespace d3d11



