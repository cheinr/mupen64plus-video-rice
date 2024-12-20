/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-core - osal_files_unix.c                                  *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2009 Richard Goedeken                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       * 
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
                       
/* This file contains the definitions for the unix-specific file handling
 * functions
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "osal_files.h"

/* global functions */

int osal_is_directory(const char* name)
{
    DIR* dir;
    dir = opendir(name);
    if(dir != NULL)
    {
        closedir(dir);
        return 1;
    }
    return 0;
}

#if (!M64P_STATIC_PLUGINS)
int osal_mkdirp(const char *dirpath, int mode)
{
    struct stat fileinfo;
    int dirpathlen = strlen(dirpath);
    char *currpath = strdup(dirpath);

    /* first, break the path into pieces by replacing all of the slashes wil NULL chars */
    while (strlen(currpath) > 1)
    {
        char *lastslash = strrchr(currpath, '/');
        if (lastslash == NULL)
            break;
        *lastslash = 0;
    }
    
    /* then re-assemble the path from left to right until we get to a directory that doesn't exist */
    while (strlen(currpath) < dirpathlen)
    {
        if (strlen(currpath) > 0 && stat(currpath, &fileinfo) != 0)
            break;
        currpath[strlen(currpath)] = '/';
    }

    /* then walk up the path chain, creating directories along the way */
    do
    {
        if (stat(currpath, &fileinfo) != 0)
        {
            if (mkdir(currpath, mode) != 0)
            {
                free(currpath);
                return 1;        /* mkdir failed */
            }
        }
        if (strlen(currpath) == dirpathlen)
            break;
        else
            currpath[strlen(currpath)] = '/';
    } while (1);
    
    free(currpath);        
    return 0;
}
#endif

void * osal_search_dir_open(const char *pathname)
{
    DIR *dir;
    dir = opendir(pathname);
    return dir;
}

const char *osal_search_dir_read_next(void * dir_handle)
{
    DIR *dir = (DIR *) dir_handle;
    struct dirent *entry;

    entry = readdir(dir);
    if (entry == NULL)
        return NULL;
    return entry->d_name;
}

void osal_search_dir_close(void * dir_handle)
{
    closedir((DIR *) dir_handle);
}

