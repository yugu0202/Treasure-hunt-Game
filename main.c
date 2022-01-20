#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "base64.h"
#include "command.h"

//escape game

int h,w;

int Title()
{
	int x,y;
	char titleText[][256] = {"command escape game","[s] start","[q] quit"};

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

void CommandAnalysisSub(char* place,char* tp,char* ret)
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
			Base64(tp,ret,pipe,&pFlag);
		}
		else if (strcmp(tp,"cat") == 0)
		{
			tp = strtok(NULL," ");
			//call cat
			ComCat(place,tp,ret,pipe,&pFlag);
		}

		tp = strtok(NULL," ");
	}

}

void CommandAnalysis(char* place,char* buf,char (*log)[w])
{
	char logText[256] = "";
	char* tp = strtok(buf," ");

	if (buf[0] == '\0')
	{
		return;
	}

	CommandAnalysisSub(place,tp,logText);

	/*
	else if (strcmp(buf,"ls") == 0)
	{
		strcpy(logText,"key\tdoor.tgz");
	}
	else if (strcmp(buf,"cat key") == 0)
	{
		strcpy(logText,"dGhpc2lza2V5");
	}
	*/

	for (int i = (h-1); i >= 0; i--)
	{
		strcpy(log[i],log[i-1]);
	}
	strcpy(log[0],logText);

}

void Game()
{
	int x = 0,y;
	char place[] = "/home/room";
	char lineFormat[] = "escape@escape-game %s $ %s",bufFormat[] = "%s%c";
	char ch,viewText[w],buf[256] = "";

	char (*log)[w];

	log = malloc(sizeof(char) * (h-1) * w);

	while (1)
	{
		sprintf(viewText,lineFormat,place,buf);
		erase();

		y = h - 2;
		for (int i = 0; i < (h-1); i++)
		{
			move(y,x);
			addstr(log[i]);
			y -= 1;
		}
		y = h - 1;
		move(y,x);
		addstr(viewText);
		refresh();

		ch = getch();
		if (ch == 127)
		{
			buf[strlen(buf)-1] = '\0';
		}
		else if (ch == 10)
		{
			Shift(log,viewText);

			CommandAnalysis(place,buf,log);
			buf[0] = '\0';
		}
		else
		{
			sprintf(buf,bufFormat,buf,ch);
		}
	}

	free(log);
}

int main(void)
{
	int ch;

	//screen setting
	initscr();
	noecho();
	cbreak();
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

