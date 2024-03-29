/*
 * ======================================== sl.c: Copyright 1993,1998 Toyoda
 * Masashi (toyoda@is.titech.ac.jp) Last Modified: 1998/ 7/22
 * ========================================
 */
/* sl version 3.03 : add usleep(20000)                                       */
/* by Toyoda Masashi 1998/ 7/22 */
/* sl version 3.02 : D51 flies! Change options.                              */
/* by Toyoda Masashi 1993/ 1/19 */
/* sl version 3.01 : Wheel turns smoother                                    */
/* by Toyoda Masashi 1992/12/25 */
/* sl version 3.00 : Add d(D51) option                                       */
/* by Toyoda Masashi 1992/12/24 */
/* sl version 2.02 : Bug fixed.(dust remains in screen)                      */
/* by Toyoda Masashi 1992/12/17 */
/* sl version 2.01 : Smoke run and disappear.                                */
/* Change '-a' to accident option.			     */
/* by Toyoda Masashi 1992/12/16 */
/* sl version 2.00 : Add a(all),l(long),F(Fly!) options.                     */
/* by Toyoda Masashi 1992/12/15 */
/* sl version 1.02 : Add turning wheel.                                      */
/* by Toyoda Masashi 1992/12/14 */
/* sl version 1.01 : Add more complex smoke.                                 */
/* by Toyoda Masashi 1992/12/14 */
/* sl version 1.00 : SL runs vomitting out smoke.                            */
/* by Toyoda Masashi 1992/12/11 */

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "sl.h"

int		ACCIDENT = 0;
int		LOGO = 0;
int		FLY = 0;

int 
my_mvaddstr(int y, int x, char *str)
{
	for (; x < 0; ++x, ++str)
		if (*str == '\0')
			return ERR;
	for (; *str != '\0'; ++str, ++x)
		if (mvaddch(y, x, *str) == ERR)
			return ERR;
	return OK;
}

void 
option(char *str)
{
	extern int	ACCIDENT, FLY, LONG;

	while (*str != '\0') {
		switch (*str++) {
		case 'a':
			ACCIDENT = 1;
			break;
		case 'F':
			FLY = 1;
			break;
		case 'l':
			LOGO = 1;
			break;
		default:
			break;
		}
	}
}

void 
main(int argc, char *argv[])
{
	int		x         , i;
	int		hoge, color;

	srandom(time(0));
	for (i = 1; i < argc; ++i) {
		if (*argv[i] == '-') {
			option(argv[i] + 1);
		}
	}
	initscr();
	signal(SIGINT, SIG_IGN);
	signal(SIGSTOP, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	//signal(SIGHUP, SIG_IGN);
	//signal(SIGKILL, SIG_IGN);
	//signal(SIGTERM, SIG_IGN);
	//signal(SIGQUIT, SIG_IGN);
	noecho();
	leaveok(stdscr, TRUE);
	scrollok(stdscr, FALSE);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE,COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);


	hoge = random() % 20000;
    color = random() % 3 + 1;

	for (x = COLS - 1;; --x) {

        attron(COLOR_PAIR(color));

		if (LOGO == 0) {
			if (add_D51(x) == ERR)
				break;
		} else {
			if (add_sl(x) == ERR)
				break;
		}
		refresh();
		usleep(500 + hoge);
		if (x == -83) {
			x = COLS - 1;
			if (random() % 3 == 0) if (ACCIDENT == 1) ACCIDENT = 0; else ACCIDENT = 1;
			if (random() % 3 == 0) if (FLY == 1) FLY = 0; else FLY = 1;
			if (random() % 3 == 0) if (LOGO == 1) LOGO = 0; else LOGO = 1;
			hoge = random() % 20000;
		}

        attroff(COLOR_PAIR(color));
	}
	mvcur(0, COLS - 1, LINES - 1, 0);
	endwin();
}


