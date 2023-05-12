#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <climits>
#include <cstdio>
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

    //cout<<"Total priority: "<<totalPriority<<endl;

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

    //cout<<"Random value: "<<r<<endl;

    for(int i = 0 ; i < probabilityVector.size() ; i++){
        //cout<<probabilityVector[i].first<<" "<<probabilityVector[i].second<<endl;
    }

    for (int i = 0; i < n; i++) {
        sum += probabilityVector[i].second;
        if (r < sum) {
            return probabilityVector[i].first;
        }
    }

    return probabilityVector[n - 1].first; // Fallback option
}


void probabilisticFair(int processes[][3], int n, int time_quantum){

    int p[100], arrivalTime[100], burstTime[100], burstTimeFinal[100], completionTime[100], priority[100];
    float avgWaitingTime = 0, avgTurnaaroundTime = 0;

    int min = INT_MAX, ix = -1;

    unordered_map <int, bool> mpp;

    for(int i = 0; i < n; i++){
        p[i] = i;
        arrivalTime[i] = processes[i][0];
        burstTime[i] = processes[i][1];
        priority[i] = processes[i][2];
        burstTimeFinal[i] = burstTime[i];
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

    int processRunning = selectProcess(mpp, processes, processesInQueue, 2);

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
        }

        processRunning = selectProcess(mpp, processes, processesInQueue, 2);

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

            processRunning = selectProcess(mpp, processes, processesInQueue, 2);  
        }
    }

    //Gantt Chart
    //sprintf(ganttChart + jj, "---%d", ans);
    cout<<ans<<endl;
    //printf("\n\nGantt Chart:\n %s\n", ganttChart);

    //cout<<"ans: "<<ans<<endl;


    printf("\n\nProcess ID\t| Priority\t| Arrival Time\t| Burst Time\t| Completion Time\t| Turnaround Time\t| Waiting Time");
    

    for (int i = 0; i < n; i++){
        int turnaaroundTime = completionTime[i] - arrivalTime[i];
        int waitingTime = turnaaroundTime - burstTimeFinal[i];
        avgTurnaaroundTime += turnaaroundTime;
        avgWaitingTime += waitingTime;
        printf("\n--------------------------------------------------------------------------------------------------------------------------------");
        printf("\nP%d\t\t| %d\t\t| %d\t\t| %d\t\t| %d\t\t\t| %d\t\t\t| %d", p[i], priority[i], arrivalTime[i], burstTimeFinal[i], completionTime[i], turnaaroundTime, waitingTime);
    }

    printf("\n--------------------------------------------------------------------------------------------------------------------------------");
    printf("\nAverage Turnaround Time: %.2f", avgTurnaaroundTime/n);
    printf("\nAverage Waiting Time: %.2f", avgWaitingTime/n);

}

int main(){

    int arr[][3] = {{0, 5, 3}, {1, 3, 1}, {2, 6, 4}, {3, 4, 2}};
    int n = sizeof(arr)/sizeof(arr[0]);
    int time_quantum = 2;

    // for(int i = 0 ; i < 10 ; i++){
    //     probabilisticFair(arr, n, time_quantum);
    //     cout<<endl;
    // }

    probabilisticFair(arr, n, time_quantum);

}