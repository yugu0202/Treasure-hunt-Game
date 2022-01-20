#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "command.h"

#define HISTORY 100

//escape game

int h,w;

char (*commandLog)[256];


int Title()
{
	int x,y;
	char titleText[][256] = {"command treasure hunt","[s] start","[q] quit"};

	erase();

	for (int i = 0; i < 3; i++)
	{
		y = (h/2) + i*2;
		x = (w - strlen(titleText[i]))/2;
		move(y,x);
		addstr(titleText[i]);
	}

	refresh();

	return getch();
}

void Shift(char (*arr)[w],char text[])
{
	for (int i = (h-1); i >= 0; i--)
	{
		strcpy(arr[i],arr[i-1]);
	}
	strcpy(arr[0],text);
}

void CommandAnalysisSub(char* place,char* tp,char* ret,int* gameFlag)
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
			ComCat(place,tp,ret,pipe,&pFlag);
		}
		else if (!strcmp(tp,"ls"))
		{
			tp = strtok(NULL," ");
			//call ls
			ComLs(place,tp,ret,pipe,&pFlag);
		}
		else if (strcmp(tp,"exit") == 0)
		{
			*gameFlag = 0;
			strcpy(ret,"End.");
			break;
		}

		tp = strtok(NULL," ");
	}

}

void CommandAnalysis(char* place,char* buf,char (*log)[w],int* gameFlag)
{
	char logText[256] = "";
	char command[256];

	strcpy(command,buf);

	char* tp = strtok(buf," ");

	if (buf[0] == '\0')
	{
		return;
	}

	CommandAnalysisSub(place,tp,logText,gameFlag);

	if (logText[0] != '\0' && strcmp(commandLog[0],command) != 0)
	{
		for (int i = HISTORY; i >= 0; i--)
		{
			strcpy(commandLog[i],commandLog[i-1]);
		}
		strcpy(commandLog[0],command);
	}

	Shift(log,logText);
	logText[0] = '\0';

}

void Game()
{
	int x = 0,y,ch;
	int gameFlag = 1;
	int before = -1;
	char place[] = "/outside/remains";
	char lineFormat[] = "treasure@treasure-hunt %s $ %s",bufFormat[] = "%s%c";
	char viewText[w],buf[256] = "";

	char (*log)[w];

	log = malloc(sizeof(char) * (h-1) * w);

	for (int i = 0; i < (h-1); i++)
	{
		strcpy(log[i],"");
	}

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

			CommandAnalysis(place,buf,log,&gameFlag);
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

int main(void)
{
	int ch;

	commandLog = malloc(sizeof(char) * HISTORY * 256);

	for (int i = 0; i < HISTORY; i++)
	{
		strcpy(commandLog[i],"");
	}

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
	}

	endwin();


	return 0;
}

