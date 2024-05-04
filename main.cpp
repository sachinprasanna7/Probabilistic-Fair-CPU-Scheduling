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

struct Result {
    float avgWaitingTime;
    float avgTurnaroundTime;
    float avgResponseTime;
};

Result probabilisticFair(int processes[][3], int n, int time_quantum, int k = 2){

    int p[300], arrivalTime[300], burstTime[300], burstTimeFinal[300], completionTime[300], priority[300], responseTime[300];
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
    
    //cout<<"\nGantt Chart:\n\n";

    int processRunning = selectProcess(mpp, processes, processesInQueue, k);

    responseTime[processRunning] = ans;

    for(int j = 0; j < n;){

        //cout<<"Process running: "<<processRunning<<endl;

        //Gantt Chart
        //sprintf(ganttChart + jj, "%d---P%d---", ans, p[processRunning]);
        //cout<<ans<<"---P"<<p[processRunning]<<"---";
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

            //cout<<ans<<endl;

            ans = min;

            processRunning = selectProcess(mpp, processes, processesInQueue, k); 

            responseTime[processRunning] = ans; 
        }
    }

    //Gantt Chart
    //sprintf(ganttChart + jj, "---%d", ans);
    //cout<<ans<<endl;
    //printf("\n\nGantt Chart:\n %s\n", ganttChart);

    //cout<<"ans: "<<ans<<endl;


    // printf("\n\nProcess ID\t| Priority\t| Arrival Time\t| Burst Time\t| Completion Time\t| Turnaround Time\t| Waiting Time\t| Response Time\n");
    

    for (int i = 0; i < n; i++){
        int turnaaroundTime = completionTime[i] - arrivalTime[i];
        int waitingTime = turnaaroundTime - burstTimeFinal[i];
        responseTime[i] -= arrivalTime[i];
        avgTurnaaroundTime += turnaaroundTime;
        avgWaitingTime += waitingTime;
        avgResponseTime += responseTime[i];
        // printf("\n------------------------------------------------------------------------------------------------------------------------------------------------");
        // printf("\nP%d\t\t| %d\t\t| %d\t\t| %d\t\t| %d\t\t\t| %d\t\t\t| %d\t\t| %d\t\t\t", p[i], priority[i], arrivalTime[i], burstTimeFinal[i], completionTime[i], turnaaroundTime, waitingTime, responseTime[i]);
    }

    // printf("\n------------------------------------------------------------------------------------------------------------------------------------------------");
    // printf("\nAverage Turnaround Time: %.2f", avgTurnaaroundTime/n);
    // printf("\nAverage Waiting Time: %.2f", avgWaitingTime/n);
    // printf("\nAverage Response Time: %.2f\n", avgResponseTime/n);

    avgTurnaaroundTime = avgTurnaaroundTime/(n*1.0);
    avgWaitingTime = avgWaitingTime/(n*1.0);
    avgResponseTime = avgResponseTime/(n*1.0);

    Result res;

    res.avgTurnaroundTime = avgTurnaaroundTime;
    res.avgWaitingTime = avgWaitingTime;
    res.avgResponseTime = avgResponseTime;

    return res;

}



