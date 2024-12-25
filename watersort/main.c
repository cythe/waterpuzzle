#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tube.h"
#include "stack.h"
#include "color.h"
#include "load.h"

void printsteps(struct _steps *s)
{
    printf("steps: %d\n", s->cnt);
    for (int i = 0; i < s->cnt; i++)
	printf("\t%d -> %d\n", s->flow[i].src, s->flow[i].dest);
}

void add_step(struct _steps *s, int src, int dest)
{
    s->flow[s->cnt].src = src;
    s->flow[s->cnt].dest = dest;
    s->cnt++;
}

#if 0
int find_next_layer(struct _tube tubes[], int tube_cnt, struct _steps *n)
{
    int cnt = 0;
    int ret;
    struct _steps next; 
    memset(&next, 0, sizeof(struct _steps));

    /* 对每种情况进行模拟 */
    for (int i = 0; i < n->cnt; i++)
    {
	printresults(tubes, tube_cnt, -1, -1, -1);

	ret = pour(&tubes[n->flow[i].src], &tubes[n->flow[i].dest], 0);
	n->flow[i].cnt = ret;
	n->scence++;

	if (is_complete(tubes, tube_cnt)) {
	    printf("--------------------------------------complete.\n");
	    return 0;
	}

	cnt = find_all(tubes, tube_cnt, &next);
	if (!cnt) {
	    printf("this scence is block, try next scence.\n");
	    npour(&tubes[n->flow[i].dest], &tubes[n->flow[i].src], n->flow[i].cnt);
	    continue;
	} else {
	    printf("go ahead, find next layer.\n");
	    struct _tube *new = dup_tubes(tubes, tube_cnt);
	    ret = find_next_layer(new, tube_cnt, &next);
	    if(ret < 0) {
		npour(&tubes[n->flow[i].dest], &tubes[n->flow[i].src], n->flow[i].cnt);
		printf("this road is block, return to last node.\n");
	    } else {
		printf("this road is a results, has been done.\n");
		return 0;
	    }
	}
    }
	    
    printf("emmmm.....\n");
    return -1;
}

/* 递归实现 */
int sort(struct _tube arr[], int tube_cnt)
{
    int ret;
    int cnt = 0;
    struct _steps next; 
    memset(&next, 0, sizeof(struct _steps));

    if (is_complete(arr, tube_cnt)) {
	printf("It's a sorted arr, complete.\n");
	return 0;
    }

    cnt = find_all(arr, tube_cnt, &next);
    if (!cnt) {
	printf("It's a unsortable arr.\n");
	return -1;
    }

    /* next is the first layer steps. */
    ret = find_next_layer(arr, tube_cnt, &next);
    if(!ret)
	printf("complete.\n");
    else
	printf("failed.\n");
}
#else
/*----------------------------------------------------------------------*
 * @brief : 找到一个layer的所有可能情况
 *
 * @param : arr[]: 所有试管
 * @param : tube_cnt: 试管数量
 * @param : s: 每种情况应该如何执行
 *
 * @ret   : 有多少种可能的情况, 如果是0表示无法执行了. layer为终态.
 * ----------------------------------------------------------------------*/
int find_all(struct _tube arr[], int tube_cnt, struct _steps *s)
{
    int cnt = 0;
    memset(s, 0, sizeof(struct _steps));
    for (int i = 0; i < tube_cnt; i++)
    {
	for (int j = 0; j < tube_cnt; j++)
	{
	    if (j == i)
		continue;
	    if (can_pour(&arr[i], &arr[j])) {
		/* add this step to list */
		add_step(s, i, j);
		cnt ++ ;
	    }
	}
    }

    return cnt;
}

/* 通过栈和回溯实现 */
void sort(struct _tube arr[], int tube_cnt)
{
    struct _steps curr[MAX_STEP];
    memset(curr, 0, sizeof(MAX_STEP * sizeof(struct _steps)));
    struct _from_to tp;
    int ret;
    int p_curr;
    int s;

    struct _stack stack;

    stack_init(&stack);

    for (s = 0; s < MAX_STEP; )
    {
	ret = find_all(arr, tube_cnt, &curr[s]); // 找到这一步所有情况
	if (ret == 0) {
	    // 如果没有下一步, 需要回溯
revert:
	    if (s > 0) {
		s--;
	    } else {
		return;
	    }

	    stack_pop(&stack, &tp);
	    npour(&arr[tp.dest], &arr[tp.src], tp.cnt);
	}

	p_curr = curr[s].scence;
	if (p_curr >= curr[s].cnt) {
	    goto revert;
	}

	ret = pour(&arr[curr[s].flow[p_curr].src], &arr[curr[s].flow[p_curr].dest], 0);
	curr[s].scence++;
	curr[s].flow[p_curr].cnt = ret;
	stack_push(&stack, curr[s].flow[p_curr]);
	s++;
	if (is_complete(arr, tube_cnt)) {
	    printf("complete!\n");
	    break;
	}
    }

    /* 打印最终结果 */
    while (s--)
    {
	stack_pop(&stack, &tp);
	printf("exec: %d -> %d, %d\n", tp.src+1, tp.dest+1, tp.cnt);
	npour(&arr[tp.dest], &arr[tp.src], tp.cnt);
	printresults(arr, tube_cnt, tp.src, tp.dest, tp.cnt);
    }
    // stack_print(&stack);
}
#endif

int main(int argc, char* argv[])
{
    load_map();

    sort(g_tubes, g_tube_cnt);

    return 0;
}
