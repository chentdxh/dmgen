
// Copyright (c) 2018 brinkqiang
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __DMUTIL_H_INCLUDE__
#define __DMUTIL_H_INCLUDE__

#include "dmos.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

// tolua_begin

#ifdef WIN32
static inline struct tm* localtime_r( const time_t* timep, struct tm* result ) {
    localtime_s( result, timep );
    return result;
}
static inline struct tm* gmtime_r( const time_t* timep, struct tm* result ) {
    gmtime_s( result, timep );
    return result;
}
#endif

static inline std::string DMFormatIP( unsigned int dwIP ) {
    sockaddr_in s;
    s.sin_family = AF_INET;
    s.sin_addr.s_addr = dwIP;
    return inet_ntoa( s.sin_addr );
}

static inline unsigned int DMFormatIP( const std::string& strIp ) {
    return inet_addr( strIp.c_str() );
}

static inline unsigned short DMFormatPort( const std::string& strPort ) {
    return htons( atoi( strPort.c_str() ) );
}

static inline unsigned short DMFormatPort( unsigned short wPort ) {
    return htons( wPort );
}

static inline std::string DMFormatDateTime( time_t tVal = time( 0 ),
        const char* pFormat = "%Y-%m-%d %H:%M:%S" ) {
    struct tm Tm = {0};

    if ( localtime_r( &tVal, &Tm ) ) {
        char szBuf[128];
        strftime( szBuf, sizeof( szBuf ), pFormat, &Tm );
        return szBuf;
    }

    return "";
}

static inline time_t DMFormatDateTime( const std::string& strTime,
                                       const char* pFormat =  "%d-%d-%d %d:%d:%d" ) {
    time_t ret = 0;
    struct tm tmMake = {0};

    if ( 6 == sscanf( strTime.c_str(), pFormat, &tmMake.tm_year, &tmMake.tm_mon,
                      &tmMake.tm_mday, &tmMake.tm_hour, &tmMake.tm_min, &tmMake.tm_sec ) ) {
        tmMake.tm_year -= 1900;
        tmMake.tm_mon -= 1;
        ret = mktime( &tmMake );
    }

    return ret;
}

static bool DMDirectoryExist(const char *dir_name)
{
#ifdef WIN32
    int ret = GetFileAttributesA(dir_name);
    if (ret == -1)
    {
        return false;
    }
    return !!(FILE_ATTRIBUTE_DIRECTORY & ret);
#else
    struct stat fileStat;
    int ret = stat(dir_name, &fileStat);
    if (ret == 0)
    {
        return S_ISDIR(fileStat.st_mode);
    }
    return false;
#endif
}

static inline bool DMDirectoryCreate(const char *dir_name, bool force)
{

#ifdef WIN32
    int ret = mkdir(dir_name);
#else
    int ret = mkdir(dir_name,S_IRWXU | S_IRWXG );
#endif
    if (!force)
    {
        if (0 != ret)
        {
            return false;
        }
        return true;
    }

    if (0 != ret)
    {
        if (force)
        {
            char path[MAX_PATH];
            char * pos = path;
            strncpy(path, dir_name, sizeof(path));
            char * end = path + strlen(path);
            while (*pos != '\0')
            {
                if (PATH_IS_DELIMITER(*pos))
                {
                    *pos = '\0';
                }
                ++pos;
            }
            pos = path;
            bool bfirst = true;
            while (pos < end)
            {
                if (bfirst)
                {
                    bool isdrive = false;
                    char * p = pos;
                    while (*p != '\0')
                    {
                        if (*p == ':')
                        {
                            isdrive = true;
                        }
                        ++p;
                    }
                    if (isdrive)
                    {
                        pos = path + strlen(path);
                        *pos = PATH_DELIMITER;
                        bfirst = false;
                        continue;
                    }
                    bfirst = false;
                }

                if (!DMDirectoryExist(path))
                {
#ifdef WIN32
                    if (0 != mkdir(path))
#else
                    if (0 != mkdir(path, S_IREAD | S_IWRITE))
#endif
                    {
                        return false;
                    }
                }
                pos = path + strlen(path);
                *pos = PATH_DELIMITER;
            }
        }
        else//bforce == false
        {
            return false;
        }
    }
    return true;
}


static std::string DMGetRootPath() {
#ifdef WIN32
	static char path[MAX_PATH];
	static bool first_time = true;

	if (first_time) {
		first_time = false;
		GetModuleFileNameA(0, path, sizeof(path));
		char* p = strrchr(path, '\\');
		*(p) = '\0';
	}

	return path;
#elif __APPLE__
	static char path[MAX_PATH];
	static bool first_time = true;

	if (first_time) {
		uint32_t size = sizeof(path);
		int nRet = _NSGetExecutablePath(path, &size);

		if (nRet != 0) {
			return "./";
		}

		char* p = strrchr(path, '/');
		*(p) = '\0';
	}

	return path;
#else
	static char path[MAX_PATH];
	static bool first_time = true;

	if (first_time) {
		first_time = false;
		int nRet = readlink("/proc/self/exe", path, MAX_PATH);

		if (nRet < 0 || nRet >= MAX_PATH) {
			return "./";
		}

		char* p = strrchr(path, '/');
		*(p) = '\0';
	}

	return path;
#endif
}

static std::string DMGetExePath() {
#ifdef WIN32
    static char path[MAX_PATH];
    static bool first_time = true;

    if (first_time) {
        first_time = false;
        GetModuleFileNameA(0, path, sizeof(path));
    }

    return path;
#elif __APPLE__
    static char path[MAX_PATH];
    static bool first_time = true;

    if (first_time) {
        uint32_t size = sizeof(path);
        int nRet = _NSGetExecutablePath(path, &size);

        if (nRet != 0) {
            return "./";
        }
    }

    return path;
#else
    static char path[MAX_PATH];
    static bool first_time = true;

    if (first_time) {
        first_time = false;
        int nRet = readlink("/proc/self/exe", path, MAX_PATH);

        if (nRet < 0 || nRet >= MAX_PATH) {
            return "./";
        }
    }

    return path;
#endif
}

static std::string DMGetWorkPath() {
	char szPath[MAX_PATH];
    getcwd(szPath, sizeof(szPath));
	return szPath;
}

// tolua_end

#endif // __DMUTIL_H_INCLUDE__
