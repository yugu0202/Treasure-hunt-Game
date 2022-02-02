#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystem.h"
#include "base64.h"
#include "viewText.h"
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
void GetFile(Item* root,char* path,char* out)
{
	Item* dfs;

	dfs = GetItem(root,path);
	if (dfs == NULL) strcpy(out,"file not found");
	else if (dfs->type != 0) strcpy(out,"not file");
	else strcpy(out,dfs->content);
}

//ディレクトリの中身を表示する
void GetList(Item* root,char* path,char* out)
{
	Item** dfses;
	char* op;

	op = out;

	dfses = GetItemList(root,path);

	if (dfses == NULL)
	{
		strcpy(out,"error");
	}
	else
	{
		for (int i = 0; dfses[i] != NULL; i++)
		{
			strcpy(op,dfses[i]->name);
			op += strlen(dfses[i]->name);
			strcpy(op,"\t");
			op += strlen("\t");
		}
	}
}

void Open(char* path,char* passwd,char* out)
{
	if (!strcmp(path,"/outside/remains/treasure-chest") && !strcmp(passwd,"boner"))
	{
		strcpy(out,"Congratulations");
	}
}

void Command(char* ret,char* pipe,int* pFlag)
{
	char* out = ret;
	char text[] = "cat ファイルの中身を表示する base64 base64の操作ができる ls ディレクトリの中身を表示する chestopen 宝箱をあける-inでファイル指定-passでパスワード入力";

	strcpy(out,text);
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
void ComCat(Item* root,char* place,char* tp,char* ret,char* pipe,int* pFlag)
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
	GetFile(root,path,out);
	
	if (!*pFlag)
	{
		ViewText(out);
	}
}

//base64 encode & decode
void ComBase64(char* tp,char* ret,char* pipe,int* pFlag)
{
	int dFlag = 0;
	char* out = ret;
	char in[256];

	if (*pFlag) strcpy(in,pipe);

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
		DecodeBase64(out,in);
	}
	else
	{
		EncodeBase64(out,in);
	}

}

//疑似ls
void ComLs(Item* root,char* place,char* tp,char* ret,char* pipe,int* pFlag)
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
	GetList(root,path,out);
}

/*
void ComOpenssl(Item* root,char* place,char* tp,char* ret,char* pipe,int* pFlag)
{
	int encFlag=0,dFlag=0,aesFlag=0,aFlag=0,pbFlag=0;
	char* out = ret;
	char path[256],base[256],passwd[256]="",inName[256]="",outName[256]="";
	Item* in;

	strcpy(base,place);

	if (*pFlag == 1) strcpy(inName,pipe);

	while (tp != NULL)
	{
		if (!strcmp(tp,"enc"))
		{
			encFlag = 1;
		}
		else if (!strcmp(tp,"-d"))
		{
			dFlag = 1;
		}
		else if (!strcmp(tp,"-a"))
		{
			aFlag = 1;
		}
		else if (!strcmp(tp,"-pbkdf2"))
		{
			pbFlag = 1;
		}
		else if (!strcmp(tp,"-pass"))
		{
			tp = strtok(NULL," ");
			if (!strncmp(tp,"pass:",5))
			{
				tp += 5;
				strcpy(passwd,tp);
			}
		}
		else if (!strcmp(tp,"-aes-256-cbc"))
		{
			aesFlag = 1;
		}
		else if (!strcmp(tp,"-in"))
		{
			tp = strtok(NULL," ");
			strcpy(inName,tp);
		}
		else if (!strcmp(tp,"-out"))
		{
			tp = strtok(NULL," ");
			strcpy(outName,tp);
		}
		else if (!strcmp(tp,"|"))
		{
			out = pipe;
			*pFlag = 1;
			break;
		}

		tp = strtok(NULL," ");
	}

	if (encFlag && dFlag && aFlag && pbFlag && aesFlag)
	{
//strcmp(passwd,"") && strcmp(inName,"") && strcmp(outName,"")
		PathConvert(inName,base,path);
		in = GetItem(root,path);
		if (in == NULL || in->type != 2) return;
		DecodeAes(passwd,root,in,outName,out,path);
	}
}
*/

void ChestOpen(char* place,char* tp,char* ret,char* pipe,int* pFlag)
{
	char* out = ret;
	char path[256],base[256],inName[256]="",passwd[256]="";

	strcpy(base,place);

	while (tp != NULL)
	{
		if (!strcmp(tp,"|"))
		{
			out = pipe;
			*pFlag = 1;
			break;
		}
		else if (!strcmp(tp,"-in"))
		{
			tp = strtok(NULL," ");
			strcpy(inName,tp);
		}
		else if (!strcmp(tp,"-pass"))
		{
			tp = strtok(NULL," ");
			strcpy(passwd,tp);
		}

		tp = strtok(NULL," ");
	}

	PathConvert(inName,base,path);
	Open(path,passwd,out);
}
