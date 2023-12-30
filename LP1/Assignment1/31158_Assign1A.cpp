// Roll No-31158
// Ganesh Patil
#include <bits/stdc++.h>
using namespace std;

class mnemonic
{
    map<string, pair<string, string>> Opcode;

public:
    mnemonic()
    {
        Opcode["STOP"] = {"IS", "00"};
        Opcode["ADD"] = {"IS", "01"};
        Opcode["SUB"] = {"IS", "02"};
        Opcode["MULT"] = {"IS", "03"};
        Opcode["MOVER"] = {"IS", "04"};
        Opcode["MOVEM"] = {"IS", "05"};
        Opcode["COMP"] = {"IS", "06"};
        Opcode["BC"] = {"IS", "07"};
        Opcode["DIV"] = {"IS", "08"};
        Opcode["READ"] = {"IS", "09"};
        Opcode["PRINT"] = {"IS", "10"};
        Opcode["START"] = {"AD", "01"};
        Opcode["END"] = {"AD", "02"};
        Opcode["ORIGIN"] = {"AD", "03"};
        Opcode["EQU"] = {"AD", "04"};
        Opcode["LTORG"] = {"AD", "05"};
        Opcode["DC"] = {"DL", "01"};
        Opcode["DS"] = {"DL", "02"};
    }
    friend class CodeGen;
};

class register_
{
    map<string, pair<string, string>> Register;

public:
    register_()
    {
        Register["AREG"] = {"1", ""};
        Register["BREG"] = {"2", ""};
        Register["CREG"] = {"3", ""};
        Register["DREG"] = {"4", ""};
    }
    friend class CodeGen;
};

class condition
{
    map<string, pair<string, string>> Condition;

    condition()
    {
        Condition["LT"] = {"1", ""};
        Condition["LE"] = {"2", ""};
        Condition["EQ"] = {"3", ""};
        Condition["GT"] = {"4", ""};
        Condition["GE"] = {"5", ""};
        Condition["ANY"] = {"6", ""};
    }
    friend class CodeGen;
};

class CodeGen
{
public:
    void read();
};

