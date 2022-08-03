#include<iostream>
#include<string>

using namespace std;


struct WifiConfigType
{
  char stassid[32]; //  定义配网得到的WIFI名长度(最大32字节)
  char stapsw[64];  //  定义配网得到的WIFI密码长度(最大64字节)
};

int main() {
  string a = "qwe";
  string b = "";
  for (int i = 0; i < 2; i++) {
    b+=a[i];
  }
  b+="z";
  b+="z";
  b+="z";
  cout << sizeof(b) << endl;
  cout << b.length() << endl;
  bool b1 = b[0] == 'q';
  bool b2 = b[34] == '\0';
  cout << b1 << endl;
  cout << b2 << endl;
  cout << b[34] << endl;
  cout << b << endl;
  b+="z";
  b+="z";
  b+="z";
  cout << b << endl;
  return 0;
}