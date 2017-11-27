#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  int case_num,max,count;
  int i,j;
  int start,end,temp;
  int corridor[201];
  freopen("in.txt","r",stdin);
  scanf("%d",&case_num);
  for(i = 0;i < case_num;i++)
  {
      memset((void *)corridor,0,201 *sizeof(int));
      scanf("%d",&count);
      max = 0;
      while(count > 0)
      {
          scanf("%d %d",&start,&end);
          if(start > end)
          {
              temp = start;
              start = end;
              end = temp;
          }
          for(j = (start + 1) /2;j <= (end + 1) /2;j++)
                   corridor[j]++;
      count--;
      }
      max = 0;
      for(j = 1;j <= 200;j++)
         if(corridor[j] > max)
             max = corridor[j];
      printf("%d\n",10*max);
   }
   return 0;
}
