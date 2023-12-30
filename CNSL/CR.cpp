#include<iostream>
#include<bits/stdc++.h>
#include<vector>
using namespace std;
class Hamming_code
{
    vector<int>in_data;
    vector<string>v;
    string s;
    int input_data;
    int data[12];
    int cc[4];
    public:
    Hamming_code()
    {
        input_data=0;
    }
    void getstring()
    {
        cout<<"ENTER STRING---"<<endl;
        getline(cin,s);
        string temp="";
        for(int i=0;i<s.length();i++)
        {
            if(s[i]==' ')
            {
                continue;
            }
            else{
                cout<<s[i]<<endl;
                strToBinary(s[i]); 
            }
        }
    }
    void strToBinary(char s)
   {
        int val = int(s);
        string bin = "";
        while (val > 0)
        {
            (val % 2)? bin.push_back('1') :
                       bin.push_back('0');
            val /= 2;
        }
        reverse(bin.begin(), bin.end()); 
        getdata(bin);   
    }
    void getdata(string b)
    {
        for(int i=0;i<b.length();i++)
        {
            if(b[i]=='0')
            {
                in_data.push_back(0);
            }
            else{
                in_data.push_back(1);
            }
        }
        reverse(in_data.begin(),in_data.end());
        int k=0;
        for(int i=3;i<12;i++)
        {
            if(i==4||i==8)
            {
                continue;
            }
            data[i]=in_data[k];
            k++;
        }
        parity_code();
    }
    void showdata()
    {
        cout<<"ENCODED DATA---";
        for(int i=1;i<12;i++)
        {
            cout<<data[i]<<" ";
        }
        cout<<endl;
        cout<<"THIS DATA IS SENT-->";
        cout<<endl;
    }
    int even_odd(int a)
    {
        if(a%2==0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    void parity_code()
    {
        //FOR FIRST PARITY BIT
        int k=3;
        int cnt=0;
        while(k<12)
        {
            if(data[k]==1)
            {
                cnt++;
            }
            k=k+2;
        }
        if(even_odd(cnt))
        {
            data[1]=1;
        }
        else
        {
            data[1]=0;
        }
        //FOR SECOND PARITY BIT
        k=3;
        cnt=0;
        while(k<12)
        {
            if(k==4||k==5||k==8||k==9)
            {
                k++;
                continue;
            }
            if(data[k]==1)
            {
                cnt++;
            }
            k++;
        }
        if(even_odd(cnt))
        {
            data[2]=1;
        }
        else
        {
            data[2]=0;
        }
        //FOR THIRD PARITY BIT
        k=5;
        cnt=0;
        while(k<8)
        {
            if(data[k]==1)
            {
                cnt++;
            }
            k=k+1;
        }
        if(even_odd(cnt))
        {
            data[4]=1;
        }
        else
        {
            data[4]=0;
        }
        //FOR FOURTH PARITY BIT
        k=9;
        cnt=0;
        while(k<12)
        {
            if(data[k]==1)
            {
                cnt++;
            }
            k++;
        }
        if(even_odd(cnt))
        {
            data[8]=1;
        }
        else
        {
            data[8]=0;
        }
        cout<<endl;
        cout<<"Parity Bits are--"<<" "<<data[1]<<" "<<data[2]<<" "<<data[4]<<" "<<data[8]<<" "<<endl;
        receive_data();
    }
    void receive_data()
    {
        cout<<"DO YOU WANT TO INDUCE AN ERROR--"<<endl;
        char a;
        cin>>a;
        cc[0]=data[1];
        cc[1]=data[2];
        cc[2]=data[4];
        cc[3]=data[8];
        if(a=='Y'||a=='y')
        {
            if(data[6]==0)
            {
                data[6]=1;
            }
            else
            {
                data[6]=0;
            }
        }
        else{
            checker();
        }    
    }
    void checker()
    {
        int flag=0;
        int loc=0;
        for(int i=0;i<4;i++)
        {
            if(cc[i]==0)
            {
               continue;
            }
            else
            {
                flag=1;
                loc=loc+pow(2,i);
            }
        }
        cout<<"VALUE OF CHECKER BITS--- ";
        for(int i=0;i<4;i++)
        {
            cout<<cc[i]<<" ";
        }
        if(flag==0)
        {
            cout<<"DATA TRANSMITTED CORRECTLY";
        }
        else
        {
             cout<<"ERROR IN DATA TRANSMISSION"<<endl;
             cout<<"ERROR AT LOCATION--"<<loc<<endl;
        }
    }
};
int main()
{
    Hamming_code h;
    h.getstring();
   //h.parity_code();
    cout<<endl;
    //h.showdata();
   // h.receive_data();
   // h.checker();
    return 0;
}