int 
add_sl(int x)
{
	static char    *sl[LOGOPATTERNS][LOGOHIGHT + 1]
	= {{LOGO1, LOGO2, LOGO3, LOGO4, LWHL11, LWHL12, DELLN},
	{LOGO1, LOGO2, LOGO3, LOGO4, LWHL21, LWHL22, DELLN},
	{LOGO1, LOGO2, LOGO3, LOGO4, LWHL31, LWHL32, DELLN},
	{LOGO1, LOGO2, LOGO3, LOGO4, LWHL41, LWHL42, DELLN},
	{LOGO1, LOGO2, LOGO3, LOGO4, LWHL51, LWHL52, DELLN},
	{LOGO1, LOGO2, LOGO3, LOGO4, LWHL61, LWHL62, DELLN}};

	static char    *coal[LOGOHIGHT + 1]
	= {LCOAL1, LCOAL2, LCOAL3, LCOAL4, LCOAL5, LCOAL6, DELLN};

	static char    *car[LOGOHIGHT + 1]
	= {LCAR1, LCAR2, LCAR3, LCAR4, LCAR5, LCAR6, DELLN};

	int		i, y, py1 = 0, py2 = 0, py3 = 0;

	if (x < -LOGOLENGTH)
		return ERR;
	y = LINES / 2 - 3;

	if (FLY == 1) {
		y = (x / 6) + LINES - (COLS / 6) - LOGOHIGHT;
		py1 = 2;
		py2 = 4;
		py3 = 6;
	}
	for (i = 0; i <= LOGOHIGHT; ++i) {
		my_mvaddstr(y + i, x, sl[(LOGOLENGTH + x) / 3 % LOGOPATTERNS][i]);
		my_mvaddstr(y + i + py1, x + 21, coal[i]);
		my_mvaddstr(y + i + py2, x + 42, car[i]);
		my_mvaddstr(y + i + py3, x + 63, car[i]);
	}
	if (ACCIDENT == 1) {
		add_man(y + 1, x + 14);
		add_man(y + 1 + py2, x + 45);
		add_man(y + 1 + py2, x + 53);
		add_man(y + 1 + py3, x + 66);
		add_man(y + 1 + py3, x + 74);
	}
	add_smoke(y - 1, x + LOGOFUNNEL);
	return OK;
}


add_D51(int x)
{
	static char    *d51[D51PATTERNS][D51HIGHT + 1]
	= {{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
	D51WHL11, D51WHL12, D51WHL13, D51DEL},
	{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
	D51WHL21, D51WHL22, D51WHL23, D51DEL},
	{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
	D51WHL31, D51WHL32, D51WHL33, D51DEL},
	{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
	D51WHL41, D51WHL42, D51WHL43, D51DEL},
	{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
	D51WHL51, D51WHL52, D51WHL53, D51DEL},
	{D51STR1, D51STR2, D51STR3, D51STR4, D51STR5, D51STR6, D51STR7,
	D51WHL61, D51WHL62, D51WHL63, D51DEL}};
	static char    *coal[D51HIGHT + 1]
	= {COAL01, COAL02, COAL03, COAL04, COAL05,
	COAL06, COAL07, COAL08, COAL09, COAL10, COALDEL};

	int		y         , i, dy = 0;

	if (x < -D51LENGTH)
		return ERR;
	y = LINES / 2 - 5;

	if (FLY == 1) {
		y = (x / 7) + LINES - (COLS / 7) - D51HIGHT;
		dy = 1;
	}
	for (i = 0; i <= D51HIGHT; ++i) {
		my_mvaddstr(y + i, x, d51[(D51LENGTH + x) % D51PATTERNS][i]);
		my_mvaddstr(y + i + dy, x + 53, coal[i]);
	}
	if (ACCIDENT == 1) {
		add_man(y + 2, x + 43);
		add_man(y + 2, x + 47);
	}
	add_smoke(y - 1, x + D51FUNNEL);
	return OK;
}


int 
add_man(int y, int x)
{
	static char    *man[2][2] = {{"", "(O)"}, {"Help!", "\\O/"}};
	int		i;

	for (i = 0; i < 2; ++i) {
		my_mvaddstr(y + i, x, man[(LOGOLENGTH + x) / 12 % 2][i]);
	}
}


int 
add_smoke(int y, int x)
#define SMOKEPTNS	16
{
	static struct smokes {
		int		y         , x;
		int		ptrn      , kind;
	}		S            [1000];
	static int	sum = 0;
	static char    *Smoke[2][SMOKEPTNS]
	= {{"(   )", "(    )", "(    )", "(   )", "(  )",
			"(  )", "( )", "( )", "()", "()",
			"O", "O", "O", "O", "O",
	" "},
	{"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
		"(@@)", "(@)", "(@)", "@@", "@@",
		"@", "@", "@", "@", "@",
	" "}};
	static char    *Eraser[SMOKEPTNS]
	= {"     ", "      ", "      ", "     ", "    ",
		"    ", "   ", "   ", "  ", "  ",
		" ", " ", " ", " ", " ",
	" "};
	static int	dy   [SMOKEPTNS] = {2, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0};
	static int	dx   [SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1, 2, 2,
	2, 2, 2, 3, 3, 3};
	int		i;

	if (x % 4 == 0) {
		for (i = 0; i < sum; ++i) {
			my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
			S[i].y -= dy[S[i].ptrn];
			S[i].x += dx[S[i].ptrn];
			S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
			my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
		}
		my_mvaddstr(y, x, Smoke[sum % 2][0]);
		S[sum].y = y;
		S[sum].x = x;
		S[sum].ptrn = 0;
		S[sum].kind = sum % 2;
		sum++;
	}
}
