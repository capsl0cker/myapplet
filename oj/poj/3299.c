#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _round(in) ((in) > 0? ((int)(((in) * 10) + 0.5) / 10.0):((int)(((in) *10) - 0.5) / 10.0))

int main(void)
{
int i;
double temperature,dewpoint,humindex;
double h,e;
char c;
freopen("in.txt","r",stdin);
while(1)
{
temperature = dewpoint = humindex = 200;
for(i = 0;i < 2;i++)
{
scanf("%c",&c);
if(c == 'E')
return 0;
else if(c == 'T')
scanf("%lf",&temperature);
else if(c == 'D')
scanf("%lf",&dewpoint);
else
scanf("%lf",&humindex);
getchar();
}
if(humindex == 200)
humindex = temperature + 0.5555*(6.11*exp(5417.7530 *((1 /273.16) - (1/(dewpoint + 273.16)))) - 10.0);
else if(temperature == 200)
temperature = humindex -(0.5555*(6.11*exp(5417.7530*((1/273.16) - (1/(dewpoint+273.16)))) -10.0));
else
{
dewpoint = 1 /(1 / 273.16 - log( ((humindex-temperature)/0.5555 + 10.0) / 6.11) / 5417.7530) - 273.16;
}
printf("T %.1f D %.1f H %.1f\n",_round(temperature),_round(dewpoint),_round(humindex));

}
return 0;
}

