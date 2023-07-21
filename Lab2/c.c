#include<stdio.h>
#include<ctype.h>

int main()
{
    int ch;
    printf("Enter characters. Once you finish printing, press enter to see the output. Then press Ctrl+Z(in Windows) or Ctrl+D(in Linux) to signify EOF else enter another set of characters and repeat the process:");
    printf("\n");
    while((ch=getchar())!=EOF)
    {
        if(islower(ch))
        {
            ch = toupper(ch);
            putchar(ch);
        }

        else if(isupper(ch))
        {
            ch = tolower(ch);
            putchar(ch);
        }
        
        else
        {
            putchar(ch);
        }
    }
    return 0;
}