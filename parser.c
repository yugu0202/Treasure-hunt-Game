#include <stdio.h>
#include <string.h>
#include "parser.h"

Json toStructSub(FILE* fp)
{
	char x;
	char obj[256];
	int flagName = 0,flagAttr = 0,flagPermission = 0,flagInside = 0,flagPasswd = 0;
	Json item,child;

	if (fscanf(fp,"%c",&x) != EOF)
	{
		if (x != '{')
		{
			item.name = "none";
			return item;
		}
	}

	while (fscanf(fp,"%c",&x) != EOF)
	{
		if (x == '\n') continue;
		//区切り文字で最初が何か特定
		if (x == ':')
		{
			if (strcmp(obj,"name") == 0)
			{
				flagName = 1;
			}
			else if (strcmp(obj,"attr") == 0)
			{
				flagAttr = 1;
			}
			else if (strcmp(obj,"permission") == 0)
			{
				flagPermission = 1;
			}
			else if (strcmp(obj,"children") == 0)
			{
				
				child = toStructSub(fp);
				item.children = &child;
			}
			else if (strcmp(obj,"inside") == 0)
			{
				flagInside = 1;
			}
			else if (strcmp(obj,"passwd") == 0)
			{
				flagPasswd = 1;
			}

			obj[0] = '\0';
			continue;
		}

		//次の区切り文字で代入
		if (x == ',')
		{
			if (flagName)
			{
				item.name = obj;
				flagName = 0;
			}
			else if (flagAttr)
			{
				item.attr = obj;
				flagAttr = 0;
			}
			else if (flagPermission)
			{
				item.permission = obj;
				flagPermission = 0;
			}
			else if (flagInside)
			{
				item.inside = obj;
				flagInside = 0;
			}
			else if (flagPasswd)
			{
				item.passwd = obj;
				flagPasswd = 0;
			}

			obj[0] = '\0';
			continue;
		}

		if (x == '}')
		{
			if (flagName)
			{
				item.name = obj;
				flagName = 0;
			}
			else if (flagAttr)
			{
				item.attr = obj;
				flagAttr = 0;
			}
			else if (flagPermission)
			{
				item.permission = obj;
				flagPermission = 0;
			}
			else if (flagInside)
			{
				item.inside = obj;
				flagInside = 0;
			}
			else if (flagPasswd)
			{
				item.passwd = obj;
				flagPasswd = 0;
			}

		return item;
		}

		strcat(obj,x);
	}

}

Json* toStruct(char* file,Json* item)
{
	FILE* fp = NULL;
	Json sItem;

	fp = fopen(file,"r");
	if (fp == NULL)
	{
		perror("open failure");
		return 0;
	}
	
	sItem = toStructSub(fp);

	item = &sItem;
}

void main(void)
{
	char* file = "test.json";
	Json* json;
	toStruct(file,json);

	printf("%s",json->name);
	printf("%s",json->attr);
	printf("%s",json->permission);
	printf("%ls",json->children);
	printf("%s",json->inside);
	printf("%s",json->passwd);
}
