#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a,const void *b)
{
    int *c,*d;
    c = (int *)a;
    d = (int *)b;
    return (*c - *d);
}

int main(void)
{
    int encry[26];
    int plate[26];
    char ch;
    int i;
    freopen("in.txt","r",stdin);
    for(i = 0;i < 26;i++)
       encry[i] = plate[i] = 0;
    while((ch = getchar()) != '\n')
        encry[ch - 'A']++;
    while((ch = getchar()) != '\n')
        plate[ch - 'A']++;
    qsort(encry,26,sizeof(int),cmp);
    qsort(plate,26,sizeof(int),cmp);
    for(i = 0;i < 26;i++)
       if(encry[i] != plate[i] )
       {
           printf("NO\n");
           return 0;
       }
     printf("YES\n");
     return 0;
}

     
        


