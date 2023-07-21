#include<stdio.h>
#include<string.h>

void pr_params(char** args)
{
    char* arr = *(args);
    int i=0;
    int end_count=0;

    while(end_count!=3){
        printf("\n");
        printf("s%d is ",end_count+1);
        while(arr[i] != '0'){
            printf("%c",arr[i]);
            i++;
        }
        i++;
        end_count++;
    }
    
}

int main()
{
    char s1[100];
    char s2[100];
    char s3[100];

    printf("Enter string 1:");
    fgets(s1,100,stdin);

    printf("Enter string 2:");
    fgets(s2,100,stdin);

    printf("Enter string 3:");
    fgets(s3,100,stdin);

    char params[500];

    int i;
    for(i=0;i<strlen(s1);i++){
        params[i] = s1[i];
    }
    i++;
    params[i] = '0';
    i++;

    for(int j=0;j<strlen(s2);j++){
        params[i] = s2[j];
        i++;
    }
    params[i] = '0';
    i++;

    for(int j=0;j<strlen(s3);j++){
        params[i] = s3[j];
        i++;
    }
    params[i] = '0';
    i++;
    
    char* ptr = params;
    pr_params(&ptr);
    return 0;
} 