#include <stdio.h>
#include <sys/time.h>
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

inline void COMPUTE()
{

    for(int i=2;i<1001;i++)
    {
            if(Isprime(i)==1){
            std::cout<<i<<",";
        }

    }

}
int main() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    
    
    COMPUTE();
    
    
    gettimeofday(&end_time, NULL);
    long long total_us = (end_time.tv_sec - start_time.tv_sec) * 1000000LL + (end_time.tv_usec - start_time.tv_usec);
    cout<<"the cost time:"<<total_us<<"us";
    return 0;
}