#include<bits/stdc++.h>
using namespace std;

int PTBR;
int si=0;
int pi=0;
int ti=0;
int ttl;
int ttc=0;
int available[30]={0};
int getrandom();
void execute_MOS();
void execute_GD();
void execute_PD();
void execute_LR();
void execute_SR();
void execute_CR();
void check();
void terminate(int x,int y);

ifstream input_file("input.txt");
ofstream out_file("output.txt");

class cpu
{
public:
  char IR[4];
  char R[4];
  int IC;
  int T;

  void display()
  {
     cout<<endl<<"IR:"<<IR<<endl;
     cout<<"R :"<<R<<endl;
     cout<<"IC:"<<IC<<endl;
     cout<<"T :"<<T<<endl;
  }

};


class ram
{
public:
  char A[300][4];
 /* ram()
  {
     for(int i=0;i<100;i++)
     {
       for(int j=0;j<4;j++)
	 {
	  A[i][j]='0';
	 }
     }
   }*/
void display()
  {
     cout<<"\nContents of RAM:"<<endl;

     for(int i=0;i<300;i++)
     {
       cout<<i<<":";
       for(int j=0;j<4;j++)
	 {
	    cout<<A[i][j];
	 }
	 cout<<"\t";
     }
     cout<<endl;
  }
};
cpu CPU;
ram RAM;
char buffer[40];
void makepageentry(int P,int F)
{
RAM.A[(PTBR*10)+P][1]=' ';
RAM.A[(PTBR*10)+P][2]=(F/10)+48;
RAM.A[(PTBR*10)+P][3]=(F%10)+48;
RAM.A[(PTBR*10)+P][0]='$';
}
void load_program()
{
int f1=0,f2=0;
char *ptr=RAM.A[0];//&RAM.A[0][0]
input_file.getline(buffer,41);
if(buffer[0]=='$' && buffer[1]=='A')
	{
	ttl=(buffer[10]-48)*10+(buffer[11]-48);
	}
if(buffer[0]!='$')
	return;
input_file.getline(buffer,41);
int PN=0,FN;
while(buffer[0]!='$')
	{
	FN = getrandom();
	makepageentry(PN,FN);
	ptr = &RAM.A[FN*10][0];
	for(int i=0;i<40 & buffer[i]!=0;i++)
		*ptr++=buffer[i];
	input_file.getline(buffer,41);
	PN++;
	}
}

/*void check()
{
input_file.getline(buffer,41);
if(buffer[0]=='$' && buffer[1]=='E' && buffer[2]=='N' && buffer[3]=='D')
f1=1;
input_file.getline(buffer,41);
if(buffer[0]=='$' && buffer[1]=='A' && buffer[2]=='M' && buffer[3]=='J')
f2=1;
if(f1==1 && f2==1)
load_program();
}*/
int APage=0;
void load_IR()
{
int FN = (RAM.A[(PTBR*10)+APage][2]-48)*10+ (RAM.A[(PTBR*10)+APage][3]-48);
CPU.IR[0]=RAM.A[(FN*10)+CPU.IC][0];
CPU.IR[1]=RAM.A[(FN*10)+CPU.IC][1];
CPU.IR[2]=RAM.A[(FN*10)+CPU.IC][2];
CPU.IR[3]=RAM.A[(FN*10)+CPU.IC][3];
CPU.IC++;
if(CPU.IC%10==0)
	{
	APage++;
	CPU.IC=0;
	}
}

void START_SIMULATION()
{
while(1)
	{
	//ttl--;
        ttc++;
        if(ttc>ttl)
//        if(ttl==0)
        ti=2;
	load_IR();
	if(CPU.IR[0]=='G' && CPU.IR[1]=='D')
		si=1;
	else if(CPU.IR[0]=='P'&&CPU.IR[1]=='D')
		si=2;
	else if(CPU.IR[0]=='L'&&CPU.IR[1]=='R')
		execute_LR();
	else if(CPU.IR[0]=='S'&&CPU.IR[1]=='R')
		execute_SR();
	else if(CPU.IR[0]=='C'&&CPU.IR[1]=='R')
		execute_CR();
	else if(CPU.IR[0]=='H')
        	//CPU.display();
		//RAM.display();
		si=3;
	else pi=1;
	execute_MOS();
	}
}

