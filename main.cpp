#include <iostream>
#include <memory>
#include <algorithm>
#include <set>
#include <string>
#include <vector>
#include <functional>

#include "linked_ptr.hpp"

using namespace smart_ptr;


struct A {
    A() {
        a = 0;
    }
    int a;
};

struct B : A {
    B() {
        a = 1;
    }
};

bool base_test() {
    bool check = true;

    linked_ptr<int> ptr1(new int(4));
    linked_ptr<int> ptr2(new int(4));

    linked_ptr<int> ptr3(ptr1);

    check *= ptr2.unique();
    check *= !ptr1.unique();

    check *= (ptr1 == ptr3);

    return check;
}

bool del_test() {
    bool check = true;
    linked_ptr<std::vector<int>> vptr(new std::vector<int>{1, 2, 3, 4});
    linked_ptr<std::vector<int>> v2ptr(vptr);

    check *= !v2ptr.unique();

    vptr.~linked_ptr<std::vector<int>>();
    std::vector<int> v = *v2ptr;

    check *= (v[0] == 1);
    check *= v2ptr.unique();

    v2ptr.reset();
    check *=  (v2ptr.get() == nullptr);

    return check;
}

bool reset_test() {
    bool check = true;

    linked_ptr<double> ptr1(new double(3.6));
    linked_ptr<double> ptr2(new double(3.6));

    check *= (ptr1.get() != ptr2.get());

    linked_ptr<double> nptr1(ptr1);
    (*nptr1) = 4;
    check *= (*ptr1 == 4);

    nptr1.reset(new double(3.6));

    check *= (*nptr1 == 3.6);

    return check;
}

bool correct_pointer_test() {
//    bool check = true;
//    int *iptr = new int(3);
//    linked_ptr<int> p1(iptr);
//    p1.~linked_ptr<int>();
//    check *= (*iptr != 3);
//    return check;
}

bool test() {
    linked_ptr<A> ptr(new B());
    bool check = (ptr->a == 1);
    return check;
}

bool set_test() {
    std::set<linked_ptr<int>> S;
    linked_ptr<int> p1(new int(1));
    linked_ptr<int> p2(new int(2));
    linked_ptr<int> p3(new int(3));
    S.insert(p1);
    S.insert(p2);
    S.insert(p3);
    return true;
}


bool unique_test() {
    bool check = true;
    linked_ptr<int> p1(new int(1));
    linked_ptr<int> p2(new int(2));
    linked_ptr<int> p3(new int(3));

    check *= (p1.unique() && p2.unique() && p3.unique());
    linked_ptr<int> p4 = p3;
    check *= !p3.unique() && !p4.unique();
    p3.reset(new int(5));
    check *= p3.unique() && p4.unique();
    return check;
}

namespace unique2 {
    struct A {
        A(int a1) :a(a1) {}
        int a;
    };

    struct B : A {
        B(int a1, int b1) : A(a1), b(b1) {}
        int b;
    };
}

bool unique2_test() {
    bool check = true;
    linked_ptr<unique2::B> p1(new unique2::B(2, 3));
    linked_ptr<unique2::A> p2(p1);
    check *= !p1.unique();
    check *= !p2.unique();
    return check;
}

bool test_bool() {
    linked_ptr<int> lp(new int(4));
    linked_ptr<int> lp2;
    if (!lp)
        return false;
    if (lp2)
        return false;

    return true;
}

bool test_swap() {
    int *a0 = new int(1);
    int *b0 = new int(2);

    linked_ptr<int> a1(a0);
    linked_ptr<int> a2(a1);
    linked_ptr<int> a3(a2);

    linked_ptr<int> b1(b0);
    linked_ptr<int> b2(b1);
    linked_ptr<int> b3(b2);

    a2.swap(b2);
    b3.swap(a2);
    b1.swap(a1);

    a3.reset();
    b2.reset();
    b1.reset();

    return !(*a0 == 1 || *b0 != 2);
}

bool test_swap_2() {
    int *a0 = new int(1);
    int *b0 = new int(2);

    linked_ptr<int> a1(a0);

    linked_ptr<int> b1(b0);
    linked_ptr<int> b2(b1);
    linked_ptr<int> b3(b2);


    return (*a1 == 1 && *b2 == 2);
}


bool make_linked_test() {
//    auto p = make_linked_ptr(new int(3));
//    linked_ptr<int> p1(p);
//    p.reset(new int(1));
//    if (!p.unique() || !p1.unique()) return false;
//    return !(*p != 1 || *p1 != 3);
}

bool total;

void run_test(bool f(), std::string name) {
    std::cout << name << std::endl;
    try {
        if (f()) {
            std::cout << "ok" << std::endl;
        } else {
            total = false;
            std::cout << "fail" << std::endl;
        }
    } catch (std::exception &e) {
        total = false;
        std::cout << e.what() << std::endl;
    }
}

int main() {

    total = true;

    run_test(&base_test, "base_test");

    run_test(&del_test, "del_test");

    run_test(&reset_test, "reset_test");

    run_test(&correct_pointer_test, "correct_pointer_test");

    run_test(*&test, "test");

    run_test(&set_test, "set_test");

    run_test(&unique_test, "unique_test");

    run_test(&unique2_test, "unique2_test");

    run_test(test_bool, "test_bool");

    run_test(&test_swap, "test_swap");

    run_test(&test_swap_2, "test_swap_2");

    run_test(&make_linked_test, "make_linked_test");

    std::cout << std::endl;
    if (total) {
        std::cout << "Tests passed" << std::endl;
    } else {
        std::cout << "Tests failed" << std::endl;
    }

    return 0;
}