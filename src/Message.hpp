#pragma once

#include "stdafx.h"
#include "Log.h"

class Message
{
    enum MsgType
    {
        Error,
        Warning,
        Info,
        Question,
    };

public:
    static void ShowError( HWND parent, const std::wstring& msg, const std::wstring& title = L"Error" )
    {
        Show( msg, title, Error, parent );
    }

    static void ShowWarning( HWND parent, const std::wstring& msg, const std::wstring& title = L"Warning" )
    {
        Show( msg, title, Warning, parent );
    }

    static void ShowInfo( HWND parent, const std::wstring& msg, const std::wstring& title = L"Info" )
    {
        Show( msg, title, Info, parent );
    }

    static bool ShowQuestion( HWND parent, const std::wstring& msg, const std::wstring& title = L"Question" )
    {
        return Show( msg, title, Question, parent ) == IDYES;
    }

private:
    static int Show( 
        const std::wstring& msg, 
        const std::wstring& title, 
        MsgType type, 
        HWND parent = NULL 
        )
    {
        UINT uType = MB_ICONERROR;

        return MessageBoxW( parent, msg.c_str(), title.c_str(), uType );
    }
};