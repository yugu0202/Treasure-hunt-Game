#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystem.h"

void append(char* name,int type,Item* parent,Item* node)
{
	node->name = name;
	node->type = type;
	node->child = NULL;
	node->next = NULL;
	node->parent = parent;

	if (parent == NULL) return;

	if (parent->child == NULL)
	{
		parent->child = node;
	}
	else
	{
		Item* last = parent->child;
		while (last->next != NULL)
		{
			last = last->next;
		}
		last->next = node;
	}
}

void SetContent(Item* root,char* path,char* content)
{
	Item* dfs;

	dfs = GetItem(root,path);
	if (dfs == NULL) return;

	strcpy(dfs->content,content);
}

Item* ItemSearch(Item* dfs,char* name)
{
	Item* search = dfs->child;

	while (1)
	{
		if (!strcmp(search->name,name)) break;
		if (search->next == NULL) return NULL;

		search = search->next;
	}

	return search;
}

Item* GetItem(Item* root,char* path)
{
	Item* dfs;
	char* pp;
	char name[128];
	int i;

	dfs = root;

	if (!strcmp(path,"/"))
	{
		return dfs;
	}

	pp = path;
	pp++;

	for (i = 0; *pp != '\0';pp++)
	{
		if (*pp == '/')
		{
			name[i] = '\0';
			dfs = ItemSearch(dfs,name);
			i = 0;
		}
		else
		{
			name[i] = *pp;
			i++;
		}

		if (dfs == NULL) return NULL;
	}

	name[i] = '\0';
	if (strcmp(name,"")) dfs = ItemSearch(dfs,name);

	if (dfs == NULL) return NULL;

	return dfs;
}

void ChildCount(int* count,Item* dfs)
{
	Item* search = dfs->child;

	while (search->next != NULL)
	{
		search = search->next;
		(*count)++;
	}
}

void ItemList(Item** dfses,Item* dfs)
{
	int i = 0;
	Item* search = dfs->child;

	dfses[i] = search;

	while (search->next != NULL)
	{
		search = search->next;
		i++;
		dfses[i] = search;
	}
}

Item** GetItemList(Item* root,char* path)
{
	Item** dfses;
	Item* dfs;
	int count=1;

	dfs = GetItem(root,path);

	if (dfs->type != 1) return NULL;

	ChildCount(&count,dfs);
	dfses = (Item**)malloc(count * sizeof(Item*));
	ItemList(dfses,dfs);

	return dfses;
}

void CreateFS(Item* root)
{
	Item* outside = (Item*)malloc(sizeof(Item));
	Item* remains = (Item*)malloc(sizeof(Item));
	Item* key = (Item*)malloc(sizeof(Item));
	Item* treasureChest = (Item*)malloc(sizeof(Item));

	append("/",1,NULL,root);
	append("outside",1,root,outside);
	append("remains",1,outside,remains);
	append("key",0,remains,key);
	key->content = "Ym9uZXI=";
	append("treasure-chest",2,remains,treasureChest);
}

/*
void main()
{
	Item root;
	Item** dfses;
	char path[] = "/outside/remains";

	CreateFS(&root);
	dfses = GetItemList(&root,path);

	for (int i = 0; dfses[i] != NULL; i++)
	{
		printf("%s\n",dfses[i]->name);
	}
}
*/
