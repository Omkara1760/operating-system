#include<iostream>
#include<string>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;

class OS{

public:
    char R[4],IR[4];
    short int IC;
    bool C;
    char M[100][4];
    char SI;
    ifstream fp;
    ofstream fp2;
    void Load();
    void MOS(int loc);
    void execute();
    void getData(int);
    void printData(int);
    void loadRegister(int);
    void storeRegister(int);
    void compareRegister(int);

};
void OS::MOS(int loc)
{
    if(SI=='1')
        getData(loc);
    else if(SI=='2')
        printData(loc);

}
void OS::execute()
{   int loc;
    fp2.open("output-1.txt",ios::app);
    while(M[IC][0]!='H')
    {
        
      loc=(M[IC][2]-'0')*10+M[IC][3]-'0';
      if(M[IC][0]=='G' && M[IC][1]=='D')
      {
          loc=loc-loc%10;  //GD works on a block so pass start of a block
        //   getData(loc);
            SI='1';
            MOS(loc);
      }
      else if(M[IC][0]=='P' && M[IC][1]=='D')
      {
          loc=loc-loc%10;  //PD works on a block so pass start of a block
        //   printData(loc);
         SI='2';
            MOS(loc);
      }
      else if(M[IC][0]=='L' && M[IC][1]=='R')
      {
          loadRegister(loc);
      }
      else if(M[IC][0]=='S' && M[IC][1]=='R')
      {
          storeRegister(loc);
      }
      else if(M[IC][0]=='C' && M[IC][1]=='R')
      {
          compareRegister(loc);
      }
      else if(M[IC][0]=='B' && M[IC][1]=='T')
      {
          if(C==true)
           { IC=loc;
             continue;
           }
      }
      
      IC+=1;
    //   cout<<"LOC->"<<loc;
    }
   

}

void OS::Load()
{
    
    
    string word,cont1="$AMJ",cont2="$DTA",cont3="$END";
    size_t found;
    while(getline(fp, word))
    { 
        if(word.find(cont1)!=string::npos)
        {
            //Read Next Word and Load into memory
            getline(fp, word);
            int l=word.length();
            for(int i=0;i<l;i++)
            {
                M[i/4][i%4]=word[i];
               
            }
        }
        else if(word.find(cont2)!=string::npos)
        {
            //Start Execution
            IC=0;
            
            execute();
        }
        else if(word.find(cont3)!=string::npos)
        {
           //cout<<"\n\n";
           fp2<<"\n\n";
           fp2.close();
           break; 
        }
    }


}

void OS::getData(int loc)
{
    string word;
    getline(fp, word);
    int l=word.length();
    for(int i=0;i<=l;i++)
    {   if(i!=l)
            M[loc+i/4][i%4]=word[i];
       
    }
}
void OS::printData(int loc)
{   
    int flag=0;
    
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<4;j++)
        {   
            if(M[loc+i][j]!='\0')
               //cout<<M[loc+i][j];
              { fp2<<M[loc+i][j];}
            else
                {
                    flag=1;
                    break;
                }
        }
        if(flag==1)
            break;
    }
    //cout<<"\n";
    fp2<<"\n";

}

void OS::loadRegister(int loc)
{
    for(int i=0;i<4;i++)
        R[i]=M[loc][i];
}
void OS::storeRegister(int loc)
{
    for(int i=0;i<4;i++)
        M[loc][i]=R[i];
}
void OS::compareRegister(int loc)
{
    int f=0;
    for(int i=0;i<4;i++)
        {
        if(M[loc][i]==R[i])
            continue;
        else
            {   f=1;
                break;
                }
        }
    if(f==1)
        C=false;
    else
        C=true;

}
int main()
{
    OS obj;
    obj.fp.open("input-1.txt"); 
    
    for(int i=0;i<6;i++)
        {
        for(int i=0;i<100;i++)
        {   
            for(int j=0;j<4;j++)
            {
            obj.M[i][j]=' ';
            }
        }    
            obj.Load();
    for(int i=0;i<100;i++)
    {   cout<<(i)<<"->";
        for(int j=0;j<4;j++)
        {
            cout<<obj.M[i][j]<<" ";
        }
        cout<<endl;
        if((i+1)%10==0)
        cout<<"\n\n";
    }
        }
    
}



