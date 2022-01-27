#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystem.h"

void New(char* name,int type,Item* parent,char* content)
{
	Item dfs;
	
	strcpy(dfs.name,name);
	dfs.type = type;
	dfs.childCount = 0;
	dfs.parent = parent;
	if (type == 0)
	{
		for (int i =0; i < 100; i++)
		{
			dfs.child[i] = NULL;
		}
		strcpy(dfs.content,content);
	}
	else
	{
		dfs.content[0] = '\0';
	}

	parent->child[parent->childCount] = &dfs;
	parent->childCount += 1;
}

void First(Item* dfs)
{
	char name[256];
	char content[256];

	dfs->name[0] = '\0';
	dfs->type = 1;
	dfs->childCount = 0;
	dfs->parent = NULL;
	dfs->content[0] = '\0';

	strcpy(name,"outside");
	New(name,1,dfs,content);
}

void ItemSearch(Item* dfs,char* name)
{

}

void GetItem(Item* root,Item* dfs,char* path)
{
	char* pp;
	char name[128];

	dfs = root;

	if (!strcmp(path,"/"))
	{
		return;
	}

	pp = path;
	*pp++;

	for (int i = 0; *pp != '\0';*pp++)
	{
		if (*pp == '/')
		{
			ItemSearch(dfs,name);
			strcpy(name,"");
			i = 0;
		}
		else
		{
			name[i] = *pp;
			i++;
		}
	}

	return;
}

void main()
{
	Item root;
	Item now;
	char path[] = "/outside";

	First(&root);

	GetItem(&root,&now,path);

	printf("%s\n",(now.name));
}
