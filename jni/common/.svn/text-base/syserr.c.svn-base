/*
	syserrmsg
	Sec. 1.4.1

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
#ifdef AUP1
/* Copyright 1985 Advanced Programming Institute, Ltd. */
/*1-3*/
#include <stdio.h>
#include "_errno.h"

void syserr(const char *msg)  /* print system call error message and terminate */
{
	extern int errno, sys_nerr;
	extern char *sys_errlist[];

	fprintf(stderr, "ERROR: %s (%d", msg, errno);
	if (errno > 0 && errno < sys_nerr)
		fprintf(stderr, "; %s)\n", sys_errlist[errno]);
	else
		fprintf(stderr, ")\n");
	exit(EXIT_FAILURE);
}
/**/
#else

#define __EXTENSIONS__
#include "defs.h"

#include <netdb.h>

#if STD_UNIX_VERSION < SUV_SUS3
#ifdef SOLARIS
#undef __EXTENSIONS__
#endif
#endif

/*
	Following appeared in the book, but now the more general function get_macrostr
	is used. It's in common/macrostr.c.
*/
/*[errcodes]*/
#if 0 /* skip errcodes */
static struct {
	int code;
	char *str;
} errcodes[] =
{
	{ EPERM, "EPERM" },
	{ ENOENT, "ENOENT" },
	{ ESRCH, "ESRCH" },
	{ EINTR, "EINTR" },
	{ EIO, "EIO" },
	{ ENXIO, "ENXIO" },
	{ E2BIG, "E2BIG" },
	{ ENOEXEC, "ENOEXEC" },
	{ EBADF, "EBADF" },
	{ ECHILD, "ECHILD" },
	{ EAGAIN, "EAGAIN" },
	{ ENOMEM, "ENOMEM" },
	{ EACCES, "EACCES" },
	{ EFAULT, "EFAULT" },
	{ EBUSY, "EBUSY" },
	{ EEXIST, "EEXIST" },
	{ EXDEV, "EXDEV" },
	{ ENODEV, "ENODEV" },
	{ ENOTDIR, "ENOTDIR" },
	{ EISDIR, "EISDIR" },
	{ EINVAL, "EINVAL" },
	{ ENFILE, "ENFILE" },
	{ EMFILE, "EMFILE" },
	{ ENOTTY, "ENOTTY" },
	{ EFBIG, "EFBIG" },
	{ ENOSPC, "ENOSPC" },
	{ ESPIPE, "ESPIPE" },
	{ EROFS, "EROFS" },
	{ EMLINK, "EMLINK" },
	{ EPIPE, "EPIPE" },
	{ EDOM, "EDOM" },
	{ ERANGE, "ERANGE" },
	{ EDEADLK, "EDEADLK" },
	{ ENAMETOOLONG, "ENAMETOOLONG" },
	{ ENOLCK, "ENOLCK" },
	{ ENOSYS, "ENOSYS" },
	{ ENOTEMPTY, "ENOTEMPTY" },
#ifndef BSD_DERIVED /* Following are on LINUX; don't know yet about Solaris */
	{ ECHRNG, "ECHRNG" },
	{ EL2NSYNC, "EL2NSYNC" },
	{ EL3HLT, "EL3HLT" },
	{ EL3RST, "EL3RST" },
	{ ELNRNG, "ELNRNG" },
	{ EUNATCH, "EUNATCH" },
	{ ENOCSI, "ENOCSI" },
	{ EL2HLT, "EL2HLT" },
	{ EBADE, "EBADE" },
	{ EBADR, "EBADR" },
	{ EXFULL, "EXFULL" },
	{ ENOANO, "ENOANO" },
	{ EBADRQC, "EBADRQC" },
	{ EBADSLT, "EBADSLT" },
	{ EDEADLOCK, "EDEADLOCK" },
	{ EBFONT, "EBFONT" },
	{ ENOSTR, "ENOSTR" },
	{ ENODATA, "ENODATA" },
	{ ETIME, "ETIME" },
	{ ENOSR, "ENOSR" },
	{ ENONET, "ENONET" },
	{ ENOPKG, "ENOPKG" },
	{ ENOLINK, "ENOLINK" },
	{ EADV, "EADV" },
	{ ESRMNT, "ESRMNT" },
	{ ECOMM, "ECOMM" },
	{ EPROTO, "EPROTO" },
	{ EMULTIHOP, "EMULTIHOP" },
	{ EBADMSG, "EBADMSG" },
	{ ENOTUNIQ, "ENOTUNIQ" },
	{ EBADFD, "EBADFD" },
	{ EREMCHG, "EREMCHG" },
	{ ELIBACC, "ELIBACC" },
	{ ELIBBAD, "ELIBBAD" },
	{ ELIBSCN, "ELIBSCN" },
	{ ELIBMAX, "ELIBMAX" },
	{ ELIBEXEC, "ELIBEXEC" },
	{ ERESTART, "ERESTART" },
	{ ESTRPIPE, "ESTRPIPE" },
	{ EDQUOT, "EDQUOT" },
#ifndef SOLARIS
	{ EDOTDOT, "EDOTDOT" },
	{ EUCLEAN, "EUCLEAN" },
	{ ENOTNAM, "ENOTNAM" },
	{ ENAVAIL, "ENAVAIL" },
	{ EISNAM, "EISNAM" },
	{ EREMOTEIO, "EREMOTEIO" },
	{ ENOMEDIUM, "ENOMEDIUM" },
	{ EMEDIUMTYPE, "EMEDIUMTYPE" },
#endif
#endif
#ifndef _POSIX_SOURCE
	{ ENOTBLK, "ENOTBLK" },
	{ ETXTBSY, "ETXTBSY" },
	{ ENOTSOCK, "ENOTSOCK" },
	{ EDESTADDRREQ, "EDESTADDRREQ" },
	{ EMSGSIZE, "EMSGSIZE" },
	{ EPROTOTYPE, "EPROTOTYPE" },
	{ ENOPROTOOPT, "ENOPROTOOPT" },
	{ EPROTONOSUPPORT, "EPROTONOSUPPORT" },
	{ ESOCKTNOSUPPORT, "ESOCKTNOSUPPORT" },
	{ EOPNOTSUPP, "EOPNOTSUPP" },
	{ EPFNOSUPPORT, "EPFNOSUPPORT" },
	{ EAFNOSUPPORT, "EAFNOSUPPORT" },
	{ EADDRINUSE, "EADDRINUSE" },
	{ EADDRNOTAVAIL, "EADDRNOTAVAIL" },
	{ ENETDOWN, "ENETDOWN" },
	{ ENETUNREACH, "ENETUNREACH" },
	{ ENETRESET, "ENETRESET" },
	{ ECONNABORTED, "ECONNABORTED" },
	{ ECONNRESET, "ECONNRESET" },
	{ ENOBUFS, "ENOBUFS" },
	{ EISCONN, "EISCONN" },
	{ ENOTCONN, "ENOTCONN" },
	{ ESHUTDOWN, "ESHUTDOWN" },
	{ ETOOMANYREFS, "ETOOMANYREFS" },
	{ ETIMEDOUT, "ETIMEDOUT" },
	{ ECONNREFUSED, "ECONNREFUSED" },
	{ ELOOP, "ELOOP" },
	{ EWOULDBLOCK, "EWOULDBLOCK" },
	{ EALREADY, "EALREADY" },
	{ EINPROGRESS, "EINPROGRESS" },
	{ EHOSTDOWN, "EHOSTDOWN" },
	{ EHOSTUNREACH, "EHOSTUNREACH" },
	{ ESTALE, "ESTALE" },
	{ EUSERS, "EUSERS" },
#ifndef DARWIN
	{ ENOMSG, "ENOMSG" },
	{ EIDRM, "EIDRM" },
	{ EILSEQ, "EILSEQ" },
#endif /* DARWIN */
	{ EREMOTE, "EREMOTE" },
	{ EOVERFLOW, "EOVERFLOW" },
#endif
	{ -EAI_AGAIN, "EAI_AGAIN" },
	{ -EAI_BADFLAGS, "EAI_BADFLAGS" },
	{ -EAI_FAIL, "EAI_FAIL" },
	{ -EAI_FAMILY, "EAI_FAMILY" },
	{ -EAI_MEMORY, "EAI_MEMORY" },
	{ -EAI_NONAME, "EAI_NONAME" },
	{ -EAI_SERVICE, "EAI_SERVICE" },
	{ -EAI_SOCKTYPE, "EAI_SOCKTYPE" },
	{ -EAI_SYSTEM, "EAI_SYSTEM" },
#ifdef EAI_OVERFLOW
	{ -EAI_OVERFLOW, "EAI_OVERFLOW" },
#endif
	{ EC_EINTERNAL, "EC_EINTERNAL" },
	{ 0, NULL}
};
/*[errsymbol]*/
const char *errsymbol(int errno_arg)
{
	int i;

	for (i = 0; errcodes[i].str != NULL; i++)
		if (errcodes[i].code == errno_arg)
			return errcodes[i].str;
	return "[UnknownSymbol]";
}
#endif /* skip errcodes */
const char *errsymbol(int errno_arg)
{
	return get_macrostr("errno", errno_arg, NULL);
}
/*[]*/
char *syserrmsgline(char *buf, size_t buf_max, int errno_arg, EC_ERRTYPE type)
{
	const char *errmsg;
	char *cat = "?";

	if (errno_arg == 0)
		snprintf(buf, buf_max, "No error");
	else {
		if (errno_arg == EC_EINTERNAL)
			errmsg = "Internal error (nonstandard)";
		else if (type == EC_EAI) {
			cat = "eai";
			errmsg = gai_strerror(errno_arg);
		}
		else if (type == EC_GETDATE)
			errmsg = getdate_strerror(errno_arg);
		else {
			cat = "errno";
			errmsg = strerror(errno_arg);
		}
		snprintf(buf, buf_max, "%s (%d: \"%s\")",
		  get_macrostr(cat, errno_arg, NULL), errno_arg,
		  errmsg != NULL ? errmsg : "no message string");
	}
	return buf;
}
/*
	The book shows syserrmsg, which is now a macro that calls
	syserrmsgtype, because the latter has one additional argument.
*/
/*[syserrmsg-1]*/
char *syserrmsgtype(char *buf, size_t buf_max, const char *msg,
  int errno_arg, EC_ERRTYPE type)
{
	const char *errmsg;
	char *cat = "?";

	if (msg == NULL)
		msg = "???";
	if (errno_arg == 0)
		snprintf(buf, buf_max, "%s", msg);
	else {
/*[]*/
		if (errno_arg == EC_EINTERNAL)
			errmsg = "Internal error (nonstandard)";
		else if (type == EC_EAI) {
			cat = "eai";
			errmsg = gai_strerror(errno_arg);
		}
		else if (type == EC_GETDATE)
			errmsg = getdate_strerror(errno_arg);
		else {
			cat = "errno";
			errmsg = (errno_arg < 1000)? strerror(errno_arg) : strerror_custom(errno_arg) ;
		}
/*[syserrmsg-2]*/
		/*errmsg = strerror(errno_arg);*/
		snprintf(buf, buf_max, "%s\n\t\t*** %s (%d: \"%s\") ***", msg,
		get_macrostr(cat, errno_arg, NULL), errno_arg,
		errmsg != NULL ? errmsg : "no message string");
	}
	return buf;
}
/*[]*/

void syserr(const char *msg)  /* print system call error message and terminate */
{
	syserr_print(msg);
	exit(EXIT_FAILURE);
}
/*[syserr_print]*/
void syserr_print(const char *msg)
{
	char buf[200];

	fprintf(stderr, "ERROR: %s\n", syserrmsgtype(buf, sizeof(buf), msg,
	  errno, EC_ERRNO));
}
/*[]*/

const char *getdate_strerror(int e)
{
	const char *s[] = {
		"Invalid getdate_err value",
		"DATEMSK environment variable null or undefined",
		"Template file cannot be opened for reading",
		"Failed to get file status information",
		"Template file not a regular file",
		"I/O error encountered while reading template file",
		"Memory allocation failed",
		"No line in template that matches input",
		"Invalid input specification"
	};
	if (e < 1 || e > 8)
		return s[0];
	return s[e];
}
#endif
