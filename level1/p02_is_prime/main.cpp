#include <iostream>
#include <cmath>
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


int main() {
    cout<<"input a number:"<<endl;
    int n,isp=1;
    cin>>n;
    if(Isprime(n)==1 && n!=1){
        std::cout<<n<<" is a prime";}
    else
        {std::cout<<n<<" is not a prime";}
    return 0;
}