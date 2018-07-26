
#ifndef __DMBIN2ARRAY_H_INCLUDE__
#define __DMBIN2ARRAY_H_INCLUDE__

#include "dmsingleton.h"
#include "dmgen.h"

class CDMBin2Array : public CDMSafeSingleton<CDMBin2Array>
{
    friend class CDMSafeSingleton<CDMBin2Array>;
public:
    CDMBin2Array();
    virtual ~CDMBin2Array();

    int DoFiles(const std::string& strRoot, VecFileInfo& vecPwds);

    std::string DoFile(std::stringstream& ssIn, SPackFileInfo& stInfo);
};

#endif // __DMBIN2ARRAY_H_INCLUDE__