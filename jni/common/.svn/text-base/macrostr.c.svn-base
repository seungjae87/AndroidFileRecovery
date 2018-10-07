/*
	Convert macro values to strings
	AUP2, Sec. 5.08 (not in book)

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#define __EXTENSIONS__
#include "defs.h"

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if STD_UNIX_VERSION < SUV_SUS3
#ifdef SOLARIS
#undef __EXTENSIONS__
#endif
#endif
#include "macrostr.h"
#ifndef DARWIN
#ifndef ANDROID
#include <sys/msg.h>
#endif
#endif
#ifndef ANDROID
#include <sys/sem.h>
#include <sys/shm.h>
#endif
#include <sys/mman.h>

static struct {
	char *ms_cat;
	intptr_t ms_code;
	char *ms_macro;
	char *ms_desc;
} macrostr_db[] = {
#include "macrostr.incl"
#include "macrostr_custom.incl"
	{ NULL, 0, NULL, NULL}
};

char *get_macrostr(const char *cat, int code, char **desc)
{
	int i;

	for (i = 0; macrostr_db[i].ms_cat != NULL; i++)
		if (strcmp(macrostr_db[i].ms_cat, cat) == 0 &&
		  macrostr_db[i].ms_code == code) {
			if (desc != NULL)
				*desc = macrostr_db[i].ms_desc;
			return macrostr_db[i].ms_macro;
		}
	if (desc != NULL)
		*desc = "?";

	return "?";
}
