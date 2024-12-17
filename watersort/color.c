#include <stdio.h>

void printcolor(int c, int blink)
{
    if(c < 0) {
	printf("|   |");
    } else if(c<256) {
	if (blink)
	    printf("|\033[5;48;5;%dm%3d\033[0m|", c, c);
	else
	    printf("|\033[1;48;5;%dm%3d\033[0m|", c, c);
    } else {
	if (blink)
	    printf("|\033[5;48;2;%d;%d;%dm   \033[0m|", (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff);
	else
	    printf("|\033[1;48;2;%d;%d;%dm   \033[0m|", (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff);
    }
}

void print_std256color(void)
{
    int c = 0;
    printf("\n==================== stadard 256 colors ====================\n");
    printf("-------------------- stadard 8 colors --------------------\n");
    for (int i = 0; i < 8; i++)
    {
	printcolor(i, 0);
    }
    printf("\n");

    printf("-------------------- enhance 8 colors --------------------\n");
    for (int i = 8; i < 16; i++)
    {
	printcolor(i, 0);
    }
    printf("\n");

    printf("-------------------- 216 colors --------------------");
    for (int i = 16; i < 232; i++)
    {
	if (c%36 == 0)
	    printf("\n");
	printcolor(i, 0);
	c++;
    }
    printf("\n");

    printf("-------------------- gray --------------------\n");
    for (int i = 232; i < 256; i++)
    {
	printcolor(i, 0);
    }
    printf("\n");
}
