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

    FCFS(myProcesses);

    myFile.close();
}

void FCFS(vector<process> processes){
    int clock = 0;
    int avgWaiting = 0;
    int totalSwitchingTime = 0;
    int cpuEfficiency;
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
    cpuEfficiency = clock / totalSwitchingTime;

    cout << "FCFS (non-preemptive)" << endl << endl;

    cout << "Total Time Required: " << clock << endl;
    cout << "Average waiting time: " << avgWaiting << endl;
    cout << "CPU Efficiency: " << cpuEfficiency << "%" << endl << endl;

    printResults(processes);
}