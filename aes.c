#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystem.h"
#include "aes.h"

//boner

void DecodeAes(char* passwd,Item* root,Item* inFile,char* outName,char* out,char* path)
{
	Item* node = (Item*)malloc(sizeof(Item));

	printf("dec %s\n",inFile->name);
	if (!strcmp(passwd,"boner"))
	{
		append(outName,0,inFile->parent,node);
		SetContent(root,path,"tesst");
	}
	else strcpy(out,"bad decrypt");
}
