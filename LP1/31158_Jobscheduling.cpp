// Roll No-31158
// Ganesh Patil

#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

class Job
{
public:
    string name, id;
    float waiting_time, arrival_time, execution_time, completion_time, turn_around_time, burst_time, rembt, priority, flag;

    Job()
    {
        name = "";
        id = "";
        burst_time = 0;
        waiting_time = 0;
        arrival_time = 0;
        execution_time = 0;
        completion_time = 0;
        turn_around_time = 0;
        priority = 0;
        rembt = 0;
        flag = false;
    }
    friend class Job_scheduling;
    void display();
    void gantt_chart();
};

void Job::display()
{
    cout << name << "\t" << burst_time << "\t" << arrival_time << "\t" << completion_time << "\t" << turn_around_time << "\t" << waiting_time << "\t" << priority<<endl;
}

void Job::gantt_chart()
{
    cout<<name<<" ";
}

class Job_scheduling
{
    Job *rq;
    int np;
    float average_time, average_turn_around_time, average_waiting_time;

public:
    Job_scheduling(int n)
    {
        np = n;
        rq = new Job[np];
        average_time = 0;
        average_waiting_time = 0;
        average_turn_around_time = 0;
        bool f = 0;
        string ch;

        cout << "Do you want priority scheduling? 1.Yes 2.No" << endl;
        cin >> ch;
        if (ch == "1" || ch == "Yes" || ch == "y")
            f = 1;

        for (int i = 0; i < n; i++)
        {
            rq[i].id = (i + 1);

            // cout << "Enter Name of Job " << i + 1 << endl;
            cin >> rq[i].name;

            // cout << "Enter arrival time of Job " << i + 1 << endl;
            cin >> rq[i].arrival_time;

            // cout << "Enter execution time of Job " << i + 1 << endl;
            cin >> rq[i].burst_time;
            rq[i].rembt = rq[i].burst_time;

            if (f == 1)
            {
                // cout << "Enter priority of Job " << i + 1 << endl;
                cin >> rq[i].priority;
            }
        }
    }
    void FCFS();
    void SJF();
    void PS();
    void selectSort();
    void SJF_Preemptive();
    void roundrobin();
};

void Job_scheduling ::selectSort()
{
    for (int i = 0; i < np - 1; i++)
    {
        int shortest = i;
        for (int j = i + 1; j < np; j++)
        {
            if (rq[j].arrival_time < rq[shortest].arrival_time)
            {
                shortest = j;
            }
        }
        if (shortest != i)
        {
            Job u = rq[i];
            rq[i] = rq[shortest];
            rq[shortest] = u;
        }
    }
}

void Job_scheduling::FCFS()
{
    // Selection sort
    selectSort();

    float ct = 0, avg_wt = 0, avg_tat = 0, tt = 0;
    for (int i = 0; i < np; i++)
    {
        if (rq[i].arrival_time > ct)
        {
            ct = rq[i].arrival_time;
        }

        rq[i].completion_time = ct + rq[i].burst_time;
        rq[i].turn_around_time = (rq[i].completion_time - rq[i].arrival_time);
        rq[i].waiting_time = (rq[i].turn_around_time - rq[i].burst_time);

        if (rq[i].waiting_time < 0)
            rq[i].waiting_time = 0;

        ct = rq[i].completion_time;

        tt += rq[i].burst_time;
        avg_wt += rq[i].waiting_time;
        avg_tat += rq[i].turn_around_time;
    }

    cout << "Table: " << endl;
    cout << "Name\tBT\tAT\tCT\tTAT\tWT" << endl;
    cout << "==================================================" << endl;
    for (int i = 0; i < np; i++)
        rq[i].display();

    cout << "\nGantt Chart:" << endl;
    for (int i = 0; i < np; i++)
        rq[i].gantt_chart();

    cout << endl;

    average_waiting_time = avg_wt / np;
    average_turn_around_time = avg_tat / np;

    cout << "\nAverage waiting time " << average_waiting_time << endl;
    cout << "Average_turn_around " << average_turn_around_time << endl;

    cout << endl;
}

bool compareJobs(const Job &lhs, const Job &rhs)
{
    if (lhs.burst_time == rhs.burst_time)
    {
        if (lhs.arrival_time == rhs.burst_time)
            return lhs.id > rhs.id;
        else
            return lhs.arrival_time > rhs.arrival_time;
    }

    return lhs.burst_time > rhs.burst_time;
}

