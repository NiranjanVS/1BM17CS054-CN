#include<iostream>
#include<unistd.h>
#include<stdlib.h>
using namespace std;
#define bucketSize 512
void bktInput(int a,int b) {
if(a>bucketSize)
cout<<"\n\t\tBucket overflow";
else {
usleep(500);
while(a>b){
cout<<"\n\t\t"<<b<<" bytes outputted.";
a-=b;
usleep(500);
}
if (a>0) cout<<"\n\t\tLast "<<a<<" bytes sent\t";
cout<<"\n\t\tBucket output successful"<<endl;
}
}
int main() {
int op, pktSize;
cout<<"Enter output rate : "; cin>>op;
for(int i=1;i<=3;i++){
usleep(rand()%1000);
pktSize=rand()%1000;
cout<<"\nPacket no "<<i<<"\tPacket size = "<<pktSize;
bktInput(pktSize,op);
}
return 0;
}