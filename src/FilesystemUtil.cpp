/*
 * FilesystemUtil.cpp
 *
 *  Created on: Mar 15, 2017
 *      Author: kel
 */

#include "FilesystemUtil.h"

bool isDirExist(const std::string& path)
{
#if defined(_WIN32)
	struct _stat info;
	if (_stat(path.c_str(), &info) != 0)
	{
		return false;
	}
	return (info.st_mode & _S_IFDIR) != 0;
#else
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
	{
		return false;
	}
	return (info.st_mode & S_IFDIR) != 0;
#endif
}

bool makePath(const std::string& path)
{
#if defined(_WIN32)
	int ret = _mkdir(path.c_str());
#else
	mode_t mode = 0755;
	int ret = mkdir(path.c_str(), mode);
#endif
	if (ret == 0)
		return true;

	switch (errno)
	{
	case ENOENT:
		// parent didn't exist, try to create it
	{
		int pos = path.find_last_of('/');
		if (pos == std::string::npos)
#if defined(_WIN32)
			pos = path.find_last_of('\\');
			if (pos == std::string::npos)
#endif
			return false;
		if (!makePath(path.substr(0, pos)))
			return false;
	}
		// now, try to create again
#if defined(_WIN32)
		return 0 == _mkdir(path.c_str());
#else
		return 0 == mkdir(path.c_str(), mode);
#endif

	case EEXIST:
		// done!
		return isDirExist(path);

	default:
		return false;
	}
}

bool removePath(const std::string& path)
{
	if (path.size() < 1 || !isDirExist(path))
		return false;
#if defined(_WIN32)
	//TODO
#else
	auto cmd = make_shared<std::string>("rm -r ");
	cmd->append(path);
	return system(cmd->c_str());
#endif
}

shared_ptr<string> getTempDir()
{

#if defined(_WIN32)
#define BUFSIZE 512
	HANDLE hFile;
	HANDLE hTempFile;
	DWORD dwRetVal, dwRetVal2;
	DWORD dwBytesRead;
	DWORD dwBytesWritten;
	DWORD dwBufSize=BUFSIZE;
	UINT uRetVal;
	WCHAR szTempName[BUFSIZE];
	char buffer[BUFSIZE];
	WCHAR lpPathBuffer[BUFSIZE];
	BOOL fSuccess;

	dwRetVal = GetTempPath(dwBufSize,     // length of the buffer
			lpPathBuffer);// buffer for path
	if (dwRetVal > dwBufSize || (dwRetVal == 0))
	{
		return NULL;
	}

	// Create a temporary file.
	uRetVal = GetTempFileName(lpPathBuffer,// directory for tmp files
			L"fmu-",// temp file name prefix
			0,// create unique name
			szTempName);// buffer for name
	if (uRetVal == 0)
	{
		return NULL;
	}
	return make_shared<string>(szTempName);
#else
	char template_name[] = "/tmp/fmu-XXXXXX";
	char * name = mkdtemp(template_name);
	return make_shared<string>(name);

#endif

}
