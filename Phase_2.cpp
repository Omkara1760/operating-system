#include<iostream>
#include<string>
#include<fstream>
#include<ctype.h>
#include<math.h>
#include <stdlib.h>
// #include<bits/stdc++.h>
using namespace std;
/*
TODO: Either store entire in page table at PTR+VA/10 or make sure the input file 
      has GD instructions in order i.e GD10 GD20 and so on 
      not like GD50 Directly

*/
class OS{

public:
    char R[4],IR[4];
    short int IC,VA,RA;
    bool C;
    char M[300][4];
    int SI,TI,PI,Jid,OD;
    string EM;
    int status[30];
    int PTR,curr;
    struct PCB{
        int TTL,TLL,TTC,LLC;
    }pcb;
    ifstream fp;
    ofstream fp2;
    void Load();
    int MOS();
    int allocate();
    void printMem();
    void execute();
    int getData();
    int terminate(int);
    int printData();
    int addressMap(int);
    void insertPage(int,int);
    void loadProgram(string);
    int stringDeci(string,int);
    void loadRegister();
    void storeRegister();
    void compareRegister();
    

};

int OS::MOS()
{
    cout<<"STATUS:Generating Interrupt.......\n";
    if(TI==0)
    {
        if(SI==1)
            {   SI=0;
                PI=0;
                return getData(); 
                
            }     //incase of valid page fault PI=3 else block wont be executed as this is executed first
        else if(SI==2)
            {   
                SI=0;
                PI=0;
                return printData();
          
                }
        else if(SI==3)
           return terminate(0);
        else if(PI==1)
            return terminate(4);
        else if(PI==2)
            return terminate(5);
        else if(PI==3)
            return terminate(6);
    }
    else if(TI==2)
    {
        if(SI==1)
            return terminate(3);
        else if(SI==2)
            {
                printData();
                return terminate(3);
            }
        else if(SI==3)
            return terminate(0);
        else if(PI==1)
            {
                fp2<<"TIME LIMIT EXCEEDED\n";
                return terminate(4);
            }
        else if(PI==2)
           { 
               fp2<<"TIME LIMIT EXCEEDED\n";
               return terminate(5);
            }
        else if(PI==3)
            return terminate(3);

    }
    return 0;
    
   
}
int OS::terminate(int err)
{   cout<<"STATUS:Terminating.......\n";
    fp2<<"JOB ID   :"<<Jid<<"\n";
    switch(err)
    {
       case 0:
       {fp2<<"NO ERROR \n";}
       break;
       case 1:
       {fp2<<"OUT OF DATA\n";}
       break;
       case 2:
       {fp2<<"LINE LIMIT EXCEEDED\n";}
       break;
       case 3:
       {fp2<<"TIME LIMIT EXCEEDED\n";}
       break;
       case 4:
       {fp2<<"OPERATION CODE ERROR\n";}
       break;
       case 5:
       {fp2<<"OPERAND ERROR\n";}
       break;
       case 6:
       {fp2<<"INVALID PAGE FAULT\n";}
    }
    fp2<<"IC       :"<<IC<<endl;
    fp2<<"IR       :"<<IR<<endl;
    fp2<<"SI       :"<<SI<<endl;
    fp2<<"PI       :"<<PI<<endl;
    fp2<<"TI       :"<<TI<<endl;
    fp2<<"TLL      :"<<pcb.TLL<<endl;
    fp2<<"TTC      :"<<pcb.TTC<<endl;
    fp2<<"LLC      :"<<pcb.LLC<<endl;
    fp2<<"TTL      :"<<pcb.TTL<<endl;
     fp2<<"\n\n";
    fp2.close();
    SI=0;
    PI=0;
    TI=0;
    return -1;

}

