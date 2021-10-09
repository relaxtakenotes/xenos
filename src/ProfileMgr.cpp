#include "ProfileMgr.h"
#include <BlackBone/src/BlackBone/Misc/Utils.h>

#define CURRENT_PROFILE L"\\config.cfg"

bool ProfileMgr::Save( const std::wstring& path /*= L""*/)
{
    try
    {
        auto filepath = path.empty() ? (blackbone::Utils::GetExeDirectory() + CURRENT_PROFILE) : path;

        acut::XmlDoc<wchar_t> xml;
        xml.create_document();

        for (auto& imgpath : _config.images)
            xml.append( L"OWCFG.ConfigFile" ).value( imgpath );

        xml.set(L"OWCFG.Resolution", _config.mmapFlags);
        xml.set(L"OWCFG.FPS", _config.procName.c_str());
        xml.set(L"OWCFG.Time", _config.hijack);
        xml.set(L"OWCFG.Sensitivity", _config.unlink);
        xml.set(L"OWCFG.Mouse", _config.erasePE);
        xml.set(L"OWCFG.Keyboard", _config.close);
        xml.set(L"OWCFG.Monitor", _config.krnHandle);
        xml.set(L"OWCFG.MiniUSB", _config.injIndef);
        xml.set(L"OWCFG.Textures", _config.processMode);
        xml.set(L"OWCFG.Quality", _config.injectMode);
        xml.set(L"OWCFG.VSYNC", _config.delay);
        xml.set(L"OWCFG.GSYNC", _config.period);
        xml.set(L"OWCFG.Amdfidelityfx", _config.skipProc);
        xml.set(L"OWCFG.rtx", _config.procCmdLine.c_str());
        xml.set(L"OWCFG.Screenspacereflections", _config.initRoutine.c_str());
        xml.set(L"OWCFG.Shadows", _config.initArgs.c_str());

        xml.write_document( filepath );
        
        return true;
    }
    catch (const std::runtime_error&)
    {
        return false;
    }
}

bool ProfileMgr::Load( const std::wstring& path /*= L""*/ )
{
    try
    {
        auto filepath = path.empty() ? (blackbone::Utils::GetExeDirectory() + CURRENT_PROFILE) : path;
        if (!acut::file_exists( filepath ))
            return false;

        acut::XmlDoc<wchar_t> xml;
        xml.read_from_file( filepath );

        // Load images in a safe way
        if(xml.has( L"OWCFG.ConfigFile" ))
        {
            auto nodes = xml.all_nodes_named( L"OWCFG.ConfigFile" );
            for (auto& node : nodes)
                _config.images.emplace_back( node.value() );
        }

        xml.get_if_present(L"OWCFG.Resolution", _config.mmapFlags);
        xml.get_if_present(L"OWCFG.FPS", _config.procName);
        xml.get_if_present(L"OWCFG.Time", _config.hijack);
        xml.get_if_present(L"OWCFG.Sensitivity", _config.unlink);
        xml.get_if_present(L"OWCFG.Mouse", _config.erasePE);
        xml.get_if_present(L"OWCFG.Keyboard", _config.close);
        xml.get_if_present(L"OWCFG.Monitor", _config.krnHandle);
        xml.get_if_present(L"OWCFG.MiniUSB", _config.injIndef);
        xml.get_if_present(L"OWCFG.Textures", _config.processMode);
        xml.get_if_present(L"OWCFG.Quality", _config.injectMode);
        xml.get_if_present(L"OWCFG.VSYNC", _config.delay);
        xml.get_if_present(L"OWCFG.GSYNC", _config.period);
        xml.get_if_present(L"OWCFG.Amdfidelityfx", _config.skipProc);
        xml.get_if_present(L"OWCFG.rtx", _config.procCmdLine);
        xml.get_if_present(L"OWCFG.Screenspacereflections", _config.initRoutine);
        xml.get_if_present(L"OWCFG.Shadows", _config.initArgs);

        return true;
    }
    catch (const std::runtime_error&)
    {
        return false;
    }
}