void Job_scheduling::SJF()
{
    int count = 0, shortest = 0;
    int time = 0;
    float avgWT = 0, avgTAT = 0;
    bool check = false;
    cout << "\nName\tBT\tAT\tCT\tTAT\tWT" << endl;
    cout << "==================================================" << endl;
    int min = INT_MAX; 
    while (count < np)
    {
        for (int i = 0; i < np; i++)
        {
            if (rq[i].arrival_time <= time && rq[i].rembt < min && rq[i].rembt > 0)
            {
                shortest = i;
                min = rq[i].burst_time;
                check = true;
            }
        }

        if (!check)
        {
            time++;
            continue;
        }

        rq[shortest].rembt--;
        min = rq[shortest].rembt;

        if (min == 0)
        {
            count++;
            time = rq[shortest].completion_time = time + 1;
            rq[shortest].turn_around_time = rq[shortest].completion_time - rq[shortest].arrival_time;
            rq[shortest].waiting_time = rq[shortest].turn_around_time - rq[shortest].burst_time;

            avgWT += rq[shortest].waiting_time;
            avgTAT += rq[shortest].turn_around_time;

            rq[shortest].display();
        }
        time++;
        
    }

    average_waiting_time = avgWT / np;
    average_turn_around_time = avgTAT / np;

    cout << "Average waiting time: " << average_waiting_time << endl;
    cout << "Average turn around time: " << average_turn_around_time << endl;
    cout << endl;
}

void Job_scheduling::SJF_Preemptive()
{
    int tm;
    cout << "Enter tm i.e. time quantum" << endl;
    cin >> tm;
    cout << "\nrq_id\tName\tBT\tAT\tCT\tTAT\tWT\t" << endl;

    selectSort();
    function<bool(const Job &, const Job &)> comp = compareJobs;
    priority_queue<Job, vector<Job>, decltype(comp)> pq(comp);

    int curr_pro = 0;
    float time = 0;

    Job *ans = new Job[np];
    int j = 0;

    while (curr_pro < np || !pq.empty())
    {
        if (!pq.empty())
        {
            Job a = pq.top();

            if (a.rembt == a.burst_time)
            {
                time = a.arrival_time;
            }

            time += tm;
            a.rembt -= tm;

            if (a.rembt <= 0) // rq completes within the time quantum
            {
                time += a.rembt; // Add negative rembt to the current time

                ans[j] = a;
                ans[j].completion_time = time;
                ans[j].turn_around_time = ans[j].completion_time - ans[j].arrival_time;
                ans[j].waiting_time = ans[j].turn_around_time - ans[j].burst_time;
                j++;

                pq.pop();
            }
            else
            {
                pq.pop();
                pq.push(a);
            }
        }
        else
        {
            time = rq[curr_pro].arrival_time;
        }

        while (curr_pro < np && rq[curr_pro].arrival_time <= time)
        {
            pq.push(rq[curr_pro]);
            rq[curr_pro].flag = 1;
            curr_pro++;
        }
    }

    cout << "Table: " << endl;
    cout << "rq_id\tName\tAT\tBT\tCT\tWT\tTAT" << endl;
    for (int i = 0; i < np; i++)
    {
        cout << "\t" << ans[i].id << "\t" << ans[i].name << "\t" << ans[i].arrival_time << "\t"
             << ans[i].burst_time << "\t" << ans[i].completion_time << "\t" << ans[i].waiting_time;
        cout << "\t" << ans[i].turn_around_time << endl;
    }

    float tt = 0, avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < np; i++)
    {
        tt += ans[i].burst_time;
        avg_wt += ans[i].waiting_time;
        avg_tat += ans[i].turn_around_time;
    }

    delete[] ans;

    average_time = tt / np;
    average_waiting_time = avg_wt / np;
    average_turn_around_time = avg_tat / np;

    cout << "\nAverage execution time " << average_time << endl;
    cout << "\nAverage waiting time " << average_waiting_time << endl;
    cout << "Average_turn_around " << average_turn_around_time << endl;
}

bool comparePrior(const Job &lhs, const Job &rhs)
{
    if (lhs.priority == rhs.priority)
    {
        if (lhs.arrival_time == rhs.burst_time)
            return lhs.id > rhs.id;
        else
            return lhs.arrival_time > rhs.arrival_time;
    }

    return lhs.priority > rhs.priority;
}

