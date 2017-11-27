#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int is_Prime(int num)
{
	int i;
	if(num == 1)
		return 0;
	for(i = 2;i <= sqrt(num);i++)
		if(num % i == 0)
			return 0;
	return 1;
}

int main(void)
{
	int a,d,n;
	int i,count;
	freopen("in.txt","r",stdin);
	while(1)
	{
		scanf("%d %d %d",&a,&d,&n);
		if(a == 0 && d == 0&& n ==0)
			return 0;
		count = 0;
		for(i = a;;i += d)
		{
			if(count == n)
			{
				printf("%d\n",i-d);
				break;
			}
			if(is_Prime(i))
				count++;
		}
	}
}

