#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	char c;
	struct node *lchild,*rchild;
}Btree;

void reconstructed(Btree **bt,char *preord,char *inord)
{
	int i,j;
	char tmp[26];
	char *p,*q;
	if(!preord || !inord)
		return ;
	if(!(*bt))
	{
		*bt = (Btree *)malloc(sizeof(Btree));
		if(!*bt)
			return ;
		(*bt)->c = *preord;
		(*bt)->lchild = (*bt)->rchild = NULL;
	}
	for(i = 0;*(inord + i) !='\0' && *(inord + i) != *preord;i++) ;
	*(inord + i) = '\0';
	p = preord + 1;
	for(j = 0;j < i;j++)
		tmp[j] = *p++;
	tmp[j] = '\0';
	if(*tmp !='\0' && *inord != '\0')
		reconstructed(&((*bt)->lchild),tmp,inord);
	if(*p != '\0' && *(inord + i + 1) != '\0')
		reconstructed(&((*bt)->rchild),p,inord + i + 1);
}

void postord(Btree *bt)
{
	if(bt)
	{
		postord(bt->lchild);
		postord(bt->rchild);
		printf("%c",bt->c);
	}
}

int main(void)
{
	char preord[27],inord[27];
	Btree *bt;
	freopen("in.txt","r",stdin);
	while(scanf("%s %s",preord,inord) != EOF)
	{
		bt = NULL;
		reconstructed(&bt,preord,inord);
		postord(bt);
		printf("\n");
	}
}



