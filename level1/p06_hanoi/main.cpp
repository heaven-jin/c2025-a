#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <string>
using namespace std;
void move(char begin,char end,int number,string *p)
{
    if(number!=1)
    {
        move(begin,'a'+'b'+'c'-end-begin,number-1,p);
        p->push_back(begin);
        p->append(" -> ");
        p->push_back(end);
        p->push_back('\n');
        
        //cout<<begin<<" -> "<<end<<endl;
        move('a'+'b'+'c'-end-begin,end,number-1,p);
    }else{
        p->push_back(begin);
        p->append(" -> ");
        p->push_back(end);
        p->push_back('\n');
        //cout<<begin<<" -> "<<end<<endl;
        return;
    }
}
int main() 
{
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    
    string s;
    move('a','b',16,&s);
    cout<<s;
    
    gettimeofday(&end_time, NULL);
    long long total_us = (end_time.tv_sec - start_time.tv_sec) * 1000000LL + (end_time.tv_usec - start_time.tv_usec);
    cout<<"the cost time:"<<total_us<<"us";

    return 0;
}