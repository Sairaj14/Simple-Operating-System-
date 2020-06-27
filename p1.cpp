/*

	OS67 Kernel

date:-6/2/2019
by:- Sairaj

*/

#include<bits/stdc++.h>
using namespace std;

//iniTIalize input file
ifstream input_file("input.txt");
ofstream output_file("output.txt");
int PTBR,SI=0,PI=0,TI=0,TTL=0,TTC=0;
int available[30]={0};
void startSImulaTIon();
int getrandom();

//cpu
class CPU
{
public:
  char IR[4];
  char REG[4];
  int IC;
  int T;

  void display()
  {
     cout<<endl<<"IR:"<<IR<<endl;
     cout<<"REG :"<<REG<<endl;
     cout<<"IC:"<<IC<<endl;
     cout<<"T :"<<T<<endl;
  }

};

//ram
class RAM
{
public:
  char M[300][4];
 void display()
  {
     cout<<"\nContents of RAM:"<<endl;

     for(int i=0;i<300;i++)
     {
       cout<<i<<":";
       for(int j=0;j<4;j++)
	 {
	    cout<<M[i][j];
	 }
	 cout<<"\t";
     }
     cout<<endl;
  }
};

CPU cpu;
RAM ram;
char buffer[40];

void makepageentry(int P,int F)
{
ram.M[(PTBR*10)+P][1]=' ';
ram.M[(PTBR*10)+P][2]=(F/10)+48;
ram.M[(PTBR*10)+P][3]=(F%10)+48;
ram.M[(PTBR*10)+P][0]='$';
}
//to load program from file to RAM

void load_program()
{
int f1=0,f2=0;
char *ptr=ram.M[0];//&ram.M[0][0]
input_file.getline(buffer,41);
if(buffer[0]=='$' && buffer[1]=='A')
	{
	TTL=(buffer[10]-48)*10+(buffer[11]-48);
	}
if(buffer[0]!='$')
	return;
input_file.getline(buffer,41);
int PN=0,FN;
while(buffer[0]!='$')
	{
	FN = getrandom();
	makepageentry(PN,FN);
	ptr = &ram.M[FN*10][0];
	for(int i=0;i<40 & buffer[i]!=0;i++)
		*ptr++=buffer[i];
	input_file.getline(buffer,41);
	PN++;
	}
}

//store result in output file
void out_putData(int add)
{
	char *ptr=ram.M[add];
	for(int i=0;i<40 &&*ptr!='$';i++)
	{
		buffer[i]=*ptr++;
	}

	output_file<<buffer<<endl;
}

