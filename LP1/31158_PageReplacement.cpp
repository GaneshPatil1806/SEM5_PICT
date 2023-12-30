#include<bits/stdc++.h>
#include<unordered_set>
using namespace std;

void FIFO(int size,int n,vector<int> v)
{
    int fault=0,hit=0;
    vector<int> frame(size,-1);
    int index=0;
    for(int i=0;i<n;i++){
        if(find(frame.begin(),frame.end(),v[i])==frame.end()){
            frame[index%size]=v[i];
            fault++;
            index++;
        }else{
            hit++;
        }

        for(auto i:frame){
            cout<<i<<" ";
        }
        cout<<endl;
    }

    cout<<"\nHits: "<<hit<<" Faults: "<<fault<<endl;  
}

void LRU(int size,int n,vector<int> v)
{
    int fault=0,hit=0;
    unordered_set<int> s; 
    unordered_map<int, int> indexes; 
    int index=0;
    for(int i=0;i<n;i++){
        if(s.size()<size){
            if(s.find(v[i])==s.end()){
                s.insert(v[i]);
                fault++;
            }
            else{
                hit++;
            }
            indexes[v[i]]=i;
        }
        else{
            if(s.find(v[i])==s.end())
            {
                int val,lruInd=INT_MAX;
                for(auto i:s)
                {
                    if(indexes[i]<lruInd){
                        lruInd=indexes[i];
                        val=i;
                    }
                }
                s.insert(v[i]);
                s.erase(val);
                fault++;
            }
            else{
                hit++;
            }

            indexes[v[i]]=i;
        } 

        for(auto i:s){
            cout<<i<<" ";
        }
        cout<<endl;
    }

    cout<<"\nHits: "<<hit<<" Faults: "<<fault<<endl;  
}

void OPTIMAL(int size,int n,vector<int> v)
{
    int fault=0,hit=0;
    vector<int> frames(size,-1);
    int index=0;
    for(int i=0;i<n;i++){
        if(index<size){
            if(find(frames.begin(),frames.end(),v[i])==frames.end()){
                frames[index]=v[i];
                index++;
            }
            else{
                hit++;
            }
        }
        else{
            if(find(frames.begin(),frames.end(),v[i])==frames.end())
            {
                int lasti=-1,ind=-1;
                for(int j=0;j<size;j++){
                    bool f=0;
                    for(int k=i;k<n;k++){
                        if(v[k]==frames[j]){
                            f=1;
                            if(k>lasti)
                            {
                                lasti=k;
                               // cout<<lasti<<", "<<v[lasti]<<endl;
                                ind=j;
                            }
                            break;
                        }
                    }
                    if(!f){
                        ind=j;
                        break;
                    }
                }

                if(lasti==-1)
                {
                    frames[0]=v[i];
                }
                else{
                    //cout<<"ind "<<ind<<endl;
                    frames[ind]=v[i];
                }
                fault++;
            } 
            else{
                hit++;
            }  
        } 

        for(auto i:frames){
            cout<<i<<" ";
        }
        cout<<endl;
    }
    cout<<"\nHits: "<<hit<<" Faults: "<<fault<<endl;  
}

int main()
{
    int size,in;

    cout<<"Enter the size of framestack"<<endl;
    cin>>size;

    cout<<"Enter the lenght of input buffer"<<endl;
    cin>>in;

    vector<int> input(in);

    cout<<"Enter the elements"<<endl;
    for(int i=0;i<in;i++){
        cin>>input[i];
    }

    int ch;
    while(1)
    {
        cout<<"Enter the operation you want to perform"<<endl;
        cout<<"1.FIFO"<<endl;
        cout<<"2.LRU"<<endl;
        cout<<"3.OPTIMAL"<<endl;
        cin>>ch;
        switch (ch)
        {
            case 1:
            {
                FIFO(size,in,input);
                break;
            }
            case 2:
            {
                LRU(size,in,input);
                break;
            }
            case 3:
            {
                OPTIMAL(size,in,input);
                break;
            }
            case 4:
            {
                cout<<"\nEXIT"<<endl;
                exit(0);
            }
            default:
            {
                cout<<"\nInvalid input"<<endl;
                break;
            }  
        }
    }
    return 0;
}