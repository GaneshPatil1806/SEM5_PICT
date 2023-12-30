// Roll No-31158
// Ganesh Patil
#include <bits/stdc++.h>
using namespace std;

class CodeGen
{
public:
    void pass2();
};

void CodeGen::pass2()
{
    // Opening intermediate code file
    ifstream fin("intermidiate.txt");
    ifstream sin("symbol_table.txt");
    ifstream lin("literal_table.txt");

    if (!fin.is_open())
    {
        cerr << "Error opening intermediate file." << endl;
        return;
    }

    ofstream fout("machine_code.txt");

    string line, lc_str, opcode_type, opcode_value, operand1, operand2;
    vector<string> pool_table;
    map<string, pair<string, string>> symbol_table;
    map<string, pair<string, string>> littab;

    while (getline(sin, line))
    {
        stringstream symbole(line);
        string index, symbol, address;
        symbole >> index >> symbol >> address;
        symbol_table[index] = {symbol, address};
    }
    sin.close();

    string lite, address;
    int literal_index = 1;
    while (getline(lin, line))
    {
        stringstream literal(line);
        literal >> lite >> address;
        littab[to_string(literal_index)] = {lite, address};
        literal_index++;
    }
    lin.close();

    int lc = -1;
    getline(fin, line);
    fout<<"LC"<<" "<<"Opcode"<<" "<<"Op1"<<" "<<"Op2"<<endl;
    fout<<endl;

    while (getline(fin, line))
    {
        stringstream ss(line);
        string lcstring;

        if (ss.peek() != ' ')
        {
            ss >> lcstring;
        }

        ss >> opcode_type >> opcode_value;

        if (opcode_type == "(AD,")
        {
            if (opcode_value == "01)")
            {
                ss >> lc_str;
                operand2 = lc_str.substr(3, lc_str.length() - 4);
                // Now LC is set to the start
                lc = stoi(operand2);
            }
            else if (opcode_value == "02)")
            {
                fout << endl;
            }
            else if (opcode_value == "03)" || opcode_value == "04)")
            {
                ss >> operand2;
                fout<<endl;
            }
        }
        else if (opcode_type == "(IS,")
        {
            // cout<<"IS"<<endl;
            ss >> operand1 >> operand2;
           // cout<<lcstring<<" operand1 "<<operand1<<"operand2"<<operand2<<endl;
            if (opcode_value!="00)")
            {
                fout << lcstring << " ";
                string op2 = operand2.substr(1, 1);

                if (op2 == "S")
                {
                    fout << "(" << opcode_value << " ";
                    fout << operand1 << " ";
                    string index = operand2.substr(3, 1);
                    fout << symbol_table[index].second << endl;
                }
                else if (op2 == "L")
                {
                    fout << "(" << opcode_value << " ";
                    fout << operand1 << " ";
                    string index = (operand2.substr(3, 1));
                    fout << littab[index].second << endl;
                }
            }
            else if(opcode_value=="00)")
            {
                fout<<lcstring<<" "<<"(00)"<<" "<<"(0)"<<" "<<"000"<<endl;
            }
        }

        else if (opcode_type == "(DL,")
        {
            if(opcode_value=="01)")
            {
                ss >> operand1;
                fout << lcstring << " ";
                fout<<"(00)"<<" "<<"(0)"<<" ";
                string constant = operand1.substr(3, 1);
                fout << "(00"<<constant<<")" << endl;
            }
            else if(opcode_value=="02)")    
                fout<<endl;
        }
    }

    fin.close();
    fout.close();

    cout << "Machine code generated!." << endl;
}

int main()
{
    CodeGen codeGen;
    codeGen.pass2();
    return 0;
}