void execute_GD()
{
  int i[2],j;
  int PN;
  input_file.getline(buffer,41);
  if((cpu.IR[2]>='0' && cpu.IR[2]<='9') && (cpu.IR[3]>='0' && cpu.IR[3]<='9'))
  {
  i[0]=cpu.IR[2]-48;
  i[1]=cpu.IR[3]-48;
  PN=i[0];
	if(ram.M[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in GD...";
		makepageentry(PN,getrandom());
		}
  int FN = (ram.M[(PTBR*10)+PN][2]-48)*10 + (ram.M[(PTBR*10)+PN][3]-48);
	char *ptr=ram.M[FN*10];
  j=0;
  while(buffer[j]!=0)
  {

    *ptr++=buffer[j];
	j++;
  }
 }
else
  PI=2;

}


void execute_PD()
{
int i[2],a,j;
if((cpu.IR[2]>='0' && cpu.IR[2]<='9') && (cpu.IR[3]>='0' && cpu.IR[3]<='9'))
{
i[0]=cpu.IR[2]-48;
i[1]=cpu.IR[3]-48;

 int PN=i[0];
	if(ram.M[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in PD...";
		}
         else
           {
  int FN = (ram.M[(PTBR*10)+PN][2]-48)*10 + (ram.M[(PTBR*10)+PN][3]-48);
	    char *ptr=ram.M[FN*10];
          for(int j=0;j<40;j++)
           buffer[j]=0;
           j=0;
        while(j<41 && *ptr!=0)
           {
             buffer[j]=*ptr++;
             j++;
           }
         output_file<<buffer<<endl;
        }
}
else
PI=2;
}

void execute_LR()
{
int i[20],a,j=0;
if((cpu.IR[2]>='0' && cpu.IR[2]<='9') && (cpu.IR[3]>='0' && cpu.IR[3]<='9'))
{
i[0]=cpu.IR[2]-48;
i[1]=cpu.IR[3]-48;
//a=i[0]*10+i[1];
int PN=i[0];
	if(ram.M[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in LR...";
		}
	else
{
  int FN = (ram.M[(PTBR*10)+PN][2]-48)*10 + (ram.M[(PTBR*10)+PN][3]-48);
	while(j<4)
{
cpu.REG[j]=ram.M[FN*10][j];
j++;
}
}
}
else PI=2;
}

void execute_SR()
{
int i[2],j=0;
int a;
if((cpu.IR[2]>='0' && cpu.IR[2]<='9') && (cpu.IR[3]>='0' && cpu.IR[3]<='9'))
{
i[0]=cpu.IR[2]-48;
i[1]=cpu.IR[3]-48;
a=i[0]*10+i[1];
int PN=i[0];
	if(ram.M[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in SR...";
		makepageentry(PN,getrandom());
		}
  int FN = (ram.M[(PTBR*10)+PN][2]-48)*10 + (ram.M[(PTBR*10)+PN][3]-48);
while(j<4)
{
ram.M[FN*10][j]=cpu.REG[j];
j++;
}
}
else PI=2;
}

void execute_CR()
{
int i[2],a,j=0;
if((cpu.IR[2]>='0' && cpu.IR[2]<='9') && (cpu.IR[3]>='0' && cpu.IR[3]<='9'))
{
i[0]=cpu.IR[2]-48;
i[1]=cpu.IR[3]-48;
int PN=i[0];
	if(ram.M[(PTBR*10)+PN][0]=='*')
		{
		cout<<"\nPage Fault in CR...";
		}
else
{
  int FN = (ram.M[(PTBR*10)+PN][2]-48)*10 + (ram.M[(PTBR*10)+PN][3]-48);
if(cpu.REG[0]==ram.M[FN*10][0] && cpu.REG[1]==ram.M[FN*10][1] && cpu.REG[2]==ram.M[FN*10][2] &&cpu.REG[3]==ram.M[FN*10][3])
cpu.T=1;
}
}
else PI=2;
}

void execute_BT()
{
int i[2],a;
if((cpu.IR[2]>='0' && cpu.IR[2]<='9') && (cpu.IR[3]>='0' && cpu.IR[3]<='9'))
{
i[0]=cpu.IR[2]-48;
i[1]=cpu.IR[3]-48;
a=i[0]*10+i[1];
if(cpu.T==1)
cpu.IC=a;
}
else PI=2;
}
int APage;
void load_IR()
{
int FN = (ram.M[(PTBR*10)+APage][2]-48)*10+ (ram.M[(PTBR*10)+APage][3]-48);
cpu.IR[0]=ram.M[(FN*10)+cpu.IC][0];
cpu.IR[1]=ram.M[(FN*10)+cpu.IC][1];
cpu.IR[2]=ram.M[(FN*10)+cpu.IC][2];
cpu.IR[3]=ram.M[(FN*10)+cpu.IC][3];
cpu.IC++;
if(cpu.IC%10==0)
	{
	APage++;
	cpu.IC=0;
	}

}
void terminate(int x,int y=-1)
{
  if(x==0)
  output_file<<"No error";
  if(x==3)
  output_file<<"Time Limit exceeded";

 if(x==4)
{
  output_file<<"opcode error";
}
 if(x==5)
  output_file<<"operand error";
  if(x==3 && y==4)
  output_file<<"time limit exceeded and opcode error";
  if(x==3 && y==5)
output_file<<"time limit exceeded and operand error";

}


void InteruptServiceRoutine()
{
if(TI==0 && SI==1)
execute_GD();
if(TI==0 && SI==2)
execute_PD();
if(TI==0 && SI==3)
{
//return;
//exit(0);
//check();
terminate(0);
cpu.display();
ram.display();
exit(0);
}
if(TI==2 && SI==1)
{
//return;
//exit(0);
//check();
terminate(3);
cpu.display();
ram.display();
exit(0);
}

//terminate(3);
if(TI==2 && SI==2)
{
execute_PD();
terminate(3);
cpu.display();
ram.display();
exit(0);
}
if(TI==2 && SI==3)
{
terminate(0);
cpu.display();
ram.display();
exit(0);
}
if(TI==0 && PI==1)
terminate(4);
if(TI==0 && PI==2)
terminate(5);
if(TI==2 && PI==1)
{
terminate(3,4);
cpu.display();
ram.display();
exit(0);
}
if(TI==2 && PI==2)
{
terminate(3,5);
cpu.display();
ram.display();
exit(0);
}
PI=0;
SI=0;
TI=0;
}
void startSImulaTIon()
{
	while(1){

	load_IR();
	TTC++;
		if(TTC>TTL)
		{
			TI=2;
		}

	if (cpu.IR[0]=='G' && cpu.IR[1]=='D'){
		SI=1;	}
	else if(cpu.IR[0]=='P' && cpu.IR[1]=='D'){
		SI=2;	}
	else if(cpu.IR[0]=='L' && cpu.IR[1]=='R'){
		execute_LR();
	}
	else if(cpu.IR[0]=='S' && cpu.IR[1]=='R'){
		execute_SR();
	}
	else if(cpu.IR[0]=='C' && cpu.IR[1]=='R'){
		execute_CR();
	}
	else if(cpu.IR[0]=='B' && cpu.IR[1]=='T'){
		execute_BT();
	}
	else if(cpu.IR[0]=='H' && cpu.IR[1]==' '){
		SI=3;
		exit(0);
	}
	else{
		PI=1;
	}

	InteruptServiceRoutine();

	}
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
srand((int)time(0));
PTBR = getrandom();//0-29 available
cout<<"\n PTBR Frame no="<<PTBR;
for(int j=0;j<10;j++)
	{
	int base = PTBR*10;
	ram.M[base+j][0]='*';
	}
cout<<"\n";
for(int i=0;i<30;i++)
cout<<"available["<<i<<"]="<<available[i]<<"\t\t";
load_program();
cpu.IC=0;
startSImulaTIon();
cpu.display();
ram.display();
for(int i=0;i<30;i++)
cout<<"available["<<i<<"]="<<available[i]<<"\t\t";

input_file.close();
output_file.close();
return 0;
}
