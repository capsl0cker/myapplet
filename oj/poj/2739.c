#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int is_prime(int num)
{
    int i;
    for(i = 2;i <= sqrt(num);i++)
       if(num %i == 0 )
         return 0;
     return 1;
}

int main(void)
{
   int num,tmp;
   int i,j,count;
   freopen("in.txt","r",stdin);
   while(scanf("%d",&num) == 1 && num != 0)
   {
       count = 0;
       for(j = 2;j<= num;j++)
         if(is_prime(j))
         {
            tmp = num;
            for(i = j;i <= num;i++)
            {
                if(is_prime(i))
                {
                   tmp -= i;
                   if(tmp == 0)
                   {   count++;  break; }
                    else if(tmp < 0)
                       break;
                 }
             }
            } 
          printf("%d\n",count);
       } 
      return 0;
}
   

