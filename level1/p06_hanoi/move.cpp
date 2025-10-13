#include "d.h"
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