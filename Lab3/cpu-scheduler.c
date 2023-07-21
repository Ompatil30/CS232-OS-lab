#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct process
{
    char *name;
    int arrival;
    int cpuburst;
    int turnaround;
    int wait;
    int complete;
    int scheduled;
};

int n = 0;
struct process arr[100];

void clearProcessTable()
{
    int i = 0;
    for (i = 0; i < n; i++)
    {
        arr[i].arrival = 0;
        arr[i].cpuburst = 0;
        arr[i].name = 0;
        arr[i].turnaround = 0;
        arr[i].wait = 0;
        arr[i].complete = 0;
        arr[i].scheduled = 0;
    }
}

// Function to read process and store them into struct process
void ReadProcessTable(char *filename)
{
    n = 0;
    FILE *ptr = fopen(filename, "r");
    if (ptr == NULL)
        printf("Couldn't open file");
    char line[100];
    int i = 0;
    while (fgets(line, sizeof(line), ptr) != NULL)
    {

        char *token = strtok(line, " ");

        i = 0;
        while (token != NULL)
        {
            if (i == 0)
            {
                arr[n].name = malloc(2 * sizeof(char));
                strcpy(arr[n].name, token);
            }
            if (i == 1)
            {
                arr[n].arrival = atoi(token);
            }
            if (i == 2)
            {
                arr[n].cpuburst = atoi(token);
            }
            i++;
            token = strtok(NULL, " ");
        }
        n++;
    }

    //schdeuled = 1 denotes process is yet to be schdeuled
    //Initially all processess are not schdeuled
    for (int i = 0; i < n; i++)
    {
        arr[i].scheduled = 1;
    }
}

void FCFS(struct process arr[])
{
    for (int i = 0; i < n; i++)
    {
        int min = arr[i].arrival;
        for (int j = i + 1; j < n; j++)
        {

            if (arr[j].arrival < min)
            {
                struct process p = arr[j];
                arr[j] = arr[i];
                arr[i] = p;
            }
        }
    }
    int ct = 0;
    int k = 0;
    int index = 0;

    while (k < n)
    {
        if (arr[index].arrival <= ct)
        {
            printf("%d-%d scheduling process %s\n", ct, ct + arr[index].cpuburst, arr[index].name);
            ct = ct + arr[index].cpuburst;
            arr[index].complete = ct;
            k++;
            index++;
        }
        else
            ct++; // if process has not yet arrived,increment the current time
    }

    for (int i = 0; i < n; i++)
    {
        printf("turnaround time of %s is %d\n", arr[i].name, arr[i].complete - arr[i].arrival);
        arr[i].turnaround = arr[i].complete - arr[i].arrival;
    }
    for (int i = 0; i < n; i++)
    {
        printf("wait time of %s is %d\n", arr[i].name, arr[i].turnaround - arr[i].cpuburst);
        arr[i].wait = arr[i].turnaround - arr[i].cpuburst;
    }
    float avg_tat = 0, avg_wt = 0;
    for (int i = 0; i < n; i++)
    {
        avg_tat = avg_tat + arr[i].turnaround;
    }
    printf("avg turnaround %f\n", (float)avg_tat / n);
    for (int i = 0; i < n; i++)
    {
        avg_wt = avg_wt + arr[i].wait;
    }
    printf("avg wait %f\n", (float)avg_wt / n);
}

void SRTF(struct process arr[])
{
    // printf("hii");
    int ct = 0;
    int k = 0;
    int bt[n];
    for (int i = 0; i < n; i++)
    {
        bt[i] = arr[i].cpuburst;
    }
    while (k < n)
    {
        // printf("k is %d\n",k);
        int min = 100;
        int index = 0;
        int f = 0;
        for (int i = 0; i < n; i++)
        {
            if (arr[i].cpuburst < min && arr[i].scheduled == 1 && arr[i].arrival <= ct)
            {
                min = arr[i].cpuburst;
                index = i;
                f = 1;
            }
        }
        if (f == 1)
        {
            printf("%d-%d scheduling %s\n", ct, ct + 1, arr[index].name);
            ct = ct + 1;
            arr[index].cpuburst--;
            if (arr[index].cpuburst == 0)
            {
                arr[index].scheduled = 0;
                k++;
                arr[index].complete = ct;
            }
        }
        else
            ct++;
    }
    for (int i = 0; i < n; i++)
    {
        arr[i].cpuburst = bt[i];
    }
    for (int i = 0; i < n; i++)
    {
        printf("turnaround time of %s is %d\n", arr[i].name, arr[i].complete - arr[i].arrival);
        arr[i].turnaround = arr[i].complete - arr[i].arrival;
    }
    for (int i = 0; i < n; i++)
    {
        printf("wait time of %s is %d\n", arr[i].name, arr[i].turnaround - arr[i].cpuburst);
        arr[i].wait = arr[i].turnaround - arr[i].cpuburst;
    }
    float avg_tat = 0, avg_wt = 0;
    for (int i = 0; i < n; i++)
    {
        avg_tat = avg_tat + arr[i].turnaround;
    }
    printf("avg turnaround %f\n", (float)avg_tat / n);
    for (int i = 0; i < n; i++)
    {
        avg_wt = avg_wt + arr[i].wait;
    }
    printf("avg wait %f\n", (float)avg_wt / n);
}

