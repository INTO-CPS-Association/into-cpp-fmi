/*
 * FilesystemUtil.h
 *
 *  Created on: Mar 15, 2017
 *      Author: kel
 */

#ifndef SRC_FILESYSTEMUTIL_H_
#define SRC_FILESYSTEMUTIL_H_


#include <iostream>
#include <string>
#include <sys/stat.h> // stat
#include <errno.h>    // errno, ENOENT, EEXIST
#if defined(_WIN32)
#include <direct.h>   // _mkdir

#include <windows.h>
#include <stdio.h>
// A safer version for string manipulation
#include <strsafe.h>

#include <memory>

#else
#include <stdlib.h>
#include <unistd.h>
#endif
#include <memory>

using namespace std;
bool makePath(const std::string& path);
bool removePath(const std::string& path);
shared_ptr<string> getTempDir();

#endif /* SRC_FILESYSTEMUTIL_H_ */
