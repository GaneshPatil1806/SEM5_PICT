#include <bits/stdc++.h>
using namespace std;

class process
{
    int pno, at, bt, ct, tat, btt, wt, priority;

public:
    process(int pn, int a, int b)
    {
        pno = pn;
        at = a;
        bt = btt = b;
        ct = tat = wt = priority = -1;
    }

    bool operator<(const process &other) const
    {
        if (this->priority == -1)
        {
            if (btt == other.btt)
                if (at == other.at)
                    return pno > other.pno;
            at > other.at;

            return btt > other.btt;
        }
        else
        {
            return priority < other.priority;
        }
    }
    friend class Scheduling;
};

class Scheduling
{
    vector<process> table;
    int time_quantum;

public:
    void get_input(vector<process> arr, int tq)
    {
        table = arr;

        sort(table.begin(), table.end(), [](process &a, process &b)
             { return a.at < b.at; });

        if (tq == -1)
        {
            time_quantum = INT_MAX;
        }
        else
        {
            time_quantum = tq;
        }
    }

    void display()
    {
        cout << "\nJob \tArrival time\tBust time\tCompletion time\tTurnAround time\tWaiting time";
        for (auto it : table)
        {
            cout << "\n"
                 << it.pno << "\t\t" << it.at << "\t\t" << it.bt << "\t\t" << it.ct << "\t\t" << it.tat << "\t\t" << it.wt;
        }
    }

    void fcfs_p()
    {
        table[0].ct = table[0].at + table[0].bt;
        for (int i = 1; i < table.size(); i++)
        {
            table[i].ct = max(table[i - 1].ct, table[i].at) + table[i].bt;
        }
    }

    void turnaround()
    {
        for (int i = 0; i < table.size(); i++)
        {
            table[i].tat = table[i].ct - table[i].at;
        }
    }

    void wait()
    {

        for (int i = 0; i < table.size(); i++)
        {
            table[i].wt = table[i].tat - table[i].bt;
        }
    }

    void sjf()
    {

        int current = 0;
        priority_queue<process> q;
        vector<process> temp;

        while (!q.empty() || !table.empty())
        {
            while (!table.empty() && table.front().at <= current)
            {
                q.push(table.front());
                table.erase(table.begin());
            }

            if (!q.empty())
            {
                process k = q.top();
                q.pop();

                int remainingTime = k.btt;
                current += remainingTime;
                k.btt = 0;
                k.ct = current;
                temp.push_back(k);
            }
            else
            {
                current++;
            }
        }
        table = temp;
    }

    void sjfp()
    {
        vector<process> temp;
        priority_queue<process> q;
        int current = 0;

        while (!table.empty() || !q.empty())
        {
            while (!table.empty() && table.front().at <= current)
            {
                q.push(table.front());
                table.erase(table.begin());
            }

            if (!q.empty())
            {
                process k = q.top();
                q.pop();
                k.btt--;

                if (k.btt > 0)
                {                   
                    q.push(k);
                }
                else
                {
                    k.ct = current;
                    temp.push_back(k);
                }
            }
            else
            {
                current++;
            }
        }
        table = temp;
    }

    void rr(int tq)
    {
        vector<process> temp;
        vector<process> rq;
        int current = 0;

        while (!table.empty() || !rq.empty())
        {
            while (!table.empty() && table.front().at <= current)
            {
                rq.push_back(table.front());
                table.erase(table.begin());
            }

            if (!rq.empty())
            {
                process k = rq.front();
                rq.erase(rq.begin());

                int deduct = min(k.btt, tq);
                k.btt -= deduct;
                current += deduct;

                if (k.btt > 0)
                {
                    while (!table.empty() && table.front().at <= current)
                    {
                        rq.push_back(table.front());
                        table.erase(table.begin());
                    }
                    rq.push_back(k);
                }
                else
                {
                    k.ct = current;
                    temp.push_back(k);
                }
            }
            else
            {
                current++;
            }
        }
        table = temp;
    }

    void priority_()
    {
        for (int i = 0; i < table.size(); i++)
        {
            cout << "Enter the priority for: " << table[i].pno;
            cin >> table[i].priority;
        }

        int current = 0;
        vector<process> temp;

        while (!table.empty())
        {
            int maxPrior = INT_MIN;
            int ind = -1;

            for (int i = 0; i < table.size(); i++)
            {
                if (current >= table[i].at && table[i].priority > maxPrior)
                {
                    maxPrior = table[i].priority;
                    ind = i;
                }
            }

            if (ind == -1)
            {
                current++;
            }
            else
            {
                current += table[ind].bt;
                table[ind].btt = 0;
                table[ind].ct = current;
                temp.push_back(table[ind]);
                table.erase(table.begin() + ind);
            }
        }
        table = temp;
    }
};

int main()
{
    int count;

    vector<process> arr;
    cout << "How many process: ";
    cin >> count;
    for (int i = 0; i < count; i++)
    {
        int pno, a, b;
        cout << "\nProcess No: ";
        cin >> pno;
        cout << "\nArrival Time : ";
        cin >> a;
        cout << "\nBust Time : ";
        cin >> b;
        process p(pno, a, b);
        arr.push_back(p);
        cout << "\n------------------";
    }

    int ch;
    cout << "\n1.FCFS\n2.SJF(Primitive)";
    cin >> ch;
    if (ch == 1)
    {
        Scheduling fcfs;
        fcfs.get_input(arr, -1);
        fcfs.fcfs_p();
        fcfs.turnaround();
        fcfs.wait();
        fcfs.display();
    }
    else if (ch == 2)
    {
        Scheduling fcfs;
        fcfs.get_input(arr, -1);
        fcfs.sjf();
        fcfs.turnaround();
        fcfs.wait();
        fcfs.display();
    }
    if (ch == 3)
    {
        Scheduling rr;
        int c;
        cout << "\nTime Quantum : ";
        cin >> c;
        rr.get_input(arr, -1);
        rr.rr(c);
        rr.turnaround();
        rr.wait();
        rr.display();
    }
    else if (ch == 4)
    {
        Scheduling priority;
        priority.get_input(arr, -1);
        priority.priority_();
        priority.turnaround();
        priority.wait();
        priority.display();
    }

    return 0;
}