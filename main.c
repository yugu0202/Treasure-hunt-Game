#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include "fileSystem.h"
#include "command.h"

#define HISTORY 100

//treasure hunt game


int h,w;

char (*commandLog)[256];


int Title()
{
	int x,y;
	char titleText[][256] = {"command treasure hunt","[s] start","[q] quit","[h] help"};

	erase();

	for (int i = 0; i < 4; i++)
	{
		y = (h/2) + i*2;
		x = (w - strlen(titleText[i]))/2;
		move(y,x);
		addstr(titleText[i]);
	}

	refresh();

	return getch();
}

void Shift(char (*arr)[w],char* text)
{
	for (int i = (h-1); i >= 0; i--)
	{
		strcpy(arr[i],arr[i-1]);
	}
	strcpy(arr[0],text);
}

void CommandAnalysisSub(Item* root,char* place,char* tp,char* ret,int* gameFlag)
{
	char pipe[256] = "";
	int pFlag = 0;

	while (tp != NULL)
	{
		if (strcmp(tp,"echo") == 0)
		{
			tp = strtok(NULL," ");
			//call echo
			ComEcho(tp,ret,pipe,&pFlag);
		}
		else if (strcmp(tp,"base64") == 0)
		{
			tp = strtok(NULL," ");
			//call openssl
			ComBase64(tp,ret,pipe,&pFlag);
		}
		else if (strcmp(tp,"cat") == 0)
		{
			tp = strtok(NULL," ");
			//call cat
			ComCat(root,place,tp,ret,pipe,&pFlag);
		}
		else if (!strcmp(tp,"ls"))
		{
			tp = strtok(NULL," ");
			//call ls
			ComLs(root,place,tp,ret,pipe,&pFlag);
		}
		else if (!strcmp(tp,"chestopen"))
		{
			tp = strtok(NULL," ");
			//call chestopen
			ChestOpen(place,tp,ret,pipe,&pFlag);
		}
		else if (!strcmp(tp,"command"))
		{
			tp = strtok(NULL," ");
			//call command
			Command(ret,pipe,&pFlag);
		}
		else if (strcmp(tp,"exit") == 0)
		{
			*gameFlag = 0;
			break;
		}

		tp = strtok(NULL," ");
	}

}

void CommandAnalysis(Item* root,char* place,char* buf,char (*log)[w],int* gameFlag)
{
	char logText[256] = "";
	char command[256];

	strcpy(command,buf);

	char* tp = strtok(buf," ");

	for (int i = HISTORY; i >= 1; i--)
	{
		strcpy(commandLog[i],commandLog[i-1]);
	}
	strcpy(commandLog[0],command);

	if (buf[0] == '\0')
	{
		return;
	}

	CommandAnalysisSub(root,place,tp,logText,gameFlag);

	Shift(log,logText);

}

void Game()
{
	int x = 0,y,ch;
	int gameFlag = 1;
	int before = -1;
	char place[] = "/outside/remains";
	Item root;
	char lineFormat[] = "treasure@treasure-hunt %s $ %s",bufFormat[] = "%s%c";
	char viewText[w],buf[256] = "";

	char (*log)[w];

	log = malloc(sizeof(char) * (h-1) * w);

	for (int i = 0; i < (h-1); i++)
	{
		log[i][0] = '\0';
	}

	CreateFS(&root);

	while (gameFlag)
	{
		sprintf(viewText,lineFormat,place,buf);
		erase();

		y = h - 2;
		for (int i = 0; i < (h-1); i++)
		{
			move(y,x);
			addstr(log[i]);
			y--;
		}
		y = h - 1;
		move(y,x);
		addstr(viewText);
		refresh();

		ch = getch();
		if (ch == KEY_BACKSPACE)
		{
			buf[strlen(buf)-1] = '\0';
		}
		else if (ch == 10)
		{
			before = -1;
			Shift(log,viewText);

			CommandAnalysis(&root,place,buf,log,&gameFlag);
			buf[0] = '\0';
		}
		else if (ch == KEY_UP)
		{
			if (before == -1)
			{
				before = 0;
				if (commandLog[before][0] != '\0') strcpy(buf,commandLog[before]);
			}
			else if (commandLog[before+1][0] != '\0')
			{
				before++;
				strcpy(buf,commandLog[before]);
			}
		}
		else if (ch == KEY_DOWN)
		{
			if (before == 0)
			{
				before--;
				strcpy(buf,"");
			}
			else if (before >= 1 && commandLog[before-1][0] != '\0')
			{
				before--;
				strcpy(buf,commandLog[before]);
			}
		}
		else
		{
			sprintf(buf,bufFormat,buf,ch);
		}
	}

	erase();
	refresh();

	free(log);
}

int StrCount(char* str)
{
	int count = 0;

	while (*str != '\0')
	{
		if ((*str & 0xC0) != 0x80) { count++; }
		str++;
	}

	return count;
}

void Help()
{
	int x,y;
	int ch;
	char titleText[][257] = {"command treasure hunt","コマンド操作で宝探しを行うゲームです","ゲーム開始後commandと入力すると使えるコマンドの一覧が出てきます","curl https://hint.treasure-hunting.instituteと入力するとヒントが出ます"};

	erase();

	y = (h/2);
	x = (w - strlen(titleText[0]))/2;
	move(y,x);
	addstr(titleText[0]);
	y = (h/2) + 2;
	x = (w - 38)/2;
	move(y,x);
	addstr(titleText[1]);
	y = (h/2) + 4;
	x = (w - 60)/2;
	move(y,x);
	addstr(titleText[2]);
	y = (h/2) + 6;
	x = (w - 68)/2;
	move(y,x);
	//addstr(titleText[3]);

	refresh();

	while (ch != 'q')
	{
		ch = getch();
	}
}

int main(void)
{
	int ch;

	commandLog = malloc(sizeof(char) * HISTORY * 256);

	for (int i = 0; i < HISTORY; i++)
	{
		commandLog[i][0] = '\0';
	}
	
	setlocale(LC_ALL,"");

	//screen setting
	initscr();
	noecho();
	cbreak();
	keypad(stdscr,TRUE);
	getmaxyx(stdscr,h,w);


	while (1)
	{
		ch = Title(h,w);
		if (ch == 'q') break;
		if (ch == 's') Game(h,w);
		if (ch == 'h') Help(h,w);
	}

	endwin();


	return 0;
}

