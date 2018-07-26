
#ifndef __DMGEN_H_INCLUDE__
#define __DMGEN_H_INCLUDE__


#include "dmsingleton.h"

#include "dmos.h"

#include <ctemplate/template_dictionary.h>

namespace tpl = ctemplate;

typedef struct tagFileInfo {
    std::string strFullPathFile;
    std::string strFullPath;
    std::string strModuleName;
    std::string strIDName;
    ///
    std::string strHFileName;
    std::string strHDataName;
} SPackFileInfo;

typedef std::vector<SPackFileInfo> VecFileInfo;
typedef VecFileInfo::iterator VecFileInfoIt;

class CDMGen : public CDMSafeSingleton<CDMGen>
{
    friend class CDMSafeSingleton<CDMGen>;


public:
    CDMGen();
    virtual ~CDMGen();

    bool Init();

    std::string MakeFile(int nType);

    bool DoCommand(int argc, char* argv[]);

	void OnSetData(tpl::TemplateDictionary& oDict);
private:

    std::string m_strUserPath;
    std::string m_strProjectName;
};

#endif // __DMGEN_H_INCLUDE__