void RR(int tq, struct process arr[])
{
    for (int i = 0; i < n; i++)
    {
        int min = arr[i].arrival;
        for (int j = i + 1; j < n; j++)
        {

            if (arr[j].arrival < min)
            {
                struct process p = arr[j];
                arr[j] = arr[i];
                arr[i] = p;
            }
        }
    }
    int bt[n];
    for (int i = 0; i < n; i++)
    {
        bt[i] = arr[i].cpuburst;
    }
    int ct = 0;
    int k = 0;
    int index = 0;
    while (k < n)
    {
        if (arr[index].arrival <= ct && arr[index].scheduled == 1)
        {
            // printf("case1\n");

            if (arr[index].cpuburst > tq)
            {
                printf("%d-%d scheduling %s\n", ct, ct + tq, arr[index].name);
                ct = ct + tq;
                arr[index].cpuburst = arr[index].cpuburst - tq;
            }
            else
            {
                printf("%d-%d scheduling %s\n", ct, ct + arr[index].cpuburst, arr[index].name);
                ct = ct + arr[index].cpuburst;
                arr[index].cpuburst = 0;
                arr[index].scheduled = 0;
                k++;
                arr[index].complete = ct;
            }
            index = (index + 1) % n;
        }
        else if (arr[index].scheduled == 0 && arr[index].arrival <= ct)
        {
            // printf("case2\n");
            index = (index + 1) % n;
        }
        else if (arr[index].arrival > ct)
        {
            // printf("case3\n");
            int ret = index;
            index = 0;
            int p = 0;
            while (p == 0)
            {
                if (arr[index].arrival <= ct && arr[index].scheduled == 1)
                {
                    if (arr[index].cpuburst > tq)
                    {
                        printf("%d-%d scheduling %s\n", ct, ct + tq, arr[index].name);
                        ct = ct + tq;
                        arr[index].cpuburst = arr[index].cpuburst - tq;
                        p = 1;
                        // break;
                    }
                    else
                    {
                        printf("%d-%d scheduling %s\n", ct, ct + arr[index].cpuburst, arr[index].name);
                        ct = ct + arr[index].cpuburst;
                        arr[index].cpuburst = 0;
                        arr[index].scheduled = 0;
                        k++;
                        arr[index].complete = ct;
                        p = 1;
                        // break;
                        // index = (index + 1) % n;
                    }
                }
                else if (arr[index].scheduled == 0 && arr[index].arrival <= ct)
                {
                    index = (index + 1) % n;
                }
                else
                    ct++;
            }
            if (ret == index)
                index = ret + 1;
            else
                index = ret;
        }
        else
        {
            ct++;
            // case 4
        }
    }
    for (int i = 0; i < n; i++)
    {
        arr[i].cpuburst = bt[i];
    }
    for (int i = 0; i < n; i++)
    {
        printf("turnaround time of %s is %d\n", arr[i].name, arr[i].complete - arr[i].arrival);
        arr[i].turnaround = arr[i].complete - arr[i].arrival;
    }
    for (int i = 0; i < n; i++)
    {
        printf("wait time of %s is %d\n", arr[i].name, arr[i].turnaround - arr[i].cpuburst);
        arr[i].wait = arr[i].turnaround - arr[i].cpuburst;
    }
    float avg_tat = 0, avg_wt = 0;
    for (int i = 0; i < n; i++)
    {
        avg_tat = avg_tat + arr[i].turnaround;
    }
    printf("avg turnaround %f\n", (float)avg_tat / n);
    for (int i = 0; i < n; i++)
    {
        avg_wt = avg_wt + arr[i].wait;
    }
    printf("avg wait %f\n", (float)avg_wt / n);
}

int main(int argc, char* argv[])
{
   
    int a = 0;
    while (a != 4)
    {
        printf("-----------------------------\n");
        printf("CPU Schdeuling Simulation\n");
        printf("-----------------------------\n");
        printf("Choose any one of the following:\n");
        printf("1.First Come First Served (FCFS)\n");
        printf("2.Round Robin (RR)\n");
        printf("3.SRBF\n");
        printf("4.Exit\n");
        scanf("%d", &a);
        switch (a)
        {
        case 1:
        {
            ReadProcessTable(argv[1]);
            printf("-------------------------------\n");
            printf("First Come First Served(FCFS)\n");
            printf("-------------------------------\n");
            FCFS(arr);
            break;
        }

        case 2:
        {
            ReadProcessTable(argv[1]);
            int tq;
            printf("-----------------------\n");
            printf("Round Robin(RR)\n");
            printf("------------------------\n");
            printf("enter time quantum ");
            scanf("%d", &tq);
            RR(tq, arr);
            break;
        }

        case 3:
        {
            ReadProcessTable(argv[1]);
            printf("-------\n");
            printf("SRBF\n");
            printf("--------\n");
            SRTF(arr);
            break;
        }

        case 4:
        {
            break;
        }
        }
    }
    return 0;
}
