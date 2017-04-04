/*
 * #%~
 * INTO-CPP-FMI
 * %%
 * Copyright (C) 2017 - 2014 Aarhus University
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/gpl-3.0.html>.
 * #~%
 */
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
#include <algorithm>

#else
#include <stdlib.h>
#include <unistd.h>
#endif
#include <memory>

using namespace std;
bool makePath(const std::string& path);
bool removePath(const std::string& path);
shared_ptr<string> getCurrentDir();
shared_ptr<string> getTempDir();

#endif /* SRC_FILESYSTEMUTIL_H_ */
