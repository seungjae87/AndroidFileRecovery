#ifndef __ERRNO_H_C_
#define __ERRNO_H_C_

#include <stdio.h>

// Range >= 1000
#define ETEST			1000 
#define EOBCLUSTER		1001
#define EBADCLUSTER		1002
#define ENOTMATCH		1003
#define EFSIZEZERO		1004
#define EREADZERO		1005
#define EREADSIZE		1006
#define EOVERSIZE_T		1007
#define ESEQFREE		1008
#define EMATCHINFO		1009
//minwoo errocode
#define ELFNNULL		1200
#define ELFNSOVR		1201
#define ERESNULL		1202
#define EDIRNULL		1203
#define EENTMODE		1204
static struct {
	int code;
	char *str;
} errcodestrs[] = 
{
	{ETEST, "Test Error"},
	{EOBCLUSTER, "Cluster number is out of bound"},
	{EBADCLUSTER, "Bad Cluster"},
	{ENOTMATCH, "Cluster number is not match with File size"},
	{EFSIZEZERO, "File size zero is not supported"},
	{EREADZERO, "Cannot read any stuff"},
	{EREADSIZE, "Not equal intent size and real read size"},
	{EOVERSIZE_T, "File size is too big to recover temporarly"},
	{ESEQFREE, "invalid free sequence"},
	{EMATCHINFO, "Nothing match"},
	{ELFNNULL, "Lenght of LFN entry is NULL, c"},
	{ELFNSOVR, "LFN length is larger than MAX"},
	{ERESNULL, "Value of result is NULL"},
	{EDIRNULL, "Value of dirp is NULL"},
	{EENTMODE, "Unknown Entry Mode"},
	{0, NULL},
};
#endif