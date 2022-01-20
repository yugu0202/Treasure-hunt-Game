#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

void GetFile(char* place,char* in,char* out)
{
	if (strcmp(place,"/home/room") == 0)
	{
		if (strcmp(in,"key") == 0)
		{
			strcpy(out,"dGhpc2lza2V5");
		}
		else
		{
			strcpy(out,"error file not found");
		}
	}
}

void ComEcho(char* tp,char* ret,char* pipe,int* pFlag)
{
	char* out = ret;
	char in[256];

	if (*pFlag == 1) strcpy(in,pipe);

	while (tp != NULL)
	{
		if (strcmp(tp,"|") == 0)
		{
			out = pipe;
			*pFlag = 1;
			break;
		}
		else
		{
			strcpy(in,tp);
		}

		tp = strtok(NULL," ");
	}

	strcpy(out,in);
}

void ComCat(char* place,char* tp,char* ret,char* pipe,int* pFlag)
{
	char* out = ret;
	char in[256];

	if (*pFlag == 1) strcpy(in,pipe);

	while (tp != NULL)
	{
		if (strcmp(tp,"|") == 0)
		{
			out = pipe;
			*pFlag = 1;
			break;
		}
		else
		{
			strcpy(in,tp);
		}

		tp = strtok(NULL," ");
	}

	GetFile(place,in,out);
}
