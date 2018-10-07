/*
	timestart and timestop
	Sec. 1.7.2

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

/*[timestart]*/
#include <sys/times.h>

static struct tms tbuf1;
static clock_t real1;
static long clock_ticks;

void timestart(void)
{
	ec_neg1( real1 = times(&tbuf1) )
	/* treat all -1 returns as errors */
	ec_neg1( clock_ticks = sysconf(_SC_CLK_TCK) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("timestart");
EC_CLEANUP_END
}

void timestop(char *msg)
{
	struct tms tbuf2;
	clock_t real2;

	ec_neg1( real2 = times(&tbuf2) )
	fprintf(stderr,"%s:\n\t\"Total (user/sys/real)\", %.2f, %.2f, %.2f\n"
	  "\t\"Child (user/sys)\", %.2f, %.2f\n", msg,
	  ((double)(tbuf2.tms_utime + tbuf2.tms_cutime) -
	  (tbuf1.tms_utime + tbuf1.tms_cutime)) / clock_ticks,
	  ((double)(tbuf2.tms_stime + tbuf2.tms_cstime) -
	  (tbuf1.tms_stime + tbuf1.tms_cstime)) / clock_ticks,
	  (double)(real2 - real1) / clock_ticks,
	  (double)(tbuf2.tms_cutime - tbuf1.tms_cutime) / clock_ticks,
	  (double)(tbuf2.tms_cstime - tbuf1.tms_cstime) / clock_ticks);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("timestop");
EC_CLEANUP_END
}
/*[]*/
