#include <stdlib.h>
#include <windows.h>
#include <iostream>
using namespace std;
void LOG(string S,int V)
{
    cout<<"LOG"<<S<<":"<<V<<endl;
    return void();

}
int main(){
    char C;
    cin>>C;
    int Pos=1;
    int V=1;
    cout<<"start"<<endl;
    while(1)
    {
        


        if(Pos==9 ^ Pos==0){
            V=V*(-1);
        }
        
        for(int i=0;i<10;i++){
        
            if(Pos==i){
                cout<<C;

            }else{
                cout<<" ";
            }
        }
        Pos=Pos+V;
        cout<<endl;
        Sleep(128);
        system("cls");


    }
    return 0;
        
        
    
}