void OS::execute()
{   int loc;
    cout<<"STATUS:Executing the Program.......\n";
    fp2.open("output_2.txt",ios::app);
    int RIC=addressMap(IC);
    if(PI!=0)
    {
        int res=MOS();
          if(res==-1)
            return;
    }
    do
    {
      RIC=addressMap(IC);
    
     for(int i=0;i<4;i++)
      {
          IR[i]=M[RIC][i];
      }
          
      if(M[RIC][0]!='H'&&(!isdigit(M[RIC][2]) || !isdigit(M[RIC][3])))
      {   
         
          PI=2; 
          int res=MOS();
          if(res==-1)
            break;
         
      }
     
      VA=(M[RIC][2]-'0')*10+M[RIC][3]-'0';
     
      RA=addressMap(VA);
    
     
    //
 
            if(M[RIC][0]=='G' && M[RIC][1]=='D')
        {
           
            RA=RA-RA%10;  //GD works on a block so pass start of a block
            SI=1;
            
           
        }
        else if(M[RIC][0]=='P' && M[RIC][1]=='D')
        {
            RA=RA-RA%10;  //PD works on a block so pass start of a block
            SI=2;
           
           
        }
        else if(M[RIC][0]=='L' && M[RIC][1]=='R' )
        {   
            if(PI!=3)
            loadRegister();
        }
        else if(M[RIC][0]=='S' && M[RIC][1]=='R')
        {
            storeRegister();
        }
        else if(M[RIC][0]=='C' && M[RIC][1]=='R')
        {   
           
             if(PI!=3)
            compareRegister();
        }
        else if(M[RIC][0]=='B' && M[RIC][1]=='T')
        {   pcb.TTC+=1;
            if(C==true)
            { IC=VA;
                continue;
            }
        }
        else if(M[RIC][0]=='H')
        {
            pcb.TTC+=1;
            SI=3;
        }
        else
        {
            PI=1;
        }
        
    

      
      IC+=1;
      RIC+=1;
     
      if(SI!=0 || TI!=0 || PI!=0)
         {  
              int res=MOS();
          if(res==-1)
            break;
         }
          if(pcb.TTC>pcb.TTL)
            TI=2;
    //   cout<<"LOC->"<<loc;
  

    }while(true);//All Termination conditions handled inside therefore infinite loop
   

}
int OS::addressMap(int VA)
{
    int pte=PTR+VA/10;
 
    cout<<"STATUS:Mapping Virtual Address to Real Address.......\n";
    if(M[pte][2]!='*')
       { 
           int rad=((M[pte][2]-'0')*10+(M[pte][3]-'0'))*10+VA%10;
          
            return rad;
       }
    else
    {   
        
        PI=3;
        return -1;
    }
    

}
int OS::stringDeci(string word,int st){
    
    return ((word[st]-'0')*1000+(word[st+1]-'0')*100+(word[st+2]-'0')*10+(word[st+3]-'0'));

}
void OS::insertPage(int loc,int ch=-1)
{
   string l=to_string(loc/10);

   int lc;

       lc=curr;


   M[lc][0]=' ';
   M[lc][1]=' ';
   if((loc/10)<10)
   {M[lc][2]='0';
   M[lc][3]=l[0];}
   else{
       M[lc][2]=l[0];
       M[lc][3]=l[1];
        }  
   curr+=1;
   
   
   
   

}
void OS::loadProgram(string word){

    int l=word.length();
    int i=0,hcount=0;
    cout<<"STATUS:Loading Program in Memory.....\n";
    while(i<l)
    {   hcount=0;
        int st=allocate();
        insertPage(st);
        int tmp=l-i,cond;
        if(tmp<40)
            cond=tmp/4+tmp%4; 
                             
                              
        else
            cond=10;
        for(int j=st;j<(st+cond);j++)
        {
            for(int k=0;k<4;k++)
                {
                    M[j][k]=word[i];
                    if(word[i]=='H')
                        {
                            M[j][1]=M[j][2]=M[j][3]=' ';
                            i+=1;
                            break;
                        }
                        i+=1;

                }
               
        }
      

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
            PTR=allocate();
            curr=PTR;
            for(int j=PTR;j<(PTR+10);j++)
            {
                for(int k=0;k<4;k++)
                {
                    M[j][k]='*';
                }
            }
            //Read Next Word and Load into memory
            pcb.TTL=stringDeci(word,8);
            pcb.TLL=stringDeci(word,12);
            Jid=stringDeci(word,4);
           
            getline(fp, word);
            loadProgram(word);
            
           
         
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
          
           break; 
        }
         
         if(OD==1)
         {
             OD=0;
             break;
         }
      
    }


}
int OS::allocate()
{   
    cout<<"STATUS:Allocating Memory....\n";
    int loc=rand()%30;
    while(status[loc]==1)
    {
        loc=rand()%30;
    }
    status[loc]=1;
   
   
    return loc*10;
}
void OS::printMem()
{
    for(int i=0;i<300;i++)
    {   cout<<(i)<<"->";
        for(int j=0;j<4;j++)
        {
            cout<<M[i][j]<<" ";
        }
        cout<<endl;
        if((i+1)%10==0)
        cout<<"\n\n";
    }
     cout<<"--------------------------------------------------\n";   
}
int OS::getData()
{
   cout<<"STATUS:Getting Data from File......\n";
    string word;
    pcb.TTC+=2;
   
    getline(fp, word);
    if(pcb.TTC>pcb.TTL)
    {
        return terminate(3);
    }
    if(word.find("$END")!=string::npos)
    {   
        
       
        OD=1;
        int r=terminate(1);
        return r;
    }
    int lpg=allocate();
    insertPage(lpg,VA);
    int l=word.length();
    for(int i=0;i<=l;i++)
    {   if(i!=l)
            M[lpg+i/4][i%4]=word[i];
        else
            M[lpg+i/4][i%4]='\0';
        // cout<<(loc+i/4)<<"->"<<(i%4)<<endl;
    }
    return 0;
}
int OS::printData()
{   

    cout<<"STATUS:Printing Data to File....\n";
    int flag=0;
    pcb.TTC+=1;
    pcb.LLC+=1;
  
    

    if(pcb.LLC>pcb.TLL && TI!=2)
        {
           return terminate(2);
        }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<4;j++)
        {   
            if(M[RA+i][j]!='\0')
               //cout<<M[loc+i][j];
              {     
                   
                  fp2<<M[RA+i][j];}
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
    return 0;
}

