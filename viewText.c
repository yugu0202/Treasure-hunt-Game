#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include "viewText.h"

void VT(int h,int w,char* text)
{
	char* tp;

	int x,y;
	int bh,bw,tw,tx,ty;
	int a,i;

	char v[2];
	char test[] = "Press Q or Enter close";

	getmaxyx(stdscr,h,w);

	tp = text;

	bh = (h - h/5);
	bw = (w - w/5);

	tw = (bw - bw/5);

	y = h/10;
	x = (w-bw)/2;

	ty = h/5;
	tx = (bw-tw)/2;

	for (a = 0; a <= bh; a++)
	{
		move(y+a,x);
		for (i = 0; i <= bw; i++)
		{
			if (a == 0 || a == bh) addstr("#");
			else
			{
				if (i == 1 || i == bw) addstr("#");
				else if (a == ty && i == tx)
				{
					while (i-tx < tw)
					{
						if (*tp == '\0')
						{
							addstr(" ");
						}
						else
						{
							v[0] = *tp;
							v[1] = '\0';
							addstr(v);
							tp++;
						}

						move(y+a,x+i);
						i++;
					}
				}
				else addstr(" ");
			}
			move(y+a,x+i);
		}
	}

	move(y+bh+1,x+bw-strlen(test));
	addstr(test);
}

void ViewText(char* text)
{
	int h,w;
	int ch;

	getmaxyx(stdscr,h,w);

	while (1)
	{
		erase();

		VT(h,w,text);

		refresh();

		//q or enter:exit
		ch = getch();
		if (ch == 'q' || ch == 10) break;
	}
}

