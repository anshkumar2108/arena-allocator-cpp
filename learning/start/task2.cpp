/*
2. The Reference vs. Pointer Reassignment Test
Ben Saks likely emphasized the difference between references and pointers, particularly regarding reassignment and nullability.

Your Task:
Write a quick test to solidify this rule: pointers can change who they point to; references cannot.

Create two integer variables on the stack (int a = 10; int b = 20;).

Create a pointer pointing to a. Change it to point to b.

Create a reference bound to a. Try to make it refer to b (you will see that ref = b just changes the value of a to 20, it doesn't change the reference itself).

Print the memory addresses of a, b, the pointer, and the reference to observe what happened.*/
#include<iostream>
#include<cstddef>
int main(){
    int a=20;
    int b=10;
    int*ptr=&a;
    std::cout<<"Current Ptr value points to: "<<*ptr<<std::endl;
    std::cout<<"Current pointer address: "<<ptr<<std::endl;
    ptr=&b;
    std::cout<<"Now Ptr value points to: "<<*ptr<<std::endl;
    std::cout<<"Pointer Address: "<<ptr<<std::endl;
    ptr=&a;
    int &ref=a;
    std::cout<<"Current reference refers to: "<<ref<<std::endl;
    std::cout<<"Before reference address: "<<&ref<<std::endl;
    ref=b;//Immovable pointer only values changes not address.
    std::cout<<"Now reference refers to: "<<ref<<std::endl;
    std::cout<<"After reference address: "<<&ref<<std::endl;
    return 0;

}

