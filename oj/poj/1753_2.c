/*
 * poj-1753:flip game
 * 只需要对第一行进行枚举就可以，第一行4个元素，改变或不改变共有16种情况，
 * 对于每一种情况，为了使本行变为全黑或全白，下一行的翻转情况是确定的。
 * state[][] 初始状态，flip[][] 对应的是否翻转
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int state[6][6],flip[6][6];

int isOK(int color)
{
	int i,j;
	for(i = 1;i <=3;i++)
		for(j = 1;j <= 4;j++)
			flip[i+1][j] = (state[i][j] + flip[i][j] + flip[i-1][j] + flip[i][j-1] + flip[i][j+1]) % 2 == color?0:1;
	for(i = 1;i <= 4;i++)
		if((state[4][i] + flip[4][i] + flip[3][i] + flip[4][i-1] + flip[4][i+1])%2 != color)
			return 0;
	return 1;
}



int enuma(int i,int color)
{
	int j,k,count;
	memset(flip,0,sizeof(flip));
	for(j = 0;j < 4;j++)
		if(i&(1<<j))
			flip[1][j+ 1] = 1;
		else
			flip[1][j + 1] = 0;
	if(isOK(color))
	{
		count = 0;
		for(j = 1;j <= 4;j++)
			for(k = 1;k <= 4;k++)
				if(flip[j][k])
					count++;
		return count;
	}
	else
		return 20;
}
int main(void)
{
	int i,j,t1,t2,min;
	char buf[6];
	freopen("in.txt","r",stdin);
	i = 0;
	for(i = 0;i< 4;i++)
	{
		gets(buf);
		for(j = 0;j<4;j++)
			if(buf[j] == 'b')
				state[i+1][j+1] = 1;
			else
				state[i+1][j+1] = 0;
	}
	/*
	while(i < 16)
	{
		c = getchar();
		if(c == '\n')
			continue;
		if(c == 'b')
			state[i/4+1][i%4+1] = 1;
		else
			state[i/4+1][i%4+1] = 0;
		i++;
	}
	*/
	i = 0;
	min = 20;
	for(i = 0;i < 16;i++)
	{
		t1 = enuma(i,0);
		t2 = enuma(i,1);
		if( (t1 > t2 ? t2:t1) < min) min = t1 > t2 ? t2:t1;
	}

	if(min == 20)
		printf("Impossible\n");
	else
		printf("%d\n",min);
	return 0;
}




