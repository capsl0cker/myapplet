#include <stdio.h>
#define Len 4

int main(void)
{
	int state[Len][Len] = {0};
	int i,j,k,step = 0;
	char c;
	freopen("in.txt","r",stdin);

	for(i = 0;i < Len;i++){
		for(j = 0;j < Len;j++){
//			scanf("%c",&c);
//			if(c == '\n')
//				scanf("%c",&c);
			scanf("%1s",&c);
			if(c == '+'){
				state[i][j] ^= 1;
				for(k = 0;k < Len;k++){
					state[i][k] ^= 1;
					state[k][j] ^= 1;
				}
			}
		}
	}

	for(i = 0;i < Len;i++){
		for(j = 0;j < Len;j++){
			if(state[i][j])
				step++;
		}
	}
	printf("%d\n",step);

	for(i = 0;i < Len;i++){
		for(j = 0;j < Len;j++){
			if(state[i][j] )
				printf("%d %d\n",i + 1, j + 1);
		}
	}
}
