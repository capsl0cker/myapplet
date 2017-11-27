/* This is 1753--flip Game on POJ.
 * use bit operation and DeepSearch;
 * Recursive method;
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int min = 0x7fffffff;
int state = 0;

int map[16]={0xc800,0xe400,0x7200,0x3100,
	0x8c80,0x4e40,0x2720,0x1310,0x08c8,0x04e4,0x0272,
	0x0131,0x008c,0x004e,0x0027,0x0013};

void flip(int level)
{
	if(level + 4 <16)
		state ^=(1<<(level+4));
	if(level -4 > 0)
		state ^=(1<<(level - 4));
	if(level % 4 != 0)
		state ^=(1<<(level - 1));
	if(level % 4 != 3)
		state ^= (1<<(level + 1));
	state ^=(1<<level);
}

int isOK(void)
{
	return ((state == 0 || state == 65535) ? 1:0);
}

void travel(int level, int times)
{
	if(level <= 16 && isOK())
	{
			if(times < min)
				min = times;
			return ;
	}
	if(level < 16)
	{
//		flip(level);
		state ^= map[level];
		travel(level + 1,times + 1);
//		flip(level);
		state ^= map[level];
		travel(level + 1,times);
	}
}

int main(void)
{
	int i;
	char c;
	i = 0;
	freopen("in.txt","r",stdin);
	while(i < 16)
	{
		c = getchar();
		if(c == '\n')
			continue;
		if(c == 'b')
			state |= (1<<i);
		i++;
	}
	travel(0, 0);
	if(min == 0x7fffffff)
		printf("Impossible\n");
	else
		printf("%d\n",min);
	return 0;
}

