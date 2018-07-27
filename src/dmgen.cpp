
#include "dmgen.h"

#include "dmutil.h"
#include "dmparser.h"

#include "arg_parser.h"

#include "dmbin2array.h"

#include "strtk.hpp"

#include "dmcmake_package_bin.h"

#include <ctemplate/per_expand_data.h>
#include <ctemplate/template.h>
#include <ctemplate/template_dictionary.h>
#include <ctemplate/template_dictionary_interface.h>
#include <ctemplate/template_emitter.h>
#include <ctemplate/template_enums.h>
#include <ctemplate/template_modifiers.h>
#include <ctemplate/template_namelist.h>
#include <ctemplate/template_pathops.h>
#include <ctemplate/template_string.h>

#include <fstream>

#define DMLog printf

CDMGen::CDMGen()
{

}

CDMGen::~CDMGen()
{

}

bool CDMGen::Init()
{
    return true;
}

bool CDMGen::DoCommand(int argc, char* argv[])
{
    std::vector<std::string> vecCommand;

    for (int i = 0; i < argc; i++)
    {
        std::string strCommand = argv[i];
        vecCommand.push_back(strCommand);
        //DMLog("%s\r\n", strCommand.c_str());
    }

	m_strUserPath = DMGetWorkPath();
    //DMLog("tool path: %s\r\n", m_strUserPath.c_str());

    const Arg_parser::Option options[] =
    {
        { 'p', "projectname",    Arg_parser::yes },
		{ 0, 0,          Arg_parser::no }
    };

    const Arg_parser parser(argc, argv, options);
    if (parser.error().size())
    {
        DMLog("Arg_parser failed\r\n");
        return false;
    }


    for (int argind = 0; argind < parser.arguments(); ++argind)
    {
        const int code = parser.code(argind);
        if (!code) break;
        switch (code)
        {
        case 'p':
        {
            const char * arg = parser.argument(argind).c_str();
            if (NULL == arg)
            {
                DMLog("Arg_parser %c failed\r\n", (char)code);
                return false;
            }

            m_strProjectName = arg;
        }
        break;
        default:
            DMLog("Arg_parser %c failed\r\n", (char)code);
            return false;
        }
    }

    if (m_strProjectName.empty())
    {
		m_strProjectName = "dmcmake";
    }

    std::string strRoot = DMGetWorkPath() + PATH_DELIMITER + m_strProjectName;
    
    if (DMDirectoryExist(strRoot.c_str()))
    {
        DMLog("project %s exist\r\n", strRoot.c_str());
        return false;
    }

	for (int i = ETPLTYPE_DMCMAKE__BEGIN; i < ETPLTYPE_DMCMAKE__END; ++i)
    {
        std::ofstream ofsh;

        std::string strFile = CDMTPL_DMCMAKE::GetFileName(i);

        strFile = ExpandFileName(strFile);

		strtk::replace('|', PATH_DELIMITER, strFile);

		strFile = strRoot + PATH_DELIMITER + strFile;

        std::string strFullPath = strFile;

        std::string strPath = strFullPath.substr(0, strFullPath.rfind(PATH_DELIMITER));

        DMDirectoryCreate(strPath.c_str(), true);

        ofsh.open(strFile,
            std::ios::out | std::ios::binary);

        if (ofsh.fail()) {
            DMASSERT(0);
            continue;
        }

        ofsh << MakeFile(i);
    }

    return true;
}

std::string CDMGen::MakeFile(int nType)
{
    tpl::TemplateDictionary oDict(CDMTPL_DMCMAKE::GetFileName(nType));
    std::string strOut;

    tpl::Template* poTemplate = tpl::Template::StringToTemplate(CDMTPL_DMCMAKE::GetData(
        nType), CDMTPL_DMCMAKE::GetDataSize(nType), ctemplate::DO_NOT_STRIP);

    if (NULL == poTemplate) {
        return strOut;
    }

    OnSetData(oDict);

    poTemplate->Expand(&strOut, &oDict);

    return strOut;
}

std::string CDMGen::ExpandFileName(const std::string& strFile)
{
    tpl::TemplateDictionary oDict(strFile);
    std::string strOut;

    tpl::Template* poTemplate = tpl::Template::StringToTemplate(strFile.c_str(), strFile.size(), ctemplate::DO_NOT_STRIP);

    if (NULL == poTemplate) {
        return strFile;
    }

    OnSetFileName(oDict);

    poTemplate->Expand(&strOut, &oDict);
    
    return strOut;
}

void CDMGen::OnSetData(tpl::TemplateDictionary& oDict)
{
    oDict.SetGlobalValue("PROJECT_NAME", m_strProjectName.c_str());
}

void CDMGen::OnSetFileName(tpl::TemplateDictionary& oDict)
{
    oDict.SetGlobalValue("PROJECT_NAME", m_strProjectName.c_str());
}
