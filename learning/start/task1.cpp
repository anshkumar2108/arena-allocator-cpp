#include<iostream>
#include<cstddef>
int main(){
    //Allocating 100 byte blocks
    std::byte* raw_memory=new std::byte[100];
    void*ptr=raw_memory;
    //Print the original address.
    std::cout<<"Initial Address: "<<ptr<<std::endl;//Compiler treats it as hex value.
    // 3. Advance the pointer by 16 bytes.
    // You CANNOT do `ptr + 16` directly because the compiler doesn't know 
    // the size of `void`. We must cast it to a type with a size of exactly 1.
    std::byte* byte_ptr=static_cast<std::byte*>(ptr);
    byte_ptr+=16;
    //Convert back to void ptr.
    std::cout<<"Byte address after incrementing: "<<byte_ptr<<std::endl;
    ptr=static_cast<void*>(byte_ptr);
    std::cout<<"Void address after incrementing: "<<ptr<<std::endl;
    delete[] raw_memory;
    return 0;

}