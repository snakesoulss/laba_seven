#include <iostream>
#include <algorithm>
#include "List.h"
using namespace std;


template <class T> class numerate
{
public:
    numerate(T start = 0, T _shift = 1) : seed(start), shift(_shift) {};
    T operator()() { T old = seed; seed += shift; return old; }

private:
    T seed, shift;
};

template <class T, class C> T sum(const C& c)
{
    T res = 0;
    for (typename C::Const_iterator i = c.cbegin(); i != c.cend(); ++i)
        res += *i;
    return res;
}

int main()
{
    Queue<int> s1, s2;
    numerate<int> f(100);

    s1.Push(1);
    s1.Push(2);
    s1.Push(3);

    s2.Push(5);
    s2 = s1;
    s2.Push(4);

    cout << s1 << endl;          // 1->2->3
    cout << s1.Size() << endl;

    cout << s2 << endl;          // 1->2->3->4
    cout << s2.Size() << endl;

    swap(s1, s2);

    cout << s1 << endl;          // 1->2->3->4
    cout << s1.Size() << endl;

    cout << s2 << endl;          // 1->2->3
    cout << s2.Size() << endl;

    generate(s1.begin(), s1.end(), f);

    cout << s1 << endl;          // 100->101->102->103
    cout << s1.Size() << endl;

    cout << sum<int,List<int>>(s2) << endl;     // 6

    return 0;
}
