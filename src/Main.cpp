#include "stdafx.h"
#include "MainDlg.h"
#include "DriverExtract.h"

#include <shellapi.h>

/// <summary>
/// Crash dump notify callback
/// </summary>
/// <param name="path">Dump file path</param>
/// <param name="context">User context</param>
/// <param name="expt">Exception info</param>
/// <param name="success">if false - crash dump file was not saved</param>
/// <returns>status</returns>

/// <summary>
/// Associate profile file extension
/// </summary>
void AssociateExtension()
{
    wchar_t tmp[255] = { 0 };
    GetModuleFileNameW( NULL, tmp, sizeof( tmp ) );

#ifdef USE64
    std::wstring ext = L".ow";
    std::wstring alias = L"OW";
    std::wstring desc = L"OW";
#endif 
    std::wstring editWith = std::wstring( tmp ) + L" --load %1";
    std::wstring runWith = std::wstring( tmp ) + L" --run %1";

    auto AddKey = []( const std::wstring& subkey, const std::wstring& value, const wchar_t* regValue ) {
        SHSetValue( HKEY_CLASSES_ROOT, subkey.c_str(), regValue, REG_SZ, value.c_str(), (DWORD)(value.size() * sizeof( wchar_t )) );
    };

    SHDeleteKeyW( HKEY_CLASSES_ROOT, alias.c_str() );

    AddKey( ext, alias, nullptr );
    AddKey( ext, L"Application/xml", L"Content Type" );
    AddKey( alias, desc, nullptr );
    AddKey( alias + L"\\shell", L"Run", nullptr );
    AddKey( alias + L"\\shell\\Edit\\command", editWith, nullptr );
    AddKey( alias + L"\\shell\\Run\\command", runWith, nullptr );
}

/// <summary>
/// Log major OS information
/// </summary>
void LogOSInfo()
{
}

/// <summary>
/// Parse command line string
/// </summary>
/// <param name="param">Resulting param</param>
/// <returns>Profile action</returns>
MainDlg::StartAction ParseCmdLine( std::wstring& param )
{
    int argc = 0;
    auto pCmdLine = GetCommandLineW();
    auto argv = CommandLineToArgvW( pCmdLine, &argc );

    for (int i = 1; i < argc; i++)
    {
        if (_wcsicmp( argv[i], L"--load" ) == 0 && i + 1 < argc)
        {
            param = argv[i + 1];
            return MainDlg::LoadProfile;
        }
        if (_wcsicmp( argv[i], L"--run" ) == 0 && i + 1 < argc)
        {
            param = argv[i + 1];
            return MainDlg::RunProfile;
        }
    }

    return MainDlg::Nothing;
}

int APIENTRY wWinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/ )
{
    AssociateExtension();

    std::wstring param;
    auto action = ParseCmdLine( param );
    MainDlg mainDlg( action, param );

    if (action != MainDlg::RunProfile)
        return (int)mainDlg.RunModeless( NULL, IDR_ACCELERATOR1 );
    else
        return mainDlg.LoadAndInject();
}