#include <stdio.h>
#include <limits.h>

int main()
{
    int processes, resources;
    printf("Enter the number of processes: ");
    scanf("%d", &processes);
    printf("Enter the number of resources: ");
    scanf("%d", &resources);

    int i,j,k;
    int allocation[processes][resources];
    printf("Enter allocation matrix:");
    for (int i = 0; i < processes; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            printf("For process %d ,resource %d: ", i, j);
            scanf("%d", &allocation[i][j]);
        }
    }


    printf("Enter max matrix:\n");
    int max[processes][resources];
    for (int i = 0; i < processes; i++)
    {
        for (int j = 0; j < resources; j++)
        {
            printf("For process %d ,resource %d: ", i, j);
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter instances for");
    int instance[resources];
    for (int i = 0; i < resources; i++)
    {
        printf(" resource %d ", i);
        scanf("%d", &instance[i]);
    }

    int finish[processes], ans[processes], index = 0;
    for (k = 0; k < processes; k++)
    {
        finish[k] = 0;
    }

    int need[processes][resources];

    for (i = 0; i < processes; i++)
    {
        for (j = 0; j < resources; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    int l = 0;

    for (k = 0; k < processes; k++)
    {
        for (i = 0; i < processes; i++)
        {
            if (finish[i] == 0)
            {
                int flag = 0;
                for (j = 0; j < resources; j++)
                {
                    if (need[i][j] > instance[j])
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                {
                    ans[index++] = i;
                    for (l = 0; l < resources; l++)
                    {
                        instance[l] += allocation[i][l];
                        finish[i] = 1;
                    }
                }
            }
        }
    }

    int flag = 1, p = -1;
    for (int i = 0; i < processes; i++)
    {
        if (finish[i] == 0)
        {
            flag = 0;
            printf("The following system is not safe\n");
            int max = INT_MIN, sum = 0;
            for (int j = i; j < processes; j++)
            {
                if (finish[j] == 0)
                {
                    sum = 0;
                    for (int k = 0; k < resources; k++)
                    {
                        sum += allocation[j][k];
                    }
                    if (sum > max)
                    {
                        max = sum;
                        p = j;
                    }
                }
            }
            printf("Smallest set of processes whose termination ends the deadlock: P%d\n", p);
            break;
        }
    }

    for (k = 0; k < processes - index - 1; k++)
    {
        for (i = 0; i < processes; i++)
        {
            if (finish[i] == 0 && i != p)
            {
                int flag = 0;
                for (j = 0; j < resources; j++)
                {
                    if (need[i][j] > instance[j])
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                {
                    ans[index++] = i;
                    for (l = 0; l < resources; l++)
                    {
                        instance[l] += allocation[i][l];
                        finish[i] = 1;
                    }
                }
            }
        }
    }

    printf("No deadlock ocuurs in given set of processes.");
    return 0;
}