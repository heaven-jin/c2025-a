#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
using namespace std;

int buffer[10][10]=
{
  {2,2,2,2,2,2,2,2,2,2},
  {2,0,0,0,2,0,0,0,0,2},
  {2,0,2,0,2,0,0,0,0,2},
  {2,0,2,0,2,0,0,0,0,2},
  {2,0,2,0,2,0,2,0,0,2},
  {2,0,2,0,0,0,2,0,0,2},
  {2,0,2,2,2,2,2,0,0,0},
  {2,0,2,0,0,0,0,0,0,2},
  {2,0,0,0,0,0,2,0,0,2},
  {2,2,2,2,2,2,2,2,2,2}
};

bool isSuccess(int x,int y)
{
  if((x==10 || y==10 || x==10 || y==10))
  {
    return true;
  }
  return false;
}

void Render(int *buffer,int &x,int &y)
{
  system("cls");
  for(int i=0;i<10;i++)
  {
    for(int j=0;j<10;j++)
    {
      if(j==x-1 && i==y-1)
      {
        cout<<1;
      }
      else{
        cout<<*(buffer+i*10+j);
      }
    }
    cout<<endl;

  }
}
void Logic(int &x,int &y,int *buffer,char &input)
{
  cin>>input;
  switch(input)
  {
    case 'a':
    if(*(buffer+(y-1)*10+x-2)==2)
    {
      break;
    }
    x-=1;
    break;
    case 'd':
    if(*(buffer+(y-1)*10+x)==2)
    {
      break;
    }
    x+=1;
    break;
    case 'w':
    if(*(buffer+(y-2)*10+x-1)==2)
    {
      break;
    }
    y-=1;
    break;
    case 's':
    if(*(buffer+(y-2)*10+x-1)==2)
    {
      break;
    }
    y+=1;
    break;  
  }
  
}
int main()
{
  int x=2,y=4;
  char input;
  while(!isSuccess(x,y))
  {
    cout<<x<<" "<<y<<endl;  
    Render((int *)buffer,x,y);
    Logic(x,y,(int *)buffer,input);


  }
  cout<<"success"<<endl;
  system("pause");
}