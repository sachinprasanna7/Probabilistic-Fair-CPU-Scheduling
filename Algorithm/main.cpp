#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <climits>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/stat.h>
#include <iomanip>
using namespace std;


double calculateProbability(int priority, int totalPriority, int n, int k) {
    double probability = (double)(priority + k)/(totalPriority + (n*k));
    return probability;
}

int selectProcess(unordered_map <int,bool> &mpp, int processes[][3], int n, int k) {

    vector <pair <int, double>> probabilityVector;

    int totalPriority = 0;
    for (int i = 0; i < mpp.size(); i++) {
        if (mpp[i] == true) {
            totalPriority += processes[i][2];
        }
    }

    if (totalPriority == 0){
        return -1;
    }

    for(int j = 0 ; j < mpp.size() ; j++){
        if (mpp[j] == true) {
            probabilityVector.push_back(make_pair(j, calculateProbability(processes[j][2], totalPriority, n, k)));
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);

    double r = (double)rand() / RAND_MAX; // Random value between 0 and 1
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        sum += probabilityVector[i].second;
        if (r < sum) {
            return probabilityVector[i].first;
        }
    }

    return probabilityVector[n - 1].first; // Fallback option
}



void probabilisticFair(int processes[][3], int n, int time_quantum, int k = 2){

    int p[10], arrivalTime[10], burstTime[10], burstTimeFinal[10], completionTime[10], priority[10], responseTime[10];
    float avgWaitingTime = 0, avgTurnaaroundTime = 0, avgResponseTime = 0;

    int min = INT_MAX, ix = -1;

    unordered_map <int, bool> mpp;

    for(int i = 0; i < n; i++){
        p[i] = i;
        arrivalTime[i] = processes[i][0];
        burstTime[i] = processes[i][1];
        priority[i] = processes[i][2];
        burstTimeFinal[i] = burstTime[i];
        responseTime[i] = -1;
        mpp[p[i]] = false;

        if(min > arrivalTime[i]){
            min = arrivalTime[i];
            ix = i;
        }
    }

    int ans = min, ii = 0, jj = 0;
    int processesInQueue = 0;

    for(int i = 0 ; i < n ; i++){
        if(arrivalTime[i] == min){
            mpp[p[i]] = true;
            processesInQueue++;
        }
    }

    char ganttChart[10000];
    
    cout<<"\nGantt Chart:\n\n";

    int processRunning = selectProcess(mpp, processes, processesInQueue, k);

    responseTime[processRunning] = ans;

    for(int j = 0; j < n;){

        //cout<<"Process running: "<<processRunning<<endl;

        //Gantt Chart
        //sprintf(ganttChart + jj, "%d---P%d---", ans, p[processRunning]);
        cout<<ans<<"---P"<<p[processRunning]<<"---";
        int o = burstTime[processRunning];
        if (o > time_quantum){
            ans += time_quantum;
            burstTime[processRunning] -= time_quantum;
            for (int i = 0; i < n; i++){
                if (mpp[p[i]] == false && arrivalTime[i] <= ans && burstTime[i] > 0){
                    mpp[p[i]] = true;
                    processesInQueue++;
                }
            }
            jj += 9;
            
        }

        else{
            ans += o;
            burstTime[processRunning] -= o;
            for (int i = 0; i < n; i++){
                if (mpp[p[i]] == false && arrivalTime[i] <= ans && burstTime[i] > 0){
                    mpp[p[i]] = true;
                    processesInQueue++;
                }
            }
            completionTime[processRunning] = ans;
            mpp[p[processRunning]] = false;
            processesInQueue--;
            j++;
            jj += 9;
            // cout<<ans<<endl;
        }

        processRunning = selectProcess(mpp, processes, processesInQueue, k);

        if(responseTime[processRunning] == -1){
            responseTime[processRunning] = ans;
        }

        if(processRunning == -1 && j < n){
            //traverse through mpp and check if any process is left that arrives later than ans
            int min = INT_MAX, ix = -1;
            for(int i = 0 ; i < n ; i++){
                if(mpp[p[i]] == false && arrivalTime[i] < min && arrivalTime[i] > ans){
                    min = arrivalTime[i];
                    ix = i;
                }
            }

            for(int i = 0 ; i < n ; i++){
                if(arrivalTime[i] == min){
                    mpp[p[i]] = true;
                    processesInQueue++;
                }
            } 

            cout<<ans<<endl;

            ans = min;

            processRunning = selectProcess(mpp, processes, processesInQueue, k); 

            responseTime[processRunning] = ans; 
        }
    }

    //Gantt Chart
    //sprintf(ganttChart + jj, "---%d", ans);
    cout<<ans<<endl;
    //printf("\n\nGantt Chart:\n %s\n", ganttChart);

    //cout<<"ans: "<<ans<<endl;


    printf("\n\nProcess ID\t| Priority\t| Arrival Time\t| Burst Time\t| Completion Time\t| Turnaround Time\t| Waiting Time\t| Response Time\n");
    

    for (int i = 0; i < n; i++){
        int turnaaroundTime = completionTime[i] - arrivalTime[i];
        int waitingTime = turnaaroundTime - burstTimeFinal[i];
        responseTime[i] -= arrivalTime[i];
        avgTurnaaroundTime += turnaaroundTime;
        avgWaitingTime += waitingTime;
        avgResponseTime += responseTime[i];
        printf("\n------------------------------------------------------------------------------------------------------------------------------------------------");
        printf("\nP%d\t\t| %d\t\t| %d\t\t| %d\t\t| %d\t\t\t| %d\t\t\t| %d\t\t| %d\t\t\t", p[i], priority[i], arrivalTime[i], burstTimeFinal[i], completionTime[i], turnaaroundTime, waitingTime, responseTime[i]);
    }

    printf("\n------------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\nAverage Turnaround Time: %.2f", avgTurnaaroundTime/n);
    printf("\nAverage Waiting Time: %.2f", avgWaitingTime/n);
    printf("\nAverage Response Time: %.2f\n", avgResponseTime/n);

    avgTurnaaroundTime = avgTurnaaroundTime/(n*1.0);
    avgWaitingTime = avgWaitingTime/(n*1.0);
    avgResponseTime = avgResponseTime/(n*1.0);

}



int main() {

    cout<<"\nPROBABILISTIC FAIR CPU SCHEDULING ALGORITHM:\n\n";
    int processes[10][3] = {{0, 2, 3}, {1, 4, 1}, {2, 6, 4}, {3, 8, 2}, {4, 10, 5}, {5, 12, 3}, {6, 14, 2}, {7, 16, 1}, {8, 18, 3}, {9, 20, 2}};
    int n = 10;
    int time_quantum = 2;
    int k = 2;

    probabilisticFair(processes, n, time_quantum, k);

    return 0;
}
