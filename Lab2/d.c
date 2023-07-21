#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Exact 2 files needed!");
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {

        close(pipefd[0]);

        FILE *file = fopen(argv[1], "r");
        if (file == NULL)
        {
            perror("fopen");
            return 1;
        }

        char ch;
        int non_alphabetic_count = 0;
        while ((ch = fgetc(file)) != EOF)
        {
            if (isupper(ch))
            {
                ch = tolower(ch);
            }

            else if (islower(ch))
            {
                ch = toupper(ch);
            }

            if(!isalpha(ch))
            {
                non_alphabetic_count++;
            }
            write(pipefd[1], &ch, 1);
        }
      /*  char c;
        int non_alphabetic_count = 0;
        while ((c = fgetc(file)) != EOF)
        {
            if (!isalpha(c))
            {
                non_alphabetic_count++;
            }
        }
        */

        printf("Total number of non alphabetic characters: %d\n", non_alphabetic_count);
        
    }
    else
    {

        close(pipefd[1]);

        FILE *file = fopen(argv[2], "w");
        if (file == NULL)
        {
            perror("fopen");
            return 1;
        }

        char c;
        while (read(pipefd[0], &c, 1) > 0)
        {
            fputc(c, file);
        }
        printf("Contents of %s copied to file %s with uppercase and lowercase letters reversed using a pipe",argv[1],argv[2]);
        fclose(file);
        close(pipefd[0]);
    }

    return 0;
}