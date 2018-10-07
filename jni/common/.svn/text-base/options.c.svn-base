/*
	Check _POSIX_SYNCHRONIZED_IO and _POSIX_ASYNCHRONOUS_IO
	AUP2, Sec. 1.05.4, 2.16.2

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
#include "defs.h"

/*[option_sync_io]*/
OPT_RETURN option_sync_io(const char *path)
{
#if _POSIX_SYNCHRONIZED_IO <= 0
	return OPT_NO;
#elif _XOPEN_VERSION >= 500 && !defined(LINUX)
	#if !defined(_POSIX_SYNC_IO)
		errno = 0;
		if (pathconf(path, _PC_SYNC_IO) == -1)
			if (errno == 0)
				return OPT_NO;
			else
				EC_FAIL
		else
			return OPT_YES;

	EC_CLEANUP_BGN
		return OPT_ERROR;
	EC_CLEANUP_END
	#elif _POSIX_SYNC_IO == -1
		return OPT_NO;
	#else
		return OPT_YES;
	#endif /* _POSIX_SYNC_IO */
#elif _POSIX_VERSION >= 199309L
	return OPT_YES;
#else
	errno = EINVAL;
	return OPT_ERROR;
#endif /* _POSIX_SYNCHRONIZED_IO */
}
/*[option_async_io]*/
OPT_RETURN option_async_io(const char *path)
{
#if _POSIX_ASYNCHRONOUS_IO <= 0
	return OPT_NO;
#elif _XOPEN_VERSION >= 500 && !defined(LINUX)
	#if !defined(_POSIX_ASYNC_IO)
		errno = 0;
		if (pathconf(path, _PC_ASYNC_IO) == -1)
			if (errno == 0)
				return OPT_NO;
			else
				EC_FAIL
		else
			return OPT_YES;

	EC_CLEANUP_BGN
		return OPT_ERROR;
	EC_CLEANUP_END
	#elif _POSIX_ASYNC_IO == -1
		return OPT_NO;
	#else
		return OPT_YES;
	#endif /* _POSIX_ASYNC_IO */
#elif _POSIX_VERSION >= 199309L
	return OPT_YES;
#else
	errno = EINVAL;
	return OPT_ERROR;
#endif /* _POSIX_ASYNCHRONOUS_IO */
}
/*[]*/
