#include <iostream>
using namespace std;

int main(){
  
  return 0;
}

void RR(int processes[][2], int n, int time_quantum)
{
    int p[100], arrivalTime[100], burstTime[100], burstTimeFinal[100], completionTime[100], turnaaroundTime[100], waitingTime[100], vis[100] = {0}, i, j, temp = 0, pr[100], m, bf[10000];
    float avgWaitingTime = 0, avgTurnaaroundTime = 0;

    int min = 100000000, ix = -1;
    for (i = 0; i < n; i++)
    {
        p[i] = i + 1;
        arrivalTime[i] = processes[i][0];
        burstTime[i] = processes[i][1];
        burstTimeFinal[i] = burstTime[i];
        if (min > arrivalTime[i])
        {
            min = arrivalTime[i];
            ix = i;
        }
    }

    int ans = 0, ii = 0, jj = 0;
    bf[jj++] = ix;
    vis[ix] = 1;
    for (j = 0; j < n;)
    {
        printf("%d ", bf[ii] + 1);
        int o = burstTime[bf[ii]];
        if (o > time_quantum)
        {
            ans += time_quantum;
            burstTime[bf[ii]] -= time_quantum;
            for (i = 0; i < n; i++)
            {
                if (vis[i] == 0 && arrivalTime[i] <= ans)
                {
                    bf[jj++] = i;
                    vis[i] = 1;
                }
            }
            bf[jj++] = bf[ii];
        }
        else
        {
            ans += o;
            burstTime[bf[ii]] -= o;
            for (i = 0; i < n; i++)
            {
                if (vis[i] == 0 && arrivalTime[i] <= ans)
                {
                    bf[jj++] = i;
                    vis[i] = 1;
                }
            }
            completionTime[bf[ii]] = ans;
            j++;
        }

        ii++;
    }

    printf("\n\nProcess\t| Arrival Time\t| Burst Time\t| Completion Time\t| Turnaround Time\t| Waiting Time");
    for (i = 0; i < n; i++)
    {
        turnaaroundTime[i] = completionTime[i] - arrivalTime[i];
        waitingTime[i] = turnaaroundTime[i] - burstTimeFinal[i];
        avgTurnaaroundTime += turnaaroundTime[i];
        avgWaitingTime += waitingTime[i];
    }

    avgTurnaaroundTime = avgTurnaaroundTime / n;
    avgWaitingTime = avgWaitingTime / n;

    for (i = 0; i < n; i++)
    {
        printf("\n------------------------------------------------------------------------------------------------------");
        printf("\nP%d\t| %d\t\t| %d\t\t| %d\t\t\t| %d\t\t\t| %d", p[i], arrivalTime[i], burstTimeFinal[i], completionTime[i], turnaaroundTime[i], waitingTime[i]);
    }
    printf("\n------------------------------------------------------------------------------------------------------");
    printf("\nAverage Turnaround Time: %.2f", avgTurnaaroundTime);
    printf("\nAverage Waiting Time: %.2f", avgWaitingTime);
}
