#include<iostream>
#include<string.h>

using namespace std;

struct WifiConfigType
{
  char stassid[32];//定义配网得到的WIFI名长度(最大32字节)
  char stapsw[64];//定义配网得到的WIFI密码长度(最大64字节)
};

class PP {
  public:
    void show() {
      cout << "show" << endl;
    };
};

PP pp;
PP pp2 = PP();

void test(const char *str1, const char *str2) {
  WifiConfigType wc{"", ""};
  strcpy(wc.stassid, str1);
  cout << "123" << endl;
  cout << wc.stassid << endl;
  pp.show();
  pp2.show();
}

int main() {
  string s1 = "abc";
  string s2 = "def";
  char *str1 = (char*)s1.c_str();
  char *str2 = (char*)s2.c_str();
  test(s1.c_str(), s2.c_str());
  return 0;
}