void OS::loadRegister()
{   
    cout<<"STATUS:Loading Register....\n";
    pcb.TTC+=1;
    for(int i=0;i<4;i++)
        R[i]=M[RA][i];
}
void OS::storeRegister()
{   
    pcb.TTC+=2;
    cout<<"STATUS:Storing Register Value in Memory.....\n";
    int lpg=allocate();
    insertPage(lpg,VA);
    for(int i=0;i<4;i++)
        M[lpg][i]=R[i];
    if(PI==3)
        PI=0;
}
void OS::compareRegister()
{   
    cout<<"STATUS:Comparing with Register....\n";
    pcb.TTC+=1;
    int f=0;
    for(int i=0;i<4;i++)
        {
        if(M[RA][i]==R[i])
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
//TODO:In case of Out of DATA error $END is not encountered in LOAD() Handle that
int main()
{
    OS obj;
    obj.fp.open("input_2.txt"); 
    int jobs=7;
    for(int i=0;i<jobs;i++)
        {

        obj.PI=0;
        obj.SI=0;
        obj.TI=0;
        obj.pcb.TTC=0;
        obj.pcb.LLC=0;
        for(int i=0;i<300;i++)
        {   
            for(int j=0;j<4;j++)
            {
            obj.M[i][j]=' ';
            }
            if(i%10==0)
                obj.status[i/10]=0;
        }    

        obj.OD=0;
        obj.Load();
    
    
    for(int i=0;i<300;i++)
    {   cout<<(i)<<"->";
        for(int j=0;j<4;j++)
        {
            cout<<obj.M[i][j]<<" ";
        }
        cout<<endl;
        if((i+1)%10==0)
        cout<<"\n\n";
    }
     cout<<"--------------------------------------------------\n";   
    }
    
}



