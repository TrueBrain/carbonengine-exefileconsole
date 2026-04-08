// Copyright © 2026 CCP ehf.
// This is a launcher for a windows application.  It allows us to run it from a command shell
// without spawning a new process.
// It is actually name agnostic, so rename the first part of it to match the windows app 
// and it will work.

#include "stdafx.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <locale>

std::wstring GetModuleName()
{
	wchar_t buf[ MAX_PATH+1 ];
	DWORD len = GetModuleFileName( 0, buf, _countof( buf ) );
	return std::wstring( buf, len );
}


int _tmain( int argc, _TCHAR* argv[] )
{
	std::wstring cmd = GetCommandLine();
	std::wstring module = GetModuleName();
	std::wstring exeName;

	//strip off the console part of the name:
	std::wstring::size_type ix = module.find( L"Console" );
	if( ix != std::wstring::npos )
	{
		exeName = module.replace( ix, strlen("Console"), L"" );
	}
	
	//find basename minus the .exe
	std::wstring::size_type found = exeName.rfind( L'\\' );
	std::wstring baseName = exeName.substr( found+1, exeName.size()-(found+1)-4 );

	//special signal for ExeFile.exe to go into inherit mode for console
	SetEnvironmentVariable( L"EXEFILE_INHERIT", L"" );

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset( &si, 0, sizeof( si ) );
	si.cb = sizeof( si );
	si.lpDesktop = (wchar_t*)L"";
	si.lpTitle = (wchar_t*)baseName.c_str();

	//pass on whatever it is we are using as stdandard IO
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	wchar_t *tmp = new wchar_t[ cmd.size()*2 ];
	wcscpy_s( tmp, cmd.size()*2, cmd.c_str() );
	BOOL ok = CreateProcess( exeName.c_str(), 
							 tmp,
							 0, 
							 0,
							 TRUE, //inherit handles (std IO)
							 0,
							 0, 
							 0,
							 &si, 
							 &pi );
	delete [] tmp;
	if( !ok )
	{
		return GetLastError();
	}

	CloseHandle( pi.hThread );
	DWORD waited = WaitForSingleObject( pi.hProcess, INFINITE );

	if( waited == WAIT_FAILED ) 
	{
		DWORD err = GetLastError();
		CloseHandle( pi.hProcess );
		return err;
	}

	DWORD exitCode;
	ok = GetExitCodeProcess(pi.hProcess, &exitCode);
	if( !ok ) 
	{
		DWORD err = GetLastError();
		CloseHandle( pi.hProcess );
		return err;
	}

	CloseHandle( pi.hProcess );
	return exitCode;
}
