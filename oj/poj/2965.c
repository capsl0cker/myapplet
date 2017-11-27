#include <stdio.h>
#include <stdlib.h>

struct point{
	int x,y;
};
int state[4][4];
int min = 0x7fffffff;

struct point path[16];
struct point best[16];

void save(void)
{
	int i;
	for(i = 0;i < 16;i++){
		best[i].x = path[i].x;
		best[i].y = path[i].y;
	}
}

void change(int level)
{
	int i;
	int x,y;
	x = level / 4;
	y = level % 4;
	for(i = 0;i < 4;i++)
		state[x][i] ^= 1;
	for(i = 0;i < 4;i++)
		if(i != x)
			state[i][y] ^= 1;
}

int isOK(void)
{
	int i,j;
	for(i = 0;i < 4;i++)
		for(j = 0;j < 4;j++)
			if(state[i][j] != 1)
				return 0;
	return 1;
}


void travel(int level,int times)
{
	if(level <= 16 && isOK())
	{
		if(times < min)
		{
			min = times;
			save();
		}
		return ;
	}
	if(level < 16)
	{
		change(level);
		path[times].x = level / 4;
		path[times].y = level % 4;
		travel(level + 1, times + 1);
		change(level);
		path[times].x = path[times].y = -1;
		travel(level + 1, times);
	}
}


int main(void)
{
	int i,j;
	char c;
	i = 0;
	freopen("in.txt","r",stdin);
	while(i < 16)
	{
		c = getchar();
		if(c == '\n')
			continue;
		if(c == '-')
			state[i/4][i%4] = 1;
		else if(c == '+')
			state[i/4][i%4] = 0;
		i++;
	}
	travel(0,0);
	printf("%d\n",min);
	for(i = 0;i < min;i++)
			printf("%d %d\n",best[i].x+1,best[i].y+1);
	return 0;
}
