/*
 * poj-1222:Extended Lights Out
 * 从首行进行枚举，注意奇偶数关系
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int puzzle[7][8];
int press[7][8];

int isOK(void)
{
	int i;
	for(i = 1;i <= 6;i++)
		if((puzzle[5][i] + press[5][i] + press[4][i] + press[5][i-1] + press[5][i+1]) % 2 != 0)
			return 0;
	return 1;
}

int change(int t)
{
	int i,j;
	memset(press,0,sizeof(press));
	for(i = 0;i <= 5;i++)
		if(t & (1<<i))
			press[1][i+1] = 1;
		else
			press[1][i+1] = 0;
	for(i = 1;i <= 4;i++)
		for(j = 1;j <= 6;j++)
			press[i+1][j] = (puzzle[i][j] + press[i][j] + press[i][j-1] + press[i][j+1] + press[i-1][j]) % 2 == 0? 0:1;
	if(isOK())
		return 1;
	else
		return 0;
}

int main(void)
{
	int m,i,j,k;
	freopen("in.txt","r",stdin);
	scanf("%d",&m);
	k = 0;
	while(k++ < m)
	{
		for(i=0;i< 5;i++)
			for(j= 0;j< 6;j++)
				scanf("%d",&puzzle[i+1][j+1]);
		for(i = 0;i< 64;i++)
		{
			if(change(i))
			{
				printf("PUZZLE #%d\n",k);
				for(i = 1;i <= 5;i++)
				{
					for(j = 1;j <= 6;j++)
						printf("%d ",press[i][j]);
					printf("\n");
				}
				break;
			}
		}
	}
	return 0;
}
					

