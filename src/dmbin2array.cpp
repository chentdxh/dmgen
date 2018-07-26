#include "strtk.hpp"
#include "dmbin2array.h"
#include "dmos.h"

#define LINE_COUNT 10

CDMBin2Array::CDMBin2Array()
{

}

CDMBin2Array::~CDMBin2Array()
{

}

int CDMBin2Array::DoFiles(const std::string& strRoot, VecFileInfo& vecPwds)
{
    int nLine = LINE_COUNT;
    std::string strBinFile = strRoot + "_package_bin";

    std::ofstream ofs;

    std::string  strOutNameUpper = strtk::as_uppercase(strBinFile);
    std::string  strOutNameLower = strtk::as_lowercase(strBinFile);

    std::string strOutFile = strBinFile + ".h";

    ofs.open(strOutFile.c_str());
    if (ofs.fail())
    {
        std::cout << "output file [" << strOutFile << "] open failed." << std::endl;
        return -1;
    }

    std::stringstream ssOut;

    ssOut << "#ifndef " << "___" << strOutNameUpper << "_H___" << std::endl;
    ssOut << "#define " << "___" << strOutNameUpper << "_H___" << std::endl;
    ssOut << std::endl;


	for (int i = 0; i < static_cast<int>(vecPwds.size()); ++i)
    {
        SPackFileInfo& stInfo = vecPwds[i];

        std::stringstream ssIn;
        std::ifstream ifs;
        ifs.open(stInfo.strFullPathFile.c_str(), std::ios::binary);
        if (ifs.fail())
        {
            std::cout << "source file [" << stInfo.strFullPathFile << "] open failed." << std::endl;
            continue;
        }
        ssIn << ifs.rdbuf();

        ssOut << DoFile(ssIn, stInfo);
    }

    ssOut << std::endl;

    ssOut << "enum ETPLTYPE_" << strtk::as_uppercase(strRoot) << " {" << std::endl;
    ssOut << "\t" << "ETPLTYPE_" << strtk::as_uppercase(strRoot) << "__BEGIN = 0," << std::endl;

	for (int i = 0; i < static_cast<int>(vecPwds.size()); ++i)
    {
        SPackFileInfo& stInfo = vecPwds[i];
        
        ssOut << "\t" << "ETPLTYPE_" << strtk::as_uppercase(stInfo.strIDName) << " = " << i << "," << std::endl;
    }

    ssOut << "\t" << "ETPLTYPE_" << strtk::as_uppercase(strRoot) << "__END," << std::endl;
    ssOut << "};" << std::endl;
    ssOut << std::endl;

    ssOut << "class CDMTPL_" << strtk::as_uppercase(strRoot) << " {"<< std::endl;
    ssOut << "public:" << std::endl;

    ///////////////////////////////////////////////////////////////////////////////////
    ssOut << "\t" << "static const char* GetFileName(int nType) {" << std::endl;
    ssOut << "\t\t" << "switch (nType) {" << std::endl;

	for (int i = 0; i < static_cast<int>(vecPwds.size()); ++i)
    {
        SPackFileInfo& stInfo = vecPwds[i];

        ssOut << "\t\t\t" << "case " << "ETPLTYPE_" << strtk::as_uppercase(stInfo.strIDName) << ": {" << std::endl;

        ssOut << "\t\t\t\t" << "return reinterpret_cast<const char*>(" << stInfo.strHFileName << ");" << std::endl;
        ssOut << "\t\t\t" << "}" << std::endl;
    }

    ssOut << "\t\t" << "}" << std::endl;
    ssOut << "\t\t" << "return \"\";" << std::endl;

    ssOut << "\t" << "}" << std::endl;
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    ssOut << "\t" << "static const char* GetData(int nType) {" << std::endl;
    ssOut << "\t\t" << "switch (nType) {" << std::endl;

	for (int i = 0; i < static_cast<int>(vecPwds.size()); ++i)
    {
        SPackFileInfo& stInfo = vecPwds[i];
        
        ssOut << "\t\t\t" << "case " << "ETPLTYPE_" << strtk::as_uppercase(stInfo.strIDName) << ": {" << std::endl;

        ssOut << "\t\t\t\t" << "return reinterpret_cast<const char*>(" << stInfo.strHDataName << ");" << std::endl;
        ssOut << "\t\t\t" << "}" << std::endl;
    }

    ssOut << "\t\t" << "}" << std::endl;
    ssOut << "\t\t" << "return \"\";" << std::endl;

    ssOut << "\t" << "}" << std::endl;
    ///////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////
    ssOut << "\t" << "static size_t GetDataSize(int nType) {" << std::endl;
    ssOut << "\t\t" << "switch (nType) {" << std::endl;

	for (int i = 0; i < static_cast<int>(vecPwds.size()); ++i)
    {
        SPackFileInfo& stInfo = vecPwds[i];

        ssOut << "\t\t\t" << "case " << "ETPLTYPE_" << strtk::as_uppercase(stInfo.strIDName) << ": {" << std::endl;

        ssOut << "\t\t\t\t" << "return sizeof(" << stInfo.strHDataName << ");" << std::endl;
        ssOut << "\t\t\t" << "}" << std::endl;
    }

    ssOut << "\t\t" << "}" << std::endl;
    ssOut << "\t\t" << "return 0;" << std::endl;

    ssOut << "\t" << "}" << std::endl;
    ///////////////////////////////////////////////////////////////////////////////////

    ssOut << "};" << std::endl;


    ssOut << std::endl;
    ssOut << "#endif " << "//___" << strOutNameUpper << "_H___" << std::endl;

    ofs << ssOut.str();
    return 0;
}

std::string CDMBin2Array::DoFile(std::stringstream& ssIn, SPackFileInfo& stInfo)
{
    std::stringstream ssOut;

    std::string strHex = strtk::convert_bin_to_hex(ssIn.str());

    ssOut << "static const char* " << stInfo.strHFileName << " = \"" << stInfo.strModuleName << "\";" << std::endl << std::endl;

    ssOut << "static const unsigned char " << stInfo.strHDataName << "[" << ssIn.str().size() << "]" << " = {";

    for (int i = 0; i + 1 < static_cast<int>(strHex.size()); i += 2)
    {
        if (0 != i)
        {
            ssOut << ",\t";
        }

        if (0 == i % (2 * LINE_COUNT))
        {
            ssOut << std::endl;
            ssOut << "\t";
        }
        ssOut << "0x" << strHex[i] << strHex[i + 1];
    }

    ssOut << std::endl;

    ssOut << "};" << std::endl;
    return ssOut.str();
}