void CodeGen::read()
{
    // For storing the intermidiate values
    map<string, pair<int, string>> symtab;
    vector<pair<string, int>> littab;
    vector<string> pooltab;
    int lc, start, length;

    // opening i/p an o/p files
    ifstream fin;

    cout << "\nEnter the file name" << endl;
    string filename;
    cin >> filename;

    fin.open(filename);
    ofstream fout;
    fout.open("intermidiate.txt");

    int literal_index = 0, symindex = 0;
    lc = -1;
    string line, word;
    mnemonic Op;
    register_ Reg;
    condition Con;

    fout << "LC"
         << "   "
         << "Intermidate code o/p";
    fout << endl;

    while (getline(fin, line))
    {
        // cout<<"lc "<<lc<<endl;
        stringstream st(line);

        // read label
        st >> word;
        string label = "";

        // if word is not opcode then it must be label
        if (Op.Opcode.count(word) == 0)
        {
            // it it is not present in symbol table then add otherwise it must be op2 and hence assign it a lc
            if (symtab.count(word) == 0)
            {
                symtab[word] = {lc, to_string(symindex)};
                symindex++;
                // cout<<"word "<<word<<" "<<symtab[word].first<<endl;
            }
            else
            {
                symtab[word].first = lc;
            }
            // read opcode
            label = word;
            st >> word;
        }

        string operation = word;
        if (operation == "START")
        {
            fout << "    ";
            fout << "(" << Op.Opcode[word].first << ", " << Op.Opcode[word].second << ") ";
            // taking const input
            st >> word;
            fout << "(C," << word << ")";
            fout << endl;
            lc = stoi(word);
        }

        else if (operation == "END")
        {
            fout << "    ";
            fout << "(" << Op.Opcode[word].first << ", " << Op.Opcode[word].second << ") ";
            fout << endl;

            if (littab.size() != 0)
            {
                pooltab.push_back("#" + to_string(literal_index + 1));
                for (; literal_index < littab.size(); literal_index++)
                {
                    fout << lc << " ";
                    fout << "(" << Op.Opcode["DC"].first << ", " << Op.Opcode["DC"].second << ") ";
                    littab[literal_index].second = lc;
                    string literal = littab[literal_index].first;
                    string sublit = literal.substr(2, literal.length() - 3);
                    fout << "(C," << sublit << ") ";
                    fout << endl;   
                    lc++;
                }
                st >> word;
                if (st.fail())
                    break;
                operation = word;
            }
        }

        // If word is equals LTROG
        else if (operation == "LTORG")
        {
            // fout << lc << " ";
            fout << "(" << Op.Opcode[word].first << ", " << Op.Opcode[word].second << ") ";
            fout << endl;

            // Push the literal pool entry
            pooltab.push_back("#" + to_string(literal_index + 1));

            // Process literals in the literal table
            for (int i = literal_index; i < littab.size(); i++)
            {
                fout << lc << "     ";
                fout << "(" << Op.Opcode[word].first << ", " << Op.Opcode[word].second << ") ";

                // Update the literal's address in the literal table
                littab[literal_index].second = lc;
                // Extracting the substring
                string literal = littab[literal_index].first;
                size_t length = literal.length() - 3;
                string sublit = literal.substr(2, length);
                // cout<<"sublit "<<sublit<<endl;

                // Write the literal's information
                fout << "(C," << sublit << ") ";
                fout << endl;
                literal_index++;
                lc++;
            }
        }

        // It is EQU
        else if (operation == "EQU")
        {
            fout << "    ";
            fout << "No Intermediate code";
            fout << endl;

            // Read the next word from input
            st >> word;
            // Find the index of '+' or '-' sign in the word
            int plusminusindex = 0;
            // cout<<word<<endl;
            for (int i = 0; i < word.length(); i++)
            {
                if (word[i] == '+' || word[i] == '-')
                {
                    plusminusindex = i;
                    break;
                }
                // cout<<"hi"<<endl;
            }

            // Initialize variables for the sign and parts of the word
            char plusminus = '0';
            string aftersign;
            string beforesign;

            // Extract parts of the word based on sign
            if (plusminusindex != 0)
            {
                plusminus = word[plusminusindex];
                beforesign = word.substr(0, plusminusindex);
                aftersign = word.substr(plusminusindex + 1);
            }
            else
            {
                beforesign = word.substr(0, word.length());
            }

            // Calculate the value for the label in the symbol table
            symtab[label].first = symtab[beforesign].first;

            // cout<<"aftersign "<<aftersign<<endl;
            // cout<<"beforesign "<<beforesign<<endl;
            // cout<<"label "<<label<<endl;
            // cout<<"plusminus "<<plusminus<<endl;

            if (plusminus == '+')
            {
                symtab[label].first += stoi(aftersign);
            }
            else if (plusminus == '-')
            {
                symtab[label].first -= stoi(aftersign);
            }
        }

        // Origin
        else if (operation == "ORIGIN")
        {
            fout << "    ";
            fout << "(" << Op.Opcode[word].first << ", " << Op.Opcode[word].second << ") ";
            st >> word;

            int plusminusindex = 0;
            for (int i = 0; i < word.length(); i++)
            {
                if (word[i] == '+' || word[i] == '-')
                {
                    plusminusindex = i;
                    break;
                }
            }
            char plusminus = '0';
            string beforesign, aftersign;

            if (plusminusindex != 0)
            {
                plusminus = word[plusminusindex];
                aftersign = word.substr(plusminusindex + 1);
                beforesign = word.substr(0, plusminusindex);
            }
            else
            {
                beforesign = word.substr(0, word.length());
            }
            lc = symtab[beforesign].first;
            fout << "(S," << symtab[beforesign].second << ")";

            if (plusminus == '+')
            {
                lc += stoi(aftersign);
                fout << "+" << aftersign << "\n";
            }
            else if (plusminus == '-')
            {
                lc -= stoi(aftersign);
                fout << "-" << aftersign << "\n";
            }
        }
        // It is imperative statement
        else
        {
            fout << lc << " ";
            fout << "(" << Op.Opcode[word].first << ", " << Op.Opcode[word].second << ") ";

            while (st >> word)
            {
                if (operation == "DC")
                {
                    fout << "(C," << word << ") ";
                }
                else if (operation == "DS")
                {
                    int c = stoi(word);
                    if (c > 1)
                    {
                        lc += (c - 1);
                    }
                    fout << "(C," << word << ") ";
                }
                // It is a literal
                else if (word[0] == '=')
                {
                    littab.push_back({word, 0});
                    fout << "(L," << littab.size() << ") ";
                }
                // Present in Register
                else if (Reg.Register.count(word) > 0)
                {
                    fout << "(" << Reg.Register[word].first << ") ";
                }
                else if (Con.Condition.count(word) > 0)
                {
                    fout << "(" << Con.Condition[word].first << ") ";
                }
                // It is a symbol
                else
                {
                    // consider
                    if (symtab.count(word) == 0)
                    {
                        symtab[word] = {lc, to_string(symindex)};
                        fout << "(S," << symtab[word].second << ") ";
                        symindex++;
                    }
                    else
                        fout << "(S," << symtab[word].second << ") ";
                }
            }

            // Statement is over
            lc++;
            fout << endl;
        }
    }

    fin.close();
    fout.close();

    ofstream sout;
    sout << "Index"
         << " "
         << "Symbol"
         << "       "
         << "Address" << endl;
    sout.open("symbol_table.txt"); // writing to symbol table file
    for (auto i : symtab)
    {
        sout << i.second.second << " " << i.first << " " << i.second.first << endl;
    }
    sout.close();

    ofstream lout;
    lout << "Literal"
         << " "
         << "Address" << endl;
    lout.open("literal_table.txt"); // writing to literal table file
    for (auto i : littab)
    {
        lout << i.first << " " << i.second;
        lout << endl;
    }
    lout.close();

    ofstream pout;
    pout << "Pool Index" << endl;
    pout.open("pool_table.txt"); // writing to pool table file
    for (auto i : pooltab)
    {
        pout << i;
        pout << endl;
    }
    pout.close();

    cout << "\nProgram Executed!!" << endl;
}

int main()
{
    CodeGen table;
    table.read();
    return 0;
}