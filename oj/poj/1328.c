/*
 * poj-1328:Radar Installation
 * Greedy(贪心）
 * 求出对于每个Island的Radar区间，排序，转换为区间覆盖问题或是活动安排问题
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct _island{
	float left,right;
	int flag;
};

void Cal(float d,float x,float y,struct _island *l)
{
	float dist;
	dist = sqrt(d*d - y*y);
	l->left = x - dist;
	l->right = x + dist;
	l->flag = 0;
}

int cmp(const void *a,const void *b)
{
	struct _island *c,*d;
	c = (struct _island *)a;
	d = (struct _island *)b;
	if(c->left > d->left)
		return 1;
	else 
		return -1;
}


int main(void)
{
	int n;
	int i,flag;
	float x,y,d;
	float left,right;
	int count,num;
	struct _island *island = NULL;

	freopen("in.txt","r",stdin);
	num = 0;
	while(scanf("%d %f",&n,&d) ==2)
	{
		num++;
		if(n == 0 && d == 0)
			return 0;

		island = (struct _island *)malloc(sizeof(struct _island)*n);
		if(island == NULL)
		{
			printf("memory failure\n");
			return -1;
		}
		flag = 1;
		for(i = 0;i < n;i++)
		{
			scanf("%f %f",&x,&y);
			if(y < 0 || y > d)
				flag = 0;
			Cal(d,x,y,&island[i]);
		}
		qsort(island,n,sizeof(struct _island),cmp);
		count = 1;
		left = island[0].left;
		right = island[0].right;
		island[0].flag = 1;
		for(i = 1;i < n;i++)
		{
			if(island[i].left > right)
			{
				left = island[i].left;
				right = island[i].right;
				island[i].flag = 1;
				count++;
				continue;
			}
			if(island[i].flag == 0 )
			{
				if(island[i].left > left && island[i].left <= right)
					left = island[i].left;
				if(island[i].right < right)
					right = island[i].right;
				island[i].flag = 1;
			}
		}
		if(flag)
			printf("Case %d: %d\n",num,count);
		else
			printf("Case %d: -1\n",num);
		free(island);
	}
	return 0;
}
			



