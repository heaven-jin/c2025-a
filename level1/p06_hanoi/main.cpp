#include "d.h"
extern void move(char begin,char end,int number,string *p);
int main() 
{

    string s;
    move('a','b',3,&s);
    cout<<s;
    return 0;
}