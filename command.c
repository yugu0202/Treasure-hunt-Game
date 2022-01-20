#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"
#include "command.h"

//相対パス to 絶対パス
void PathConvert(const char* path,const char* base,char* ret)
{
	const char *pp, *bp;
	char *rp;
	int length;

	if (*path == '/')
	{
		strcpy(ret,path);
		return;
	}

	if (!strcmp(path,".") || !strcmp(path,"./"))
	{
		strcpy(ret,base);
		return;
	}

	length = strlen(base);
	bp = base + length;

	if (*(bp - 1) == '/') --bp;

	for (pp = path; *pp != '\0' && *pp == '.';)
	{
		if (!strncmp(pp,"../",3))
		{
			pp += 3;
			while (bp > base && *--bp != '/');
		}
		else if (!strncmp(pp,"./",2))
		{
			pp += 2;
		}
		else if (!strncmp(pp,"..\0",3))
		{
			pp += 2;
			while (bp > base && *--bp != '/');
		}
		else
		{
			break;
		}
	}

	length = bp - base;
	strncpy(ret,base,length);

	rp = ret + length;
	*rp++ = '/';

	strcpy(rp,pp);
	return;
}

//ファイルの中身を読み取る place:現在のディレクトリ in:ファイル名 out:結果
void GetFile(char* path,char* out)
{
	if (strcmp(path,"/outside/remains/key") == 0)
	{
		strcpy(out,"dGhpc2lza2V5");
	}
}

void GetList(char* path,char* out)
{
	if (!strncmp(path,"/outside/remains",(strlen(path) - 1)))
	{
		strcpy(out,"key\ttreasure-chest.zip");
	}
	else
	{
		strcpy(out,"error:not found");
	}
}

//echo
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


//疑似cat
void ComCat(char* place,char* tp,char* ret,char* pipe,int* pFlag)
{
	char* out = ret;
	char path[256],base[256];
	char in[256];

	strcpy(base,place);

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

	PathConvert(in,base,path);
	GetFile(path,out);
}

//base64 encode & decode
void ComBase64(char* tp,char* ret,char* pipe,int* pFlag)
{
	int dFlag = 0;
	char* out = ret;
	char in[256];

	if (*pFlag == 1) strcpy(in,pipe);

	while (tp != NULL)
	{
		if (strcmp(tp,"-d") == 0)
		{
			dFlag = 1;
		}
		else if (strcmp(tp,"|") == 0)
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


	if (dFlag)
	{
		decode_base64(out,in);
	}
	else
	{
		encode_base64(out,in);
	}

}

void ComLs(char* place,char* tp,char* ret,char* pipe,int* pFlag)
{
	char* out = ret;
	char path[256],base[256],in[256] = "";

	strcpy(base,place);

	if (*pFlag == 1) strcpy(in,pipe);

	while (tp != NULL)
	{
		if (!strcmp(tp,"|"))
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

	PathConvert(in,base,path);
	GetList(path,out);
}
