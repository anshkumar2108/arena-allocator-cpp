/*
3. The Dangling Pointer Test
Understanding lifetime is critical before we get to Object Lifetime (Day 4).

Your Task:

Write a function that creates an integer on the stack, assigns it a value, and returns a pointer to that integer.

In your main() function, catch that returned pointer and try to print its value.

Note: Your compiler (like GCC or MSVC) should yell at you with a warning. If it runs, it might print garbage, or it might crash. This proves that stack memory is destroyed the moment the function ends.*/
#include<iostream>
#include<cstddef>
int *createDanglingPtr(){
    int local_var=43;
    return &local_var;
}
int main(){
    int*dangling_ptr=createDanglingPtr();
    std::cout<<"Dangling pointer value: "<<*dangling_ptr<<std::endl;
}