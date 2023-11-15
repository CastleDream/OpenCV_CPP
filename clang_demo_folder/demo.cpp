#include <iostream>
#include <vector>
#include <string>
// 命令行运行
// clang++ demo.cpp -o demo
// ./demo
using namespace std;

int main()
{
    vector<string> msg;
    msg.push_back("Hello");
    msg.push_back("C++");
    msg.push_back("World");
    msg.push_back("from");
    msg.push_back("VS code");
    msg.push_back("and the C++ extension!");

    for (int i = 0; i < msg.size(); i++)
    {
        cout << msg[i] << " ";
    }
    cout << endl;
    return -1;
}
