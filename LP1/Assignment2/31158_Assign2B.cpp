// Roll No- 31158
// Ganesh Patil

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class APTABEntry
{
public:
    int index;
    string parameterName;

    APTABEntry(int idx, const string &paramName)
        : index(idx), parameterName(paramName) {}
};

class MNT
{
    int index;
    string macroName;
    int ppCount;
    int kpCount;
    int mdtPointer;
    int kpdtPointer;

public:
    MNT()
    {
        index = 0;
        macroName = "";
        ppCount = 0;
        kpCount = 0;
        mdtPointer = 1;
        kpdtPointer = 1;
    }
    friend class Generate;
};

class MDT
{
    int index;
    string opcode;
    string operand;

public:
    MDT(){};
    MDT(int i, string opc, string opr)
    {
        index = i;
        opcode = opc;
        operand = opr;
    }
    friend class Generate;
};

class KDPTAB
{
    string keyword;
    string defaultValue;

public:
    KDPTAB(){};
    KDPTAB(string key, string value)
    {
        keyword = key;
        defaultValue = value;
    }
    friend class Generate;
};

class PNTAB
{
    string paramName;

public:
    PNTAB(){};
    PNTAB(string pn)
    {
        paramName = pn;
    }
    friend class Generate;
};

class Generate
{
public:
    void inout();
};

void Generate ::inout()
{
    ifstream mntFile("mnt.txt");
    ifstream pntabFile("pntab.txt");
    ifstream kpdtabFile("kpdtab.txt");
    ifstream mdtFile("mdt.txt");
    ofstream aptabFile("aptab.txt");
    ofstream expandedCodeFile("expanded_code.txt");

    if (!mntFile.is_open() || !pntabFile.is_open() || !kpdtabFile.is_open() || !mdtFile.is_open() || !aptabFile.is_open() || !expandedCodeFile.is_open())
    {
        cerr << "Error opening one or more input/output files." << endl;
        return;
    }

    // Load data structures from files
    map<int, MNT> mnt;
    map<int, MDT> mdt;
    map<int, KDPTAB> kpdtabMap;
    map<string, vector<PNTAB>> pntab;

    // Load MNT
    string line;
    getline(mntFile, line); // Skip the header line
    while (getline(mntFile, line))
    {
        stringstream ss(line);
        int index, ppCount, kpCount, mdtPointer, kpdtPointer;
        string macroName;
        ss >> index >> macroName >> ppCount >> kpCount >> mdtPointer >> kpdtPointer;
        MNT mntEntry;
        mntEntry.index = index;
        mntEntry.macroName = macroName;
        mntEntry.ppCount = ppCount;
        mntEntry.kpCount = kpCount;
        mntEntry.mdtPointer = mdtPointer;
        mntEntry.kpdtPointer = kpdtPointer;
        mnt[index] = mntEntry;
    }

    // Load PNTAB
    getline(pntabFile, line);
    string currentMacro = "";
    while (getline(pntabFile, line))
    {
        //here is the change
        if (!line.empty())
        {
            if (line.length()>1)
            {
                currentMacro = line;
            }
            else
            {
                stringstream ss(line);
                string paramName;
                ss >> paramName;
                pntab[currentMacro].push_back(PNTAB(paramName));
            }
        }
    }

    // Load KPDTAB
    getline(kpdtabFile, line);
    while (getline(kpdtabFile, line))
    {
        stringstream ss(line);
        int index;
        string keyword, defaultValue;
        ss >> index >> keyword >> defaultValue;
        KDPTAB kpdtabEntry;
        kpdtabEntry.keyword = keyword;
        kpdtabEntry.defaultValue = defaultValue;
        kpdtabMap[index] = kpdtabEntry;
    }

    map<int, pair<string, string>> aptab;
    // call macro
    string Macrocall = "M2 100 200 &V=AREG &U=BREG";
    stringstream breakmacrocall(Macrocall);
    string macroname;
    breakmacrocall >> macroname;
    string arg;

    int mdtptr = 1;
    int index = -1;
    bool f = 0;
    for (auto i : mnt)
    {
        if (macroname == i.second.macroName)
        {
            index = i.first;
            f = 1;
            break;
        }
    }

    if (f)
    {
        for (auto entry : pntab)
        {
            cout << macroname << endl;
            if (entry.first == macroname)
            {
                int index = 1;
                for (auto i : entry.second)
                {
                    aptab[index].second = i.paramName;
                    for (auto j : kpdtabMap)
                    {
                        if (i.paramName == j.second.keyword)
                        {
                            aptab[index].second = j.second.defaultValue;
                            aptab[index].first = j.second.keyword;
                        }
                    }
                    index++;
                }
                break;
            }
        }

        int ind = 1;
        while (breakmacrocall >> arg)
        {
            if (arg[0] == '&')
            {
                string key = arg.substr(0, arg.find('='));
                string defaultValue = arg.substr(arg.find('=') + 1);
                key.erase(0, 1);
                cout << key << " " << defaultValue << endl;
                for (auto i : aptab)
                {
                    if (i.second.first == key)
                    {
                        aptab[i.first].second = defaultValue;
                    }
                }
            }
            else
            {
                aptab[ind++].second = arg;
            }
        }

        cout << "\nAPTAB" << endl;
        aptabFile << "\nAPTAB" << endl;
        for (auto i : aptab)
        {
            cout << i.first << " " << i.second.second << endl;
            aptabFile << i.first << " " << i.second.second << endl;
        }
        cout << endl;

        int op = mnt[index].mdtPointer;

        while(op--)
            getline(mdtFile, line);

        while (getline(mdtFile, line))
        {
            stringstream ss(line);
            int index;
            string opcode, operand1, operand2;
            ss >> index >> opcode >> operand1 >> operand2;
            if (operand1[1] == 'P')
            {
                int in = (operand1[3] - '0');
                operand1 = aptab[in].second;
            }
            if (operand2[1] == 'P')
            {
                int in = (operand2[3] - '0');
                operand2 = aptab[in].second;
            }
            expandedCodeFile << index << "\t" << opcode << "\t" << operand1 << "\t" << operand2 << endl;
        }

        cout << "APTAB and expanded code generated successfully." << endl;
    }
    else
    {
        cout << "Macro not found!!" << endl;
    }

    mntFile.close();
    pntabFile.close();
    kpdtabFile.close();
    mdtFile.close();
    aptabFile.close();
    expandedCodeFile.close();
}

int main()
{
    Generate obj;
    obj.inout();
    return 0;
}