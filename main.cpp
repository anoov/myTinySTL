#include <iostream>
#include "myVector.h"
#include "myList.h"
using namespace std;
int main() {
    myList<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);
    auto iter = a.begin();
    for (; iter != a.end(); ++iter) {
        cout << *iter << endl;
    }
    return 0;
}
