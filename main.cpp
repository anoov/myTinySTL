#include <iostream>
#include "myVector.h"
using namespace std;
int main() {
    Vector<int> a(10, 1);
//    a.pop_back();
//    cout << a.capacity() << endl;
    a.push_back(20);
    cout << a.empty() << endl;
    auto iter = a.begin();
    for (; iter != a.end(); ++iter)
        cout << *iter << endl;
    return 0;
}
