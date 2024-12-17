#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "basetype.h"
#include "load.h"
#include "color.h"

struct _choiced g_choiced;
struct _tube * g_tubes;
int g_tube_cnt = 0;

struct _tube tubes[] = {
#if 0
    /* case 1 */
    {4, 38, 84, 9,  9},
    {4, 9,  9,  84, 38},
    {4, 84,  84,  38, 38},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
#endif
#if 0
    /* case 2 */
    {4, 38, 9,  9, 9},
    {4, 84, 9,  84, 38},
    {4, 84,  84,  38, 38},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
#endif
#if 0
    /* case 117 */
    {4, 84, 22, 93,  38},
    {4, 218,   240,   84, 21},
    {4, 38,  21,  38,  21},
    {4, 22, 93,  214, 9},
    {4, 38,  9,    214, 84},
    {4, 22, 214, 226, 9},
    {4, 93,  9,    79,  79},
    {4, 84, 79, 240,    79},
    {4, 226, 94, 21, 226},
    {4, 218, 214, 93, 94},
    {4, 240, 226, 94, 240},
    {4, 218, 22, 94, 218},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
#endif
#if 1
    /* case 119 */
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
    {0, COL_NULL, COL_NULL, COL_NULL, COL_NULL},
    {4, 14, 2,  211,196 },
    {4, 4 , 11, 11, 5  },
    {4, 3 , 196,211,4  },
    {4, 5 , 196,13, 3  },
    {4, 14, 13, 13, 1  },
    {4, 7 , 10, 2,  7  },
    {4, 4 , 7,  11, 1  },
    {4, 196,1,  5,  10 },
    {4, 1,  10, 13, 5  },
    {4, 211,14, 3,  4  },
    {4, 11, 10, 3,  2  },
    {4, 14, 7,  2,  211},
#endif
};

static void load_map_from_file(void)
{
    FILE *file = fopen("cfg.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
	exit(1);
    }

    unsigned int colors[MAX_TUBE_CNT][TUBE_FLOOR] = {0};  // 用于存储颜色的二维数组
    int row = 0;

    // 读取文件中的每一行
    while (fscanf(file, "0x%X 0x%X 0x%X 0x%X\n",
		&colors[g_tube_cnt][0], &colors[g_tube_cnt][1],
		&colors[g_tube_cnt][2], &colors[g_tube_cnt][3]) == 4) {
	g_tube_cnt++;
    }

    fclose(file);

    // 打印读取的颜色值，供验证
    for (int i = 0; i < g_tube_cnt; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < TUBE_FLOOR; j++) {
            printf("0x%X ", colors[i][j]);
        }
        printf("\n");
    }

    g_tubes = (struct _tube*)malloc(g_tube_cnt * sizeof(struct _tube));

    for(int i = 0; i < g_tube_cnt; i++)
    {
	for (int j = 0; j < TUBE_FLOOR; j++)
	{
	    g_tubes[i].colors[TUBE_FLOOR-j-1] = colors[i][j];
	}
	format_tube(&g_tubes[i]);
	printtube(&g_tubes[i]);
    }

    printresults(g_tubes, g_tube_cnt, -1,-1,-1);
}

static int is_not_exist(int color)
{
    for (int i = 0; i < g_choiced.cnt; i++)
	if (color == g_choiced.colors[i])
	    return 0;

    return 1;
}

static void show_choice(void)
{
    for (int i = 0; i < g_choiced.cnt; i++)
	printcolor(g_choiced.colors[i], 0);
    printf("\n");
}

static void init_choiced(void)
{
    int color;
    int ch;
    memset(&g_choiced, 0, sizeof(struct _choiced));

    do{
	show_choice();
	printf("==== init color panton ====\n"
		"In this section, you can select colors you may use.\n"
		"You also can pick colors in next step with a little tedious.\n"
		"[ -1]: show all colors\n"
		"[999]: End\n"
		"Choice a color: ");
	fflush(stdout);

	scanf("%d", &color);
	if (color < 0) {
	    print_std256color();
	    continue;
	}
	if (color > 256) {
	    break;
	}

	if (is_not_exist(color))
	{
	    g_choiced.colors[g_choiced.cnt] = color;
	    g_choiced.cnt++ ;
	}
    } while(1);
}

static void deinit_choiced(void)
{
    memset(&g_choiced, 0, sizeof(struct _choiced));
}

static void load_map_from_stdin(void)
{
    int color_cnt;
    int color;
    char ch;
	int j = 0;

    printf("Please input tube cnt (include empty tube): ");
    fflush(stdout);
    scanf("%d", &g_tube_cnt); 
    g_tubes = (struct _tube*)malloc(g_tube_cnt * sizeof(struct _tube));

    for(int i = 0; i < g_tube_cnt; i++)
    {
repick:
	j = 0;
	show_choice();
	printf("==== choice color for tube[%d] ====\n"
		"format: [color1 color2 color3 color4]\n"
		"         top ---------------> bottom\n"
		"[ -1]: show all colors\n"
		"[ 0 ]: for NULL empty tube is [0 0 0 0] \n"
		"choice color: ", i);
	fflush(stdout);

	while(j < TUBE_FLOOR)
	{
	    scanf("%d", &color);
	    if (color < 0) {
		print_std256color();
		goto repick;
	//	continue;
	    }

	    g_tubes[i].colors[TUBE_FLOOR-j-1] = color;
	    j++;
	    if (is_not_exist(color))
	    {
		printf("This color is not exist at list!!!\n");
		color_cnt = g_choiced.cnt;
		g_choiced.colors[g_choiced.cnt] = color;
		g_choiced.cnt++ ;
	    }
	}

	format_tube(&g_tubes[i]);
	printtube(&g_tubes[i]);
	printf("ok? [y/n] ");
	fflush(stdout);
	getchar();
	scanf("%c", &ch);
	printf("ch = %c\n", ch);
	if (ch == 'n')
	{
	    color_cnt = g_choiced.cnt;
	    goto repick;
	}
    }

    printresults(g_tubes, g_tube_cnt, -1,-1,-1);
}

void load_map(void)
{
    int type;

    init_choiced();
    printf("------- Select map type -------\n");
    printf("\t1 - from stdin\n");
    printf("\t2 - from file\n");
    printf("\t3 - from inside case\n");
    printf("Please input map type: ");
    fflush(stdout);
    scanf("%d", &type);
    switch(type)
    {
	case 1:
	    load_map_from_stdin();
	    break;
	case 2:
	    load_map_from_file();
	    break;
	case 3:
	    g_tubes = tubes;
	    g_tube_cnt = sizeof(tubes) / sizeof(tubes[0]);
	    printf("tube_cnt = %d\n", g_tube_cnt);
	    break;
	default:
	    printf("Get out !\n");
	    exit(-1);
    }
    deinit_choiced();
}
