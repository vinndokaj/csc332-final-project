#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;

const int SWITCH_TIME = 5;

struct process {
    int id;
    int arrival_time;
    int tat;
    int start_time;
    int completion_time;
    int cpu_time;

    bool operator==(const process &other) const {
        return id == other.id;
    }   
};

struct fcfsCompare
{
    bool operator()(const process& lhs, const process& rhs)
    {
        return lhs.arrival_time > rhs.arrival_time;
    }
};

void printResults(vector<process>&);
void getResults();
void FCFS(vector<process>);
void RR(vector<process>, int);
double calculateEfficiency(int, int);

int main() {
    getResults();
    return 0;
}

void printResults(vector<process>& p){
    for(int i=0; i < p.size(); i++){
        cout << "Process " << p[i].id << ":" << endl;
        cout << "Service time = " << p[i].cpu_time << endl;
        cout << "Turnaround time = " << p[i].tat << endl;
    }
    cout << endl;
}

void getResults() {
    ifstream myFile;
    myFile.open("input_SchedulingAlgorithms.txt");

    vector<process> myProcesses;

    int num_proccesses;

    myFile >> num_proccesses;

    while(!myFile.eof()){
        process p;
        myFile >> p.id;
        myFile >> p.arrival_time;
        myFile >> p.cpu_time;
        myProcesses.push_back(p);
    }

    cout << "****** Final Project Scheduling Task 2 ******" << endl << endl;
    FCFS(myProcesses);
    RR(myProcesses, 10);
    RR(myProcesses, 50);
    RR(myProcesses, 100);

    myFile.close();
}

double calculateEfficiency(int totalExecutionTime, int totalSwitchingTime) {
    return ((double)totalExecutionTime) / (totalExecutionTime + totalSwitchingTime) * 100;
}

void FCFS(vector<process> processes){
    int clock = 0;
    int totalSwitchingTime = 0;
    double avgWaiting = 0;
    double cpuEfficiency;
    priority_queue<process, vector<process>, fcfsCompare> fcfsQueue;
    
    for(int i=0; i < processes.size(); i++){
        fcfsQueue.push(processes[i]);
    }

    while(!fcfsQueue.empty()) {
        int target_index = fcfsQueue.top().id - 1;

        //should we be adding switch time in the first iteration?
        clock += SWITCH_TIME;
        totalSwitchingTime += SWITCH_TIME;

        //update clock based on service time
        processes[target_index].start_time = clock;
        clock += processes[target_index].cpu_time;

        //save completion time of a process and calculate tat (= completion - arrival)
        processes[target_index].completion_time = clock;
        processes[target_index].tat = clock - processes[target_index].arrival_time;

        avgWaiting += processes[target_index].tat - processes[target_index].cpu_time;

        fcfsQueue.pop();
    }

    avgWaiting = avgWaiting / processes.size();
    cpuEfficiency = calculateEfficiency(clock, totalSwitchingTime);//((double)clock) / (clock + totalSwitchingTime) * 100;

    cout << "FCFS (non-preemptive)" << endl << endl;

    cout << "Total Time Required: " << clock << endl;
    cout << "Average waiting time: " << avgWaiting << endl;
    cout << "CPU Efficiency: " << cpuEfficiency << "%" << endl << endl;

    printResults(processes);
}

void RR(vector<process> processes, int quantum){
    int clock = 0;
    int totalSwitchingTime = 0;
    double avgWaiting = 0;
    double cpuEfficiency;

    queue<process> rr;
    

    for(int i=0; i < processes.size(); i++){
        //in arrival time order due to nature of txt file
        rr.push(processes[i]);
    }

    while(!rr.empty()){
        process p = rr.front();
        rr.pop();
        if(p.cpu_time < quantum){
            clock += p.cpu_time;
            processes[p.id-1].completion_time = clock;
            processes[p.id-1].tat = clock - processes[p.id-1].arrival_time;
            avgWaiting += processes[p.id-1].tat - processes[p.id-1].cpu_time;
        } else {
            clock += quantum;
            p.cpu_time -= quantum;
            rr.push(p);
        }
        clock += SWITCH_TIME;
        totalSwitchingTime += SWITCH_TIME;
    }

    avgWaiting = avgWaiting / processes.size();
    cpuEfficiency = calculateEfficiency(clock, totalSwitchingTime);

    cout << "Round Robin (preemptive) Q=" << quantum << endl << endl;

    cout << "Total Time Required: " << clock << endl;
    cout << "Average waiting time: " << avgWaiting << endl;
    cout << "CPU Efficiency: " << cpuEfficiency << "%" << endl << endl;

    printResults(processes);
}
