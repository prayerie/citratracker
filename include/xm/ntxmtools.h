/*
 * libNTXM - XM Player Library for the Nintendo DS
 *
 *    Copyright (C) 2005-2008 Tobias Weyand (0xtob)
 *                         me@nitrotracker.tobw.net
 *
 */

/***** BEGIN LICENSE BLOCK *****
 * 
 * Version: Noncommercial zLib License / GPL 3.0
 * 
 * The contents of this file are subject to the Noncommercial zLib License 
 * (the "License"); you may not use this file except in compliance with
 * the License. You should have recieved a copy of the license with this package.
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 3 or later (the "GPL"),
 * in which case the provisions of the GPL are applicable instead of those above.
 * If you wish to allow use of your version of this file only under the terms of
 * either the GPL, and not to allow others to use your version of this file under
 * the terms of the Noncommercial zLib License, indicate your decision by
 * deleting the provisions above and replace them with the notice and other
 * provisions required by the GPL. If you do not delete the provisions above,
 * a recipient may use your version of this file under the terms of any one of
 * the GPL or the Noncommercial zLib License.
 * 
 ***** END LICENSE BLOCK *****/

#ifndef _NTXMTOOLS_H_
#define _NTXMTOOLS_H_

#include <3ds/types.h>
#include <stdlib.h>
#include <stdio.h>
// #include <io.h>

/*
 * Some tools for ensuring there are no memory leaks and buffered file operations
 */

#if defined(ARM9) && defined(DEBUG)
void *my_malloc(size_t size);
void my_free(void *ptr);
void my_start_malloc_invariant(void);
void my_end_malloc_invariant(void);
void *my_memalign(size_t blocksize, size_t bytes);
#else
#define my_malloc malloc
#define my_free free
#define my_start_malloc_invariant() {}
#define my_end_malloc_invariant() {}
#define my_memalign memalign
static inline void my_dprintf(...) {}
#endif /* ARM9 && DEBUG */
bool my_file_exists(const char *name);

inline s32 my_clamp(s32 val, s32 min, s32 max)
{
	if(val < min)
		return min;
	if(val > max)
		return max;
	return val;
}

u32 my_getFreeDiskSpace(void); // Gets free disk space in bytes
u32 my_getUsedRam(void);
u32 my_getFileSize(const char *filename);

void ntxm_unsigned2signed_8(uint8_t *buffer, size_t count);
void ntxm_unsigned2signed_16(uint16_t *buffer, size_t count);

#endif
