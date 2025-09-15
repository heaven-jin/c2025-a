#include <iostream>
#include <string>
using namespace std;
string encrypt(string s)
{
    for(char c:s)
    {
        c+=1;
    }
    return s;

}
string decrypt(string s)
{
    for(char c:s)
    {
        c-=1;
    }
    return s;
}
int main()
{
    string s;
    cout<<"input a string: "<<endl;
    cin>>s;
    cout<<"string by encrypting:"<<encrypt(s)<<endl;
    cout<<"decrypt the string :"<<decrypt(encrypt(s));
    return 0;
}


/*
string encrypt(string s)
{
    string s1;
    for (int i = 0; i <(3 -(int)(s.size()%3)%3); ++i) {
        s.push_back(0x00);  
    }
    for(int i=0;i<s.size()/3;i++)
    {
        char a=s[i],b=s[i+1],c=s[i+2];
        s1.push_back((a&0xfc)>>2);
        s1.push_back(((a&0x3)<<4)+((b&0xf0)>>4));
        s1.push_back(((b&0x0f)<<2)+((c&0xc0)>>6));
        s1.push_back(c&0x3f);
        
    }

    s1.push_back('0'+(int)(s.size()%3)%3);
    return s1;
}

string decrypt(string s)
{
    string s1;
    int pad_count = s.back()-'0';
    s.pop_back();
    for(int i=0;i<s.size()/4;i++)
    {
        char a=s[i],b=s[i+1],c=s[i+2],d=s[i+3];
        s1.push_back(((a&0xfc)<<2)+((b&0x30)>>4));
        s1.push_back(((b&0x0f)<<4)+((c&0x03c)>>2));
        s1.push_back(((c&0x03)<<6)+(d&0x3f));
    }
    if (pad_count > 0 && pad_count <= s1.size()) {
        s1.erase(s1.end() - pad_count, s1.end());
    }
    return s1;
}

int main()
{

    string s;
    cout << "请输入字符串：";
    cin.ignore();
    getline(cin, s);

    string encrypted = encrypt(s);
    string decrypted = decrypt(encrypted);
    cout << "\n加密结果：" << encrypted << endl;
    cout << "解密结果：" << decrypted << endl;
    if (s == decrypted) {
        cout << "\n加解密成功！" << endl;
    } else {
        cout << "\n加解密失败！" << endl;
    }
    return 0;
}
*/


