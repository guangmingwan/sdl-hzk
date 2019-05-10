#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

//获取中文字段的GBK编码值
vector<unsigned char> encode(const string s)
{
    string str = s;
    vector<unsigned char> buff;

    int len = str.length();
    buff.resize(len);
    memcpy(&buff[0], str.c_str(), len);

    return buff;
}

//把GBK编码值解码成中文字段
string decode(vector<unsigned char> buff)
{
    vector<unsigned char>debuf = buff;
    debuf.resize(buff.size() + 1);
    string str = (char *)&debuf[0];
    return str;
}

int main() {
    //编码
    vector<unsigned char>str;
    string s = "询";
    str = encode(s);
    printf("gbk编码值：");
    for (int i = 0; i < str.size(); i++) {
        printf("%X ", str[i]);
    }
    printf("\n");

    //解码
    string k2 = decode(str);
    printf("中文值：");
    printf("%s\n", k2.c_str());

    system("pause");
    return 0;
}