void createDirectory(const char *path) {
    #ifdef _WIN32
        mkdir(path);
    #else 
        mkdir(path, 0777);
    #endif
}
void printArray(float arr[], int n) {
    cout << fixed << setprecision(2);
    for (int i = 0; i < n; i++) {
        cout << arr[i] << ", ";
    }
    cout << endl;
}
float sum(float arr[], int n) {
    float sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

Result calculateResults(int arr[][3], int n, int time_quantum, int k, int timetoAverage) {

        float avgWaitingTime[timetoAverage];
        float avgTurnaroundTime[timetoAverage];
        float avgResponseTime[timetoAverage];
        for(int i = 0 ; i < timetoAverage ; i++){
            Result ans;
            ans = probabilisticFair(arr, 300, time_quantum, k);
            avgWaitingTime[i] = ans.avgWaitingTime;
            avgTurnaroundTime[i] = ans.avgTurnaroundTime;
            avgResponseTime[i] = ans.avgResponseTime;
        }

        printf("Average Waiting Time Array: ");
        printArray(avgWaitingTime, timetoAverage);
        printf("Average Turnaround Time Array: ");
        printArray(avgTurnaroundTime, timetoAverage);
        printf("Average Response Time Array: ");
        printArray(avgResponseTime, timetoAverage);

        printf("\nFinal Average Waiting Time: %.2f\n", sum(avgWaitingTime, timetoAverage)/(timetoAverage*1.0));
        printf("Final Average Turnaround Time: %.2f\n", sum(avgTurnaroundTime, timetoAverage)/(timetoAverage*1.0));
        printf("Final Average Response Time: %.2f\n", sum(avgResponseTime, timetoAverage)/(timetoAverage*1.0));

        Result res;
        res.avgWaitingTime = sum(avgWaitingTime, timetoAverage)/(timetoAverage*1.0);
        res.avgTurnaroundTime = sum(avgTurnaroundTime, timetoAverage)/(timetoAverage*1.0);
        res.avgResponseTime = sum(avgResponseTime, timetoAverage)/(timetoAverage*1.0);

        return res;
}


Result shorterTimeProcessesFirst(int catTime, int burstTimeLens, int priorityLens, int time_quantum, int timetoAverage, int k){

    int arr[300][3];

    srand(12345); 

    for(int i = 0 ; i < 100 ; i++){
        arr[i][0] = rand() % catTime;
        arr[i][1] =  (rand() % burstTimeLens) + 1;
        arr[i][2] =  (rand() % priorityLens) + 1;
    }

    for(int i = 100 ; i < 200 ; i++){
        arr[i][0] = (rand() % catTime) + catTime;
        arr[i][1] =  (rand() % burstTimeLens) + (1+burstTimeLens);
        arr[i][2] =  (rand() % priorityLens) + 1;
    }

    for(int i = 200 ; i < 300 ; i++){
        arr[i][0] =  (rand() % catTime) + (catTime*2);
        arr[i][1] =  (rand() % burstTimeLens) + (1+(burstTimeLens*2));
        arr[i][2] =  (rand() % priorityLens) + 1;
    }


    createDirectory("Generated Data");
    createDirectory("Generated Data/Arrival Times");
    createDirectory("Generated Data/Burst Times");
    createDirectory("Generated Data/Priority");

    ofstream arrivalFile("Generated Data/Arrival Times/arrivalTime_shorterCPUtimesfirst.txt");
    ofstream burstFile("Generated Data/Burst Times/burstTime_shorterCPUtimesfirst.txt");
    ofstream priorityFile("Generated Data/Priority/priority_shorterCPUtimesfirst.txt");

    if (arrivalFile.is_open() && burstFile.is_open() && priorityFile.is_open()) {
        for (int i = 0; i < 300; ++i) {
            arrivalFile << arr[i][0] << " ";
            burstFile << arr[i][1] << " ";
            priorityFile << ((3*burstTimeLens)-arr[i][2]) << " ";
        }
        arrivalFile.close();
        burstFile.close();
        priorityFile.close();
       // cout << "Array contents written to files" << endl;
    }

    else {
        cout << "Unable to open one or more files." << endl;
    }

    printf("\n\n****Shorter Time Processes First****\n");
    return calculateResults(arr, 300, time_quantum, k, timetoAverage);

}


Result mediumTimeProcessesFirst(int catTime, int burstTimeLens, int priorityLens, int time_quantum, int timetoAverage, int k){
    
        int arr[300][3];

        srand(12345); 

        for(int i = 0 ; i < 100 ; i++){
            arr[i][0] = rand() % catTime;
            arr[i][1] =  (rand() % burstTimeLens) + (1+burstTimeLens);
            arr[i][2] =  (rand() % priorityLens) + 1;
        }

        for(int i = 100 ; i < 200 ; i++){
            arr[i][0] = (rand() % catTime) + catTime;
            arr[i][1] =  (rand() % burstTimeLens) + (1+(burstTimeLens*2));
            arr[i][2] =  (rand() % priorityLens) + 1;
        }

        for(int i = 200 ; i < 300 ; i++){
            arr[i][0] =  (rand() % catTime) + (catTime*2);
            arr[i][1] =  (rand() % burstTimeLens) + 1;
            arr[i][2] =  (rand() % priorityLens) + 1;
        }


        createDirectory("Generated Data");
        createDirectory("Generated Data/Arrival Times");
        createDirectory("Generated Data/Burst Times");
        createDirectory("Generated Data/Priority");

        ofstream arrivalFile("Generated Data/Arrival Times/arrivalTime_mediumCPUtimesfirst.txt");
        ofstream burstFile("Generated Data/Burst Times/burstTime_mediumCPUtimesfirst.txt");
        ofstream priorityFile("Generated Data/Priority/priority_mediumCPUtimesfirst.txt");

        if (arrivalFile.is_open() && burstFile.is_open() && priorityFile.is_open()) {
            for (int i = 0; i < 300; ++i) {
                arrivalFile << arr[i][0] << " ";
                burstFile << arr[i][1] << " ";
                priorityFile << ((3*burstTimeLens)-arr[i][2]) << " ";
            }
            arrivalFile.close();
            burstFile.close();
            priorityFile.close();
           // cout << "Array contents written to files" << endl;
        }

        else {
            cout << "Unable to open one or more files." << endl;
        }

        printf("\n\n****Medium Time Processes First****\n");
       return calculateResults(arr, 300, time_quantum, k, timetoAverage);
    
}


Result longerTimeProcessesFirst(int catTime, int burstTimeLens, int priorityLens, int time_quantum, int timetoAverage, int k){
    
        int arr[300][3];

        srand(12345);
    
        for(int i = 0 ; i < 100 ; i++){
            arr[i][0] = rand() % catTime;
            arr[i][1] =  (rand() % burstTimeLens) + (1+(burstTimeLens*2));
            arr[i][2] =  (rand() % priorityLens) + 1;
        }

        for(int i = 100 ; i < 200 ; i++){
            arr[i][0] = (rand() % catTime) + catTime;
            arr[i][1] =  (rand() % burstTimeLens) + (1+burstTimeLens);
            arr[i][2] =  (rand() % priorityLens) + 1;
        }

        for(int i = 200 ; i < 300 ; i++){
            arr[i][0] =  (rand() % catTime) + (catTime*2);
            arr[i][1] =  (rand() % burstTimeLens) + 1;
            arr[i][2] =  (rand() % priorityLens) + 1;
        }
    
        createDirectory("Generated Data");
        createDirectory("Generated Data/Arrival Times");
        createDirectory("Generated Data/Burst Times");
        createDirectory("Generated Data/Priority");

        ofstream arrivalFile("Generated Data/Arrival Times/arrivalTime_longerCPUtimesfirst.txt");
        ofstream burstFile("Generated Data/Burst Times/burstTime_longerCPUtimesfirst.txt");
        ofstream priorityFile("Generated Data/Priority/priority_longerCPUtimesfirst.txt");

        if (arrivalFile.is_open() && burstFile.is_open() && priorityFile.is_open()) {
            for (int i = 0; i < 300; i++) {
                arrivalFile << arr[i][0] << " ";
                burstFile << arr[i][1] << " ";
                priorityFile << ((3*burstTimeLens)-arr[i][2]) << " ";
            }
            arrivalFile.close();
            burstFile.close();
            priorityFile.close();
            //cout << "Array contents written to files" << endl;
        }

        else {
            cout << "Unable to open one or more files." << endl;
        }

        printf("\n\n****Longer Time Processes First****\n");
       return calculateResults(arr, 300, time_quantum, k, timetoAverage);

}


Result lowerPriorityFirst(int catTime, int burstTimeLens, int priorityLens, int time_quantum, int timetoAverage, int k){
    
         int arr[300][3];

        srand(12345);
    
        for(int i = 0 ; i < 100 ; i++){
            arr[i][0] = rand() % catTime;
            arr[i][1] =  (rand() % burstTimeLens) + 1;
            arr[i][2] =  (rand() % priorityLens) + 1;
        }

        for(int i = 100 ; i < 200 ; i++){
            arr[i][0] = (rand() % catTime) + catTime;
            arr[i][1] =  (rand() % burstTimeLens) + (1+burstTimeLens);
            arr[i][2] =  (rand() % priorityLens) + (1+priorityLens);
        }

        for(int i = 200 ; i < 300 ; i++){
            arr[i][0] =  (rand() % catTime) + (catTime*2);
            arr[i][1] =  (rand() % burstTimeLens) + (1+(burstTimeLens*2));
            arr[i][2] =  (rand() % priorityLens) + (1+(priorityLens*2));
        }
    
        createDirectory("Generated Data");
        createDirectory("Generated Data/Arrival Times");
        createDirectory("Generated Data/Burst Times");
        createDirectory("Generated Data/Priority");

        ofstream arrivalFile("Generated Data/Arrival Times/arrivalTime_lowerpriorityfirst.txt");
        ofstream burstFile("Generated Data/Burst Times/burstTime_lowerpriorityfirst.txt");
        ofstream priorityFile("Generated Data/Priority/priority_lowerpriorityfirst.txt");

        if (arrivalFile.is_open() && burstFile.is_open() && priorityFile.is_open()) {
            for (int i = 0; i < 300; i++) {
                arrivalFile << arr[i][0] << " ";
                burstFile << arr[i][1] << " ";
                priorityFile << ((3*priorityLens)-arr[i][2])<< " ";
            }
            arrivalFile.close();
            burstFile.close();
            priorityFile.close();
            //cout << "Array contents written to files" << endl;
        }

        else {
            cout << "Unable to open one or more files." << endl;
        }

        printf("\n\n****Lower Priority Processes First****\n");
       return calculateResults(arr, 300, time_quantum, k, timetoAverage);

}


Result mediumPriorityFirst(int catTime, int burstTimeLens, int priorityLens, int time_quantum, int timetoAverage, int k){
    
        int arr[300][3];

        srand(12345);
    
        for(int i = 0 ; i < 100 ; i++){
            arr[i][0] = rand() % catTime;
            arr[i][1] =  (rand() % burstTimeLens) + 1;
            arr[i][2] =  (rand() % priorityLens) + (1+priorityLens);
        }

        for(int i = 100 ; i < 200 ; i++){
            arr[i][0] = (rand() % catTime) + catTime;
            arr[i][1] =  (rand() % burstTimeLens) + (1+burstTimeLens);
            arr[i][2] =  (rand() % priorityLens) + (1+(priorityLens*2));
        }

        for(int i = 200 ; i < 300 ; i++){
            arr[i][0] =  (rand() % catTime) + (catTime*2);
            arr[i][1] =  (rand() % burstTimeLens) + (1+(burstTimeLens*2));
            arr[i][2] =  (rand() % priorityLens) + 1;
        }
    
        createDirectory("Generated Data");
        createDirectory("Generated Data/Arrival Times");
        createDirectory("Generated Data/Burst Times");
        createDirectory("Generated Data/Priority");

        ofstream arrivalFile("Generated Data/Arrival Times/arrivalTime_mediumpriorityfirst.txt");
        ofstream burstFile("Generated Data/Burst Times/burstTime_mediumpriorityfirst.txt");
        ofstream priorityFile("Generated Data/Priority/priority_mediumpriorityfirst.txt");

        if (arrivalFile.is_open() && burstFile.is_open() && priorityFile.is_open()) {
            for (int i = 0; i < 300; i++) {
                arrivalFile << arr[i][0] << " ";
                burstFile << arr[i][1] << " ";
                priorityFile << ((3*priorityLens)-arr[i][2]) << " ";
            }
            arrivalFile.close();
            burstFile.close();
            priorityFile.close();
            //cout << "Array contents written to files" << endl;
        }

        else {
            cout << "Unable to open one or more files." << endl;
        }

        printf("\n\n****Medium Priority Processes First****\n");
        return calculateResults(arr, 300, time_quantum, k, timetoAverage);

}


Result higherPriorityFirst(int catTime, int burstTimeLens, int priorityLens, int time_quantum, int timetoAverage, int k){
    
        int arr[300][3];

        srand(12345);
    
        for(int i = 0 ; i < 100 ; i++){
            arr[i][0] = rand() % catTime;
            arr[i][1] =  (rand() % burstTimeLens) + 1;
            arr[i][2] =  (rand() % priorityLens) + (1+(priorityLens*2));
        }

        for(int i = 100 ; i < 200 ; i++){
            arr[i][0] = (rand() % catTime) + catTime;
            arr[i][1] =  (rand() % burstTimeLens) + (1+burstTimeLens);
            arr[i][2] =  (rand() % priorityLens) + (1+(priorityLens));
        }

        for(int i = 200 ; i < 300 ; i++){
            arr[i][0] =  (rand() % catTime) + (catTime*2);
            arr[i][1] =  (rand() % burstTimeLens) + (1+(burstTimeLens*2));
            arr[i][2] =  (rand() % priorityLens) + 1;
        }
    
        createDirectory("Generated Data");
        createDirectory("Generated Data/Arrival Times");
        createDirectory("Generated Data/Burst Times");
        createDirectory("Generated Data/Priority");

        ofstream arrivalFile("Generated Data/Arrival Times/arrivalTime_higherpriorityfirst.txt");
        ofstream burstFile("Generated Data/Burst Times/burstTime_higherpriorityfirst.txt");
        ofstream priorityFile("Generated Data/Priority/priority_higherpriorityfirst.txt");

        if (arrivalFile.is_open() && burstFile.is_open() && priorityFile.is_open()) {
            for (int i = 0; i < 300; i++) {
                arrivalFile << arr[i][0] << " ";
                burstFile << arr[i][1] << " ";
                priorityFile << ((3*priorityLens)-arr[i][2]) << " ";
            }
            arrivalFile.close();
            burstFile.close();
            priorityFile.close();
            //cout << "Array contents written to files" << endl;
        }

        else {
            cout << "Unable to open one or more files." << endl;
        }

        printf("\n\n****Higher Priority Processes First****\n");

        return calculateResults(arr, 300, time_quantum, k, timetoAverage);

}

Result generateRandomProcesses(int catTime, int burstTimeLens, int priorityLens, int time_quantum, int timetoAverage, int k){

    int arr[300][3];

    srand(12345);

    for(int i = 0 ; i < 300 ; i++){
        arr[i][0] = rand() % catTime;
        arr[i][1] =  (rand() % burstTimeLens) + 1;
        arr[i][2] =  (rand() % priorityLens) + 1;
    }

    createDirectory("Generated Data");
    createDirectory("Generated Data/Arrival Times");
    createDirectory("Generated Data/Burst Times");
    createDirectory("Generated Data/Priority");

    ofstream arrivalFile("Generated Data/Arrival Times/arrivalTime_random.txt");
    ofstream burstFile("Generated Data/Burst Times/burstTime_random.txt");
    ofstream priorityFile("Generated Data/Priority/priority_random.txt");

    if (arrivalFile.is_open() && burstFile.is_open() && priorityFile.is_open()) {
        for (int i = 0; i < 300; i++) {
            arrivalFile << arr[i][0] << " ";
            burstFile << arr[i][1] << " ";
            priorityFile << arr[i][2] << " ";
        }
        arrivalFile.close();
        burstFile.close();
        priorityFile.close();
        //cout << "Array contents written to files" << endl;
    }

    else {
        cout << "Unable to open one or more files." << endl;
    }

    printf("\n\n****Random Processes****\n");
    return calculateResults(arr, 300, time_quantum, k, timetoAverage);

}


int main() {

    cout<<"\nPROBABILISTIC FAIR CPU SCHEDULING ALGORITHM RESULT SIMULATOR:\n\n";
    
    int time_quantum = 6;
    int catTime = 1000;
    int burstTimeLens = 10;
    int priorityLens = 10;
    int timetoAverage = 3;
    int k = 2;

    //First 3 testing cases
    Result res1 = shorterTimeProcessesFirst(catTime, burstTimeLens, priorityLens, time_quantum, timetoAverage, k);
    Result res2 = mediumTimeProcessesFirst(catTime, burstTimeLens, priorityLens, time_quantum, timetoAverage, k);
    Result res3 = longerTimeProcessesFirst(catTime, burstTimeLens, priorityLens, time_quantum, timetoAverage, k);

    //Last 3 testing cases
    Result res4 = lowerPriorityFirst(catTime, burstTimeLens, priorityLens, time_quantum, timetoAverage, k);
    Result res5 = mediumPriorityFirst(catTime, burstTimeLens, priorityLens, time_quantum, timetoAverage, k);
    Result res6 = higherPriorityFirst(catTime, burstTimeLens, priorityLens, time_quantum, timetoAverage, k);

    //Random Cases
    Result res7 = generateRandomProcesses(catTime, burstTimeLens, priorityLens, time_quantum, timetoAverage, k);


    cout << "\n\n****FINAL RESULTS****\n\n";
    cout << "Case\tAvg Waiting Time\tAvg Turnaround Time\tAvg Response Time\n";

    cout << "1\t" << res1.avgWaitingTime << "\t\t\t" << res1.avgTurnaroundTime << "\t\t\t" << res1.avgResponseTime << endl;
    cout << "2\t" << res2.avgWaitingTime << "\t\t\t" << res2.avgTurnaroundTime << "\t\t\t" << res2.avgResponseTime << endl;
    cout << "3\t" << res3.avgWaitingTime << "\t\t\t" << res3.avgTurnaroundTime << "\t\t\t" << res3.avgResponseTime << endl;
    cout << "4\t" << res4.avgWaitingTime << "\t\t\t" << res4.avgTurnaroundTime << "\t\t\t" << res4.avgResponseTime << endl;
    cout << "5\t" << res5.avgWaitingTime << "\t\t\t" << res5.avgTurnaroundTime << "\t\t\t" << res5.avgResponseTime << endl;
    cout << "6\t" << res6.avgWaitingTime << "\t\t\t" << res6.avgTurnaroundTime << "\t\t\t" << res6.avgResponseTime << endl;
    cout << "7\t" << res7.avgWaitingTime << "\t\t\t" << res7.avgTurnaroundTime << "\t\t\t" << res7.avgResponseTime << endl;
    
    return 0;
}
