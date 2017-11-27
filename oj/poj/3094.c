//2013.3.28

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char ch;
	int i,checksum;
	i = 0;
	checksum = 0;
	freopen("in.txt","r",stdin);
	while(1)
	{
		ch = getchar();
		i++;
		if(ch == '#')
			return 0;
		if(ch == '\n')
		{
			i = 0;
			printf("%d\n",checksum);
			checksum = 0;
		}
		if(ch !=' ')
			checksum += i*(ch - 'A' + 1);
	}
	return 0;
}
