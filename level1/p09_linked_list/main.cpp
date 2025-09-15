#include <iostream>
using namespace std;

typedef struct Node {
    int data;
    Node* next;    
}Node;

Node* NCreate(int data)
{
    Node* p=(Node*)malloc(sizeof(Node));
    p->data=data;
    p->next=NULL;
    return p;
}
void LAppend(Node* head,int data)
{
    Node *p=head;
    while(p->next!=NULL)
    {
        p=p->next;
    }
    p->next=NCreate(data);
}
void LInsert(Node* head,int n,int data)
{
    Node* p=head;
    for(int i=0;i<n-1;i++)
    {
        if(p->next==NULL)
        {
            return;
        }
        p=p->next;
    }
    Node* temp=p->next;
    p->next=(Node*)malloc(sizeof(Node));
    p=p->next;
    p->data=data;
    p->next=temp;
}

Node* LReverse(Node* head)
{
    Node*p1=NULL,*p2=head,*p3=head->next;
    while(1)
    {
        p2->next=p1;

        if(p3->next==NULL)
        {
            p3->next=p2;
            p2=p3;
            break;
        }
        p1=p2;
        p2=p3;
        p3=p3->next;
    } 
    return p2;
}
int LCheck(Node* head,int num)
{
    Node* p=head;
    int n=0;
    while(1)
    {
        n++;
        if(p->data==num)
        {
            break;
        }
        p=p->next;

        if(p->next==NULL)
        {
            return -1;
        }
    }
    return n;
}
int LChecknext(Node* head,int num)
{
    Node* p=head;
    int n=0;
    while(1)
    {
        n++;
        if(p->next->data==num)
        {
            break;
        }
        
        if(p->next->next==NULL)
        {
            return -1;
        }
        p=p->next;
    }
    return n;
}
void LPrint(Node* head )
{
    Node*p=head;
    while(p->next!=NULL)
    {
        cout<<p->data;
        p=p->next;
    }
    cout<<p->data;
}
int main() {
    Node* head=NCreate(1);
    LAppend(head,2);
    LAppend(head,3);
    LInsert(head,3,4);
    LAppend(head,5);
    head=LReverse(head);
    LPrint(head);
    cout<<endl;
    cout<<LCheck(head,0)<<endl;
    cout<<LChecknext(head,1);
    return 0;
}