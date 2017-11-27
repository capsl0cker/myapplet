#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char preord[27],inord[27],postord[27];

void print(int a,int b,int c,int len)
{
	int i;
	if(len <= 0) 
		return ;
	postord[c+len-1] = preord[a];
	if(len == 1)
		return ;
	for(i = b;i < b+len;i++)
		if(inord[i] == preord[a])
			break;
	print(a+1,b,c,i-b);
	print(a-b+i+1,i+1,c-b+i,len-i+b-1);
}

int main(void)
{
	int i,len;
	freopen("in.txt","r",stdin);
	while(scanf("%s %s",preord,inord) != EOF)
	{
		len = strlen(preord);
		print(0,0,0,len);
		for(i = 0;i < len;i++)
			printf("%c",postord[i]);
		printf("\n");
	}
	return 0;
}
		
