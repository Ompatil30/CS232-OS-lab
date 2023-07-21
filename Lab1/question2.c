#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pr_params(int nargs,char **args)
{
    char *arr = *(args);
    int i = 0;
    int end_count = 0;

    while (end_count != nargs)
    {
        printf("\n");
        printf("s%d is ", end_count + 1);
        while (arr[i] != '0')
        {
            printf("%c", arr[i]);
            i++;
        }
        i++;
        end_count++;
    }
}

int main()
{
    int n;
    printf("Enter the number of strings you want to enter:");
    scanf("%d", &n);

    char *params;
    params = (char *)malloc(500 * sizeof(char));
    int k = 0;
    char s[100];
    for (int i = 0; i < n; i++)
    {
        printf("Enter string %d:", i + 1);
        scanf("%s",s);
        for (int j = 0; j < strlen(s); j++)
        {
            params[k] = s[j];
            k++;
        }
        params[k] = '0';
        k++;
    }
    pr_params(n,&params);
}