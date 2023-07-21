#include<stdio.h>

int main()
{
    int ch;
    int count = 0;
    printf("Enter characters. Once you finish printing, press enter to see the output. Then press Ctrl+Z(in Windows) or Ctrl+D(in Linux) to signify EOF else enter another set of characters and repeat the process:");
    printf("\n");
    while((ch=getchar())!=EOF)
    {
        if(((ch>=65)&&(ch<=90)) || ((ch>=97)&&(ch<=122)))
        {
            continue;
        }
        else{
            putchar(ch);
            count++;
        }
    }
    printf("Total non-alphabetic characters:%d",count-1); //one is subtracted to exclude the space that is eneterd by user while hitting EOF
    return 0;
}