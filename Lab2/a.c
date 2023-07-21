#include<stdio.h>
#include<string.h>

int main(int argc,char* argv[])
{
    if(argc == 1)
    {
        return 0;
    }
    else
    {
        for(int i=1;i<argc;i++)
        {
            if(argv[i][0] != '-')
            {
                printf("%s\t",argv[i]);
            }
        }
        printf("\n");
    }
}