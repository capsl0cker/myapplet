/*
 * poj-2109 Power of Cryptography
 * 浮点数的表示范围
 * 类型			长度	有效数字	绝对值范围
 * float		32		6~7			10^(-37)~10^38
 * double		64		15~16		10^(-307)~10^(308)
 * long double	128		18~19		10^(-4931)~10^4932
 * printf("%.0f\n",...可以AC，但是printf("%.0lf\n",...却是WA，不知道为什么
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
	double p,n;
	freopen("in.txt","r",stdin);
	while(scanf("%lf %lf",&n,&p) != EOF)
//		printf("%.0lf\n",pow(p,1.0/n));
		printf("%.0f\n",pow(p,1.0/n));
	return 0;
}