void execute_GD()
{
  int i[2],j;
  int PN;
  input_file.getline(buffer,41);
  if((CPU.IR[2]>='0' && CPU.IR[2]<='9') && (CPU.IR[3]>='0' && CPU.IR[3]<='9'))
  {
  i[0]=CPU.IR[2]-48;
  i[1]=CPU.IR[3]-48;
  PN=i[0];
	if(RAM.A[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in GD...";
		makepageentry(PN,getrandom());
		}
  int FN = (RAM.A[(PTBR*10)+PN][2]-48)*10 + (RAM.A[(PTBR*10)+PN][3]-48);
	char *ptr=RAM.A[FN*10];
  j=0;
  while(buffer[j]!=0)
  {

    *ptr++=buffer[j];
	j++;
  }
 }
else
  pi=2;

}


void execute_PD()
{
int i[2],a,j;
if((CPU.IR[2]>='0' && CPU.IR[2]<='9') && (CPU.IR[3]>='0' && CPU.IR[3]<='9'))
{
i[0]=CPU.IR[2]-48;
i[1]=CPU.IR[3]-48;
//a=i[0]*10+i[1];
 int PN=i[0];
	if(RAM.A[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in PD...";
		//makepageentry(PN,getrandom());
		}
         else
           {
  int FN = (RAM.A[(PTBR*10)+PN][2]-48)*10 + (RAM.A[(PTBR*10)+PN][3]-48);
	    char *ptr=RAM.A[FN*10];
            //char *ptr=RAM.A[a];
          for(int j=0;j<40;j++)
           buffer[j]=0;
           j=0;
        while(j<41 && *ptr!=0)
           {
             buffer[j]=*ptr++;
             j++;
           }
         out_file<<buffer<<endl;
        }
}
else
pi=2;
}

void execute_LR()
{
int i[20],a,j=0;
if((CPU.IR[2]>='0' && CPU.IR[2]<='9') && (CPU.IR[3]>='0' && CPU.IR[3]<='9'))
{
i[0]=CPU.IR[2]-48;
i[1]=CPU.IR[3]-48;
//a=i[0]*10+i[1];
int PN=i[0];
	if(RAM.A[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in LR...";
		//makepageentry(PN,getrandom());
		}
	else
{
  int FN = (RAM.A[(PTBR*10)+PN][2]-48)*10 + (RAM.A[(PTBR*10)+PN][3]-48);
	//char *ptr=RAM.A[FN*10];
while(j<4)
{
CPU.R[j]=RAM.A[FN*10][j];
j++;
}
}
}
else pi=2;
}

void execute_SR()
{
int i[2],a,j=0;
if((CPU.IR[2]>='0' && CPU.IR[2]<='9') && (CPU.IR[3]>='0' && CPU.IR[3]<='9'))
{
i[0]=CPU.IR[2]-48;
i[1]=CPU.IR[3]-48;
a=i[0]*10+i[1];
int PN=i[0];
	if(RAM.A[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in SR...";
		makepageentry(PN,getrandom());
		}
  int FN = (RAM.A[(PTBR*10)+PN][2]-48)*10 + (RAM.A[(PTBR*10)+PN][3]-48);
while(j<4)
{
RAM.A[FN*10][j]=CPU.R[j];
j++;
}
}
else pi=2;
}

void execute_CR()
{
int i[2],a,j=0;
if((CPU.IR[2]>='0' && CPU.IR[2]<='9') && (CPU.IR[3]>='0' && CPU.IR[3]<='9'))
{
i[0]=CPU.IR[2]-48;
i[1]=CPU.IR[3]-48;
//a=i[0]*10+i[1];
int PN=i[0];
	if(RAM.A[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in CR...";
		//makepageentry(PN,getrandom());
		}
else
{
  int FN = (RAM.A[(PTBR*10)+PN][2]-48)*10 + (RAM.A[(PTBR*10)+PN][3]-48);
if(CPU.R[0]==RAM.A[FN*10][0] && CPU.R[1]==RAM.A[FN*10][1] && CPU.R[2]==RAM.A[FN*10][2] &&CPU.R[3]==RAM.A[FN*10][3])
CPU.T=1;
}
}
else pi=2;
}

void execute_BT()
{
int i[2],a;
if((CPU.IR[2]>='0' && CPU.IR[2]<='9') && (CPU.IR[3]>='0' && CPU.IR[3]<='9'))
{
i[0]=CPU.IR[2]-48;
i[1]=CPU.IR[3]-48;
a=i[0]*10+i[1];
if(CPU.T==1)
CPU.IC=a;
}
else pi=2;
}

void terminate(int x,int y=-1)
{
  if(x==0)
  cout<<"No error";
  if(x==3)
  cout<<"Time Limit exceeded";

 if(x==4)
{
  cout<<"opcode error";
}
 if(x==5)
  cout<<"operand error";
  if(x==3 && y==4)
  cout<<"time limit exceeded and opcode error";
  if(x==3 && y==5)
  cout<<"time limit exceeded and operand error";

}


void execute_MOS()
{
if(ti==0 && si==1)
execute_GD();
if(ti==0 && si==2)
execute_PD();
if(ti==0 && si==3)
{
//return;
//exit(0);
//check();
terminate(0);
CPU.display();
RAM.display();
exit(0);
}
if(ti==2 && si==1)
{
//return;
//exit(0);
//check();
terminate(3);
CPU.display();
RAM.display();
exit(0);
}

//terminate(3);
if(ti==2 && si==2)
{
execute_PD();
terminate(3);
CPU.display();
RAM.display();
exit(0);
}
if(ti==2 && si==3)
{
terminate(0);
CPU.display();
RAM.display();
exit(0);
}
if(ti==0 && pi==1)
terminate(4);
if(ti==0 && pi==2)
terminate(5);
if(ti==2 && pi==1)
{
terminate(3,4);
CPU.display();
RAM.display();
exit(0);
}
if(ti==2 && pi==2)
{
terminate(3,5);
CPU.display();
RAM.display();
exit(0);
}
pi=0;
si=0;
ti=0;
}
int getrandom()
	{
	int i;
	do
	{
	i=rand()%30;
	}
	while(available[i]!=0);
	available[i]=1;
	return i;
	}
int main()
{
PTBR = getrandom();//0-29 available
for(int j=0;j<10;j++)
	{
	int base = PTBR*10;
	RAM.A[base+j][0]='*';
	}

for(int i=0;i<30;i++)
cout<<available[i]<<"\n";
load_program();
CPU.IC=0;
START_SIMULATION();
CPU.display();
RAM.display();

input_file.close();
out_file.close();
return 0;
}












