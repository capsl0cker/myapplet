#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int is_Prime(int num)
{
	int i;
	for(i = 3;i <= sqrt(num);i++)
		if(num % i == 0)
			return 0;
	return 1;
}

int main(void)
{
	int num;
	int i,flag;
	freopen("in.txt","r",stdin);
	while(scanf("%d",&num) == 1 && num != 0)
	{
		flag = 0;
		for(i = 3;i <= num >>1;i+=2)
			if( is_Prime(i) && is_Prime(num - i))
			{
				flag = 1;
				break;
			}
		if(flag )
			printf("%d = %d + %d\n",num,i,num-i);
		else
			printf("Goldbach's conjecture is wrong.\n");
	}
	return 0;
}





