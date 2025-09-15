#include <cmath>
#include <memory.h>
#include <iostream>
using namespace std;

inline bool Isprime(int n)
{
    bool Isprime=1;
    for(int i=1;i<sqrt(n);)
    {
        i++;
        if(n%i==0 && n!=i)
        {
            Isprime=0;
            break;
        }
    }
    return Isprime;
}

inline void COMPUTE(bool* num)
{

    for(int i=2;i<101;i++)
    {
        if(Isprime(i)==1){
            num[i-1]=1;
        }

    }

}
void Print_array(bool *num)
{
    for(int i=0;i<100;i++)
    {
        cout<<num[i]<<" ";
    }
    cout<<endl;

}
void Check(bool *num)
{
    bool IsTrue=1;
    for(int i=0;i<99;i++)
    {
        num[i]==0? IsTrue=0:NULL;
    }
    if(IsTrue==1)
    {
        cout<<"true"<<endl;
    }else{
        cout<<"False"<<endl;
    }

}
int main() {
    bool num[100];
    memset(num,0,sizeof(num));
    COMPUTE(num);

    Print_array(num);

    for(int i=0;i<100;i++)
    {
        for(int j=i;j<100-i;j++)
        {
            num[i+j]==1?num[i+j]=1:num[i+j]=num[i]*num[j];
        }
    }
    Print_array(num);
    Check(num+1);
    return 0;
}