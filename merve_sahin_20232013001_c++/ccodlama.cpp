#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Süreç yapısı
struct Process
{
    string pid;
    int arrival;
    int burst;
    int priority;

    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    bool completed = false;
};

struct Gantt
{
    int start;
    int end;
    string pid; // process id veya "IDLE"
};

int main()
{
    ifstream file("case1.csv");

    if (!file.is_open())
    {
        cout << "case1.csv dosyasi acilamadi!" << endl;
        return 1;
    }

    vector<Process> processes;
    string line;

    // Başlık satırını atla
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string pid, arrival, burst, priorityStr;

        getline(ss, pid, ',');
        getline(ss, arrival, ',');
        getline(ss, burst, ',');
        getline(ss, priorityStr, ',');

        Process p;
        p.pid = pid;
        p.arrival = stoi(arrival);
        p.burst = stoi(burst);

        if (priorityStr == "high")
            p.priority = 1;
        else if (priorityStr == "normal")
            p.priority = 2;
        else
            p.priority = 3;

        processes.push_back(p);
    }

    file.close();
    // FCFS ALGORITMASI

    int current_time = 0;
    vector<Gantt> gantt;

    for (auto &p : processes)
    {

        if (current_time < p.arrival)
        {
            gantt.push_back({current_time, p.arrival, "IDLE"});
            current_time = p.arrival;
        }

        p.start_time = current_time;
        p.finish_time = p.start_time + p.burst;
        p.waiting_time = p.start_time - p.arrival;
        p.turnaround_time = p.finish_time - p.arrival;
        gantt.push_back({p.start_time, p.finish_time, p.pid});
        current_time = p.finish_time;
    }

    cout << "Toplam okunan surec sayisi: " << processes.size() << endl;
    cout << "\nFCFS - Ilk 5 Surec Sonuclari:\n";
    for (int i = 0; i < 5; i++)
    {
        cout << processes[i].pid
             << " | Arrival: " << processes[i].arrival
             << " | Start: " << processes[i].start_time
             << " | Finish: " << processes[i].finish_time
             << " | Waiting: " << processes[i].waiting_time
             << " | Turnaround: " << processes[i].turnaround_time
             << endl;
    }

    cout << "\nFCFS Zaman Tablosu (Ilk 10 Kayit):\n";

    for (int i = 0; i < 10 && i < gantt.size(); i++)
    {
        cout << "[ " << gantt[i].start << " ] -- "
             << gantt[i].pid << " -- [ "
             << gantt[i].end << " ]" << endl;
    }

    int context_switch = 0;

    for (int i = 1; i < gantt.size(); i++)
    {
        if (gantt[i].pid != gantt[i - 1].pid)
        {
            context_switch++;
        }
    }

    cout << "\nFCFS Toplam Context Switch Sayisi: "
         << context_switch << endl;

    double cpu_work_time = 0;

    for (auto &p : processes)
    {
        cpu_work_time += p.burst;
    }

    double total_time = gantt.back().end;
    double context_switch_cost = context_switch * 0.001;

    double cpu_efficiency =
        cpu_work_time / (total_time + context_switch_cost);

    cout << "\nFCFS CPU Verimliligi: "
         << cpu_efficiency << endl;

    int T_values[] = {50, 100, 150, 200};

    cout << "\nFCFS Throughput Sonuclari:\n";

    for (int T : T_values)
    {
        int completed = 0;

        for (auto &p : processes)
        {
            if (p.finish_time <= T)
            {
                completed++;
            }
        }

        cout << "T = " << T
             << " icin tamamlanan is sayisi: "
             << completed << endl;
    }

    // SJF

    vector<Gantt> gantt_sjf;
    int current_time_sjf = 0;
    int completed_count = 0;
    int n = processes.size();
    int completed_sjf = 0;
    // SJF icin completed bayraklarini sifirla
    for (auto &p : processes)
    {
        p.completed = false;
    }

    while (completed_sjf < n)
    {

        int idx = -1;
        int min_burst = 1000000;

        for (int i = 0; i < n; i++)
        {
            if (!processes[i].completed &&
                processes[i].arrival <= current_time_sjf &&
                processes[i].burst < min_burst)
            {

                min_burst = processes[i].burst;
                idx = i;
            }
        }

        if (idx == -1)
        {
            gantt_sjf.push_back({current_time_sjf, current_time_sjf + 1, "IDLE"});
            current_time_sjf++;
            continue;
        }

        processes[idx].start_time = current_time_sjf;
        processes[idx].finish_time =
            processes[idx].start_time + processes[idx].burst;

        gantt_sjf.push_back({processes[idx].start_time,
                             processes[idx].finish_time,
                             processes[idx].pid});

        current_time_sjf = processes[idx].finish_time;
        processes[idx].completed = true;
        completed_sjf++;
    }
    cout << "\nSJF - Ilk 5 Surec Sonuclari:\n";
    for (int i = 0; i < 5; i++)
    {
        cout << processes[i].pid
             << " | Arrival: " << processes[i].arrival
             << " | Start: " << processes[i].start_time
             << " | Finish: " << processes[i].finish_time
             << endl;
    }
    cout << "\nSJF Zaman Tablosu (Ilk 10 Kayit):\n";
    for (int i = 0; i < 10 && i < gantt_sjf.size(); i++)
    {
        cout << "[ " << gantt_sjf[i].start << " ] -- "
             << gantt_sjf[i].pid << " -- [ "
             << gantt_sjf[i].end << " ]\n";
    }
    cout << "\nSJF Throughput Sonuclari:\n";

    for (int T : {50, 100, 150, 200})
    {
        int count = 0;

        for (auto &g : gantt_sjf)
        {
            if (g.pid != "IDLE" && g.end <= T)
            {
                count++;
            }
        }

        cout << "T = " << T
             << " icin tamamlanan is sayisi: "
             << count << endl;
    }

    return 0;
}