void Job_scheduling::PS()
{
    cout << "rq_id\tName\tBT\tAT\tCT\tTAT\tWT\t" << endl;

    selectSort();
    function<bool(const Job &, const Job &)> comp = comparePrior;
    priority_queue<Job, vector<Job>, decltype(comp)> pq(comp);

    int curr_pro = 0;
    float time = 0;

    Job *ans = new Job[np];
    int j = 0;

    while (curr_pro < np || !pq.empty())
    {
        if (!pq.empty())
        {
            Job a = pq.top();
            pq.pop();

            time += a.burst_time;
            ans[j] = a;
            ans[j].completion_time = time;
            ans[j].turn_around_time = ans[j].completion_time - ans[j].arrival_time;
            ans[j].waiting_time = ans[j].turn_around_time - ans[j].burst_time;
            j++;
        }
        else
        {
            time = rq[curr_pro].arrival_time;
        }

        while (curr_pro < np && rq[curr_pro].arrival_time <= time)
        {
            pq.push(rq[curr_pro]);
            rq[curr_pro].flag = 1;
            curr_pro++;
        }
    }

    cout << "Table: " << endl;
    cout << "rq_id\tName\tAT\tBT\tCT\tWT\tTAT\tPriority" << endl;
    for (int i = 0; i < np; i++)
    {
        cout << "\t" << ans[i].id << "\t" << ans[i].name << "\t" << ans[i].arrival_time << "\t"
             << ans[i].burst_time << "\t" << ans[i].completion_time << "\t" << ans[i].waiting_time;
        cout << "\t" << ans[i].turn_around_time << "\t" << ans[i].priority << endl;
    }

    float tt = 0, avg_wt = 0, avg_tat = 0;
    for (int i = 0; i < np; i++)
    {
        tt += ans[i].burst_time;
        avg_wt += ans[i].waiting_time;
        avg_tat += ans[i].turn_around_time;
    }

    delete[] ans;

    average_time = tt / np;
    average_waiting_time = avg_wt / np;
    average_turn_around_time = avg_tat / np;

    cout << "\nAverage execution time " << average_time << endl;
    cout << "\nAverage waiting time " << average_waiting_time << endl;
    cout << "Average_turn_around " << average_turn_around_time << endl;
}

void Job_scheduling::roundrobin()
{
    int p = 0;
    int i, ts, nextval, count = np; // I assume that 'np' is the number of rqes in the 'rq' array.
    nextval = rq[0].arrival_time;
    i = 0;
    cout << "\nEnter the Time Slice or Quantum: ";
    cin >> ts;
    queue<Job> q1;
    Job *ans = new Job[np];

    for (i = 0; i < count && rq[i].arrival_time <= nextval; i++)
    {
        q1.push(rq[i]);
    }

    while (!q1.empty())
    {
        Job m = q1.front();
        cout << m.name << " ";
        q1.pop();

        if (m.burst_time > ts)
        {
            m.burst_time -= ts;
            nextval += ts;
        }
        else
        {
            nextval += m.burst_time;
            m.burst_time = 0;
        }

        while (i < count && rq[i].arrival_time <= nextval)
        {
            q1.push(rq[i]);
            i++;
        }

        if (m.burst_time > 0)
        {
            q1.push(m);
        }
        else
        {
            // rq completed, set completion time for the current rq in 'rq' array.
            for (int j = 0; j < count; j++)
            {
                if (m.name == rq[j].name)
                {
                    rq[j].completion_time = nextval;
                    ans[p] = rq[j];
                    p++;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        ans[i].turn_around_time = ans[i].completion_time - ans[i].arrival_time;
        ans[i].waiting_time = ans[i].turn_around_time - ans[i].burst_time;
    }

    float avgwt = 0;
    float avgtat = 0;
    cout << endl;
    cout << "Table: " << endl;
    cout << "rq_id\tName\tAT\tBT\tCT\tWT\tTAT" << endl;
    cout << "Table:" << endl;

    cout << setw(7) << "rq_id" << setw(8) << "Name" << setw(8) << "AT" << setw(8) << "BT"
         << setw(8) << "CT" << setw(8) << "WT" << setw(8) << "TAT" << endl;
    for (int i = 0; i < count; i++)
    {
        cout << setw(7) << rq[i].id << setw(8) << rq[i].name << setw(8) << rq[i].arrival_time << setw(8)
             << rq[i].burst_time << setw(8) << rq[i].completion_time << setw(8) << rq[i].waiting_time;
        cout << setw(8) << rq[i].turn_around_time << endl;

        avgwt += rq[i].waiting_time;
        avgtat += rq[i].turn_around_time;
    }
}

int main()
{
    int n, ch;
    cout << "Enter the number of Jobs " << endl;
    cin >> n;

    Job_scheduling obj(n);
    cout << "\nEnter the number of opertion " << endl;
    cout << "1.FCFS" << endl;
    cout << "2.SJF" << endl;
    cout << "3.SJF_Preemptive" << endl;
    cout << "4.Priority_Scheduling" << endl;
    cout << "5.Round robin" << endl;

    cin >> ch;
    switch (ch)
    {
    case 1:
        obj.FCFS();
        break;
    case 2:
        obj.SJF();
        break;
    case 3:
        obj.SJF_Preemptive();
        break;
    case 4:
        obj.PS();
        break;
    case 5:
        obj.roundrobin();
        break;
    default:
        cout << "Invalid Input" << endl;
        break;
    }
    return 0;
}