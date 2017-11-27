/*
 * poj2109:Power of Cryptography
 * 采用高精度+二分法
 * 	-高精度算法时,采用字符串按正常顺序存放，从后往前运算，
 * 	-对进位的处理，先加，再单独处理进位
 * 	-对前导空白的处理
 * 	-因为采用字符串便于比较等优点，要时刻注意保证仍然为字符串
 * 通过对数的处理获取底数的大体范围
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LEN 110


char *Mul(char *a,char *b,char *res)
{
	int value;
	int tmp[LEN];
	int i,j,k,start;
	int carry;
	start = LEN - 1;
	memset(tmp,0,sizeof(tmp));
	for(i = strlen(b) - 1;i >= 0;i--)
	{
		k = start--;
		value = b[i] - '0';
		for(j = strlen(a) - 1;j >= 0;j--)
			tmp[k--] += (value * (a[j] - '0'));
	}
	carry = 0;
	for(i = LEN - 1;i >= 0;i--)
	{
		tmp[i] += carry;
		carry = tmp[i] / 10;
		tmp[i] = tmp[i] % 10;
		res[i] = tmp[i] + '0';
	}
	i = 0;
	while(i < LEN && res[i] == '0')
		i++;
	if(i < LEN)
		return &res[i];
}

char *Power(char *base,int exp,char *res)
{
	char *p;
	if(exp == 1)
	{
		memcpy(res,base,sizeof(base));
		p = res;
	}
	else if(exp == 2)
		p = Mul(base,base,res);
	else
	{
		p = Mul(base,base,res);
		exp -= 2;
		while(exp-- > 0)
			p = Mul(p,base,res);
	}
	return p;
}

int Cmp(char *a, char *b)
{
	if(strlen(a) == strlen(b))
		return strcmp(a,b);
	else
		return (strlen(a) - strlen(b));
}

int main(void)
{
	int n,flag;
	int len,i,min,max,mid;
	char str[LEN + 1],tmp[LEN + 1],res[LEN + 1];
	char *p;
	freopen("in.txt","r",stdin);
	while(scanf("%d %s",&n,str) == 2)
	{
		len = strlen(str);
		for(i = 1,min = 1;i < (int)ceil((double)len / n);i++)
			min *= 10;
		for(i = 1,max = 9;i < (int)ceil((double)len / n);i++)
		{
			max *= 10;
			max += 9;
		}

		flag = 0;
		while(min <= max)
		{
			mid = (min + max)/2;
			sprintf(tmp,"%d",mid);
			/*promise res is an string, preparing for Cmp()*/
			res[LEN] = '\0';
			p = Power(tmp,n,res);
			if(Cmp(p,str) == 0)
			{
				flag = 1;
				printf("%d\n",mid);
				break;
			}
			else if(Cmp(p,str) > 0)
				max = mid -1;
			else
				min = mid + 1;
		}
		/*if can't find equal(min >= max),then print the biggest one larger than 'string'*/
		if(flag == 0)
			printf("%d\n",min-1);
	}
	return 0;
}


	

