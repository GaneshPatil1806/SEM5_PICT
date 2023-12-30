// Roll No-31158
// Ganesh Patil

#include <bits/stdc++.h>
using namespace std;

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

void Generate::inout()
{
    ofstream mntFile("mnt.txt");
    ofstream pntabFile("pntab.txt");
    ofstream kpdtabFile("kpdtab.txt");
    ofstream mdtFile("mdt.txt");
    ifstream fin;

    fin.open("MACRO.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening intermediate file." << endl;
        return;
    }

    // Storage declaration
    map<int, MNT> mnt;
    map<int, MDT> mdt;
    map<int, KDPTAB> kpdtab;
    map<string, vector<PNTAB>> pntab;

    bool inMacro = false;
    string macroName;
    string currentMacro;
    int macroIndex = 0;
    int mdtPtr = 1;
    int kpdtPtr = 1;
    string line;

    while (getline(fin, line))
    {
        string word;
        istringstream st(line);
        st >> word;

        if (word == "MACRO")
        {
            inMacro = true;
        }
        else if (inMacro && word != "MEND")
        {
            MNT mne;
            mne.kpdtPointer = kpdtPtr;
            mne.mdtPointer =mdtPtr;
            macroName = word;

            macroIndex++;
            int localpn = 0;
            int localkpdt = 0;

            currentMacro = macroName;

            while (st >> word)
            {
                if (word.find('=') != string::npos)
                {
                    string key = word.substr(0, word.find('='));
                    string defaultValue = word.substr(word.find('=') + 1);
                    key.erase(0, 1);
                    if (defaultValue.length() == 5)
                    {
                        defaultValue.pop_back();
                    }

                    KDPTAB kpe(key, defaultValue);
                    kpdtab[kpdtPtr] = kpe;
                    pntab[currentMacro].push_back(PNTAB(key));
                    kpdtPtr++;
                    localkpdt++;
                }
                else
                {
                    word.pop_back();
                    word.erase(0, 1);
                    PNTAB pne(word);
                    pntab[currentMacro].push_back(pne);
                    localpn++;
                }
            }

            mne.index = macroIndex;
            mne.macroName = macroName;
            mne.ppCount = localpn;
            mne.kpCount = localkpdt;
            mnt[macroIndex] = mne;
            inMacro = false;
        }
        else if (word == "MEND")
        {
            currentMacro = "";
            MDT mdte(mdtPtr, "\t" + word + "\t", "");
            mdt[mdtPtr] = mdte;
            mdtPtr++;
        }
        else
        {
            string opcode = word;
            string operand1, operand2;
            st >> operand1 >> operand2;

            if (operand1[0] == '&')
            {
                operand1.erase(0, 1);
            }

            if (operand2[0] == '&')
            {
                operand2.erase(0, 1);
            }

            // cout<<"Word "<<word<<endl;
            // bool found = false;
            // cout<<operand1<<" "<<operand2<<endl;
            // cout<<"mdtptr"<<mdtPtr<<endl;
            int i = 1;
            int i1 = -1, i2 = -1;

            if (pntab.find(currentMacro) != pntab.end())
            {
                const vector<PNTAB> &pnVector = pntab[currentMacro];

                for (const PNTAB &entry : pnVector)
                {
                    if (entry.paramName == operand1)
                    {
                        // cout<<"operand1" <<operand1<<endl;
                        i1 = i;
                    }
                    if (entry.paramName == operand2)
                    {
                        // cout<<"operand2" <<operand2<<endl;
                        i2 = i;
                    }
                    i++;
                }
            }

            if (i1 != -1)
            {
                if (i2 != -1)
                {
                    MDT mdte(mdtPtr, "\t" + word + "\t", "(P," + to_string(i1) + ") " + "(P," + to_string(i2) + ") ");
                    mdt[mdtPtr] = mdte;
                }
                else
                {
                    MDT mdte(mdtPtr, "\t" + word + "\t", "(P," + to_string(i1) + ") " + operand2);
                    mdt[mdtPtr] = mdte;
                }
                mdtPtr++;
            }
        }
    }

    // Output the data
    cout << "\nMNT" << endl;
    mntFile << "MNT" << endl;
    cout << "INDEX "
         << "MACRO_NAME "
         << "#PP "
         << "#KP "
         << "MDT_PTR "
         << "KPDT_PTR " << endl;

    for (const auto &entry : mnt)
    {
        cout << entry.second.index << "\t" << entry.second.macroName << "\t  " << entry.second.ppCount << "   " << entry.second.kpCount << "  " << entry.second.mdtPointer << "\t " << entry.second.kpdtPointer << endl;
        mntFile << entry.second.index << "\t" << entry.second.macroName << "\t  " << entry.second.ppCount << "   " << entry.second.kpCount << "  " << entry.second.mdtPointer << "\t " << entry.second.kpdtPointer << endl;
    }

    // Output PNTAB and KPDTAB for each macro
    cout << "\nPNTAB" << endl;
    pntabFile << "PNTAB" << endl;
    cout << "Positional Params" << endl;
    for (auto entry : pntab)
    {
        pntabFile << "\n"
                  << entry.first << endl;
        for (const auto &pnt : entry.second)
        {
            cout << pnt.paramName << endl;
            pntabFile << pnt.paramName << endl;
        }
    }

    cout << "\nKPDTAB" << endl;
    kpdtabFile << "KPDTAB" << endl;
    cout << "Index"
         << "\t"
         << "Key"
         << "\t"
         << "Default" << endl;
    for (const auto &entry : kpdtab)
    {
        cout << entry.first << "\t" << entry.second.keyword << "\t" << entry.second.defaultValue << endl;
        kpdtabFile << entry.first << "\t" << entry.second.keyword << "\t" << entry.second.defaultValue << endl;
    }

    // Output MDT for each macro

    cout << "\nMDT" << endl;
    mdtFile << "MDT" << endl;
    for (const auto &entry : mdt)
    {
        cout << entry.second.index << "\t" << entry.second.opcode << "\t" << entry.second.operand << endl;
        mdtFile << entry.second.index << "\t" << entry.second.opcode << "\t" << entry.second.operand << endl;
    }

    fin.close();
    mntFile.close();
    pntabFile.close();
    kpdtabFile.close();
    mdtFile.close();

    cout << "\n\nPass 1 output files generated successfully." << endl;
    // fout.close();
}

int main()
{
    Generate obj;
    obj.inout();
    return 0;
}