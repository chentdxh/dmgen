
#include "dmgen.h"

#include "dmutil.h"
#include "dmparser.h"

#include "arg_parser.h"

#include "dmbin2array.h"

#include "strtk.hpp"

#include "dmcmake_package_bin.h"
#include "dmerror_package_bin.h"

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

#include "pugixml.hpp"

#define DMLog printf

const char * ELEM_CODER = "coder";
const char * ELEM_ERRORS = "errors";

using namespace pugi;

CDMGen::CDMGen()
{

}

CDMGen::~CDMGen()
{

}

bool CDMGen::Init()
{
    return true
        && LoadError()
        && true;
}

bool CDMGen::LoadError()
{
    std::string strFileName = CDMTPL_DMERROR::GetFileName(ETPLTYPE_ERRORCODE_XAT);
    std::string strBuffer = CDMTPL_DMERROR::GetData(ETPLTYPE_ERRORCODE_XAT);

    xml_document doc;
    if (!doc.load_buffer(strBuffer.c_str(), strBuffer.size()))
    {
        DMLog("xat file:%s is not exist\r\n", strFileName.c_str());
        return false;
    }

    xml_node coder = doc.child(ELEM_CODER);
    if (!coder)
    {
        DMLog("not find root node %s\r\n", ELEM_CODER);
        return false;
    }

    xml_node errors = coder.child(ELEM_ERRORS);
    if (errors)
    {
        xml_object_range<xml_named_node_iterator> Item = errors.children("error");
        xml_named_node_iterator It = Item.begin();

        for (; It != Item.end(); ++It)
        {
            std::string strErrorName = (*It).attribute("name").as_string();
            std::string strErrorID = (*It).attribute("id").as_string();
            std::string strInfo = (*It).attribute("info").as_string();
            std::string strDes = (*It).attribute("des").as_string();

            if (strErrorName.empty())
            {
                DMLog("error name is empty\r\n");
                return false;
            }

            CDM_ERROR oError;

            oError.GetName() = strErrorName;
            oError.GetID() = atoi(strErrorID.c_str());

            oError.GetInfo() = strInfo;
            oError.GetDes() = strDes;

            if (!AddError(oError))
            {
                DMLog("error node [%s] error\r\n", strErrorName.c_str());
                return false;
            }
        }
    }
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
        { 'f', "force",    Arg_parser::no },
		{ 0, 0,          Arg_parser::no }
    };

    const Arg_parser parser(argc, argv, options);
    if (parser.error().size())
    {
        DMLog("Arg_parser failed\r\n");
        return false;
    }

    bool bForce = false;

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
        case 'f':
        {
            bForce = true;
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
    
    if (!bForce)
    {
        if (DMDirectoryExist(strRoot.c_str()))
        {
            DMLog("project %s exist\r\n", strRoot.c_str());
            return false;
        }
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

bool CDMGen::AddError(CDM_ERROR& oError)
{
    for (int i = 0; i < static_cast<int>(m_vecErrorCode.size()); i++)
    {
        if (m_vecErrorCode[i].GetName() == oError.GetName()
            || m_vecErrorCode[i].GetID() == oError.GetID())
        {
            DMLog("已存在错误码定义 %s -> %d\r\n", m_vecErrorCode[i].GetName().c_str(), m_vecErrorCode[i].GetID());
            return false;
        }
    }

    m_vecErrorCode.push_back(oError);
    return true;
}

void CDMGen::OnSetData(tpl::TemplateDictionary& oDict)
{
    std::string strUP_PROTO_NAME = strtk::as_uppercase(m_strProjectName);
    std::string strLO_PROTO_NAME = strtk::as_lowercase(m_strProjectName);

    oDict.SetGlobalValue("PROJECT_NAME", m_strProjectName.c_str());

    oDict.SetGlobalValue("UP_PROTO_NAME", strUP_PROTO_NAME.c_str());
    oDict.SetGlobalValue("LO_PROTO_NAME", strLO_PROTO_NAME.c_str());
    oDict.SetGlobalValue("PROTO_NAME", m_strProjectName.c_str());

    for (int i = 0; i < static_cast<int>(m_vecErrorCode.size()); i++)
    {
        CDM_ERROR& oError = m_vecErrorCode[i];
        tpl::TemplateDictionary* poEnum = oDict.AddSectionDictionary("ERRORCODE_LIST_ENUM");
        if (poEnum)
        {
            poEnum->SetValue("ERROR_NAME", oError.GetName().c_str());
            poEnum->SetIntValue("ERROR_ID", oError.GetID());
            poEnum->SetValue("ERROR_INFO", oError.GetInfo());
            poEnum->SetValue("ERROR_DES", oError.GetDes());
        }
    }

    for (int i = 0; i < static_cast<int>(m_vecErrorCode.size()); i++)
    {
        CDM_ERROR& oError = m_vecErrorCode[i];
        tpl::TemplateDictionary* poEnum = oDict.AddSectionDictionary("ERRORCODE_LIST_GETERROR");
        if (poEnum)
        {
            poEnum->SetValue("ERROR_NAME", oError.GetName().c_str());
            poEnum->SetIntValue("ERROR_ID", oError.GetID());
            poEnum->SetValue("ERROR_INFO", oError.GetInfo());
            poEnum->SetValue("ERROR_DES", oError.GetDes());
        }
    }

    for (int i = 0; i < static_cast<int>(m_vecErrorCode.size()); i++)
    {
        CDM_ERROR& oError = m_vecErrorCode[i];
        tpl::TemplateDictionary* poEnum = oDict.AddSectionDictionary("ERRORCODE_LIST_SWITCH");
        if (poEnum)
        {
            poEnum->SetValue("ERROR_NAME", oError.GetName().c_str());
            poEnum->SetIntValue("ERROR_ID", oError.GetID());
            poEnum->SetValue("ERROR_INFO", oError.GetInfo());
            poEnum->SetValue("ERROR_DES", oError.GetDes());
        }
    }
}

void CDMGen::OnSetFileName(tpl::TemplateDictionary& oDict)
{
    oDict.SetGlobalValue("PROJECT_NAME", m_strProjectName.c_str());
}
