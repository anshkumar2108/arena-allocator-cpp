#include <iostream>
#include <chrono>
#include <list>
#include "../include/Arena.hpp"
#include "../include/PoolAllocator.hpp"
#include "../include/Allocator.hpp"
int main(){
    //OS and CPU Cache warmup.
    //Not timed.
    {
        Arena arena(256*1024);
        PoolAllocator<24,10000>pool(arena);
        Allocator<int,PoolAllocator<24,10000>>alloc(pool);
        std::list<int, Allocator<int, PoolAllocator<24, 10000>>>v(alloc);
        for(int i=0;i<10000;i++){
            v.push_back(i);
        }
    }
    std::cout<<"Benchmarking tests starts\n";
    std::cout<<"Custom allocators test started: \n";
    //Now test starts.
    auto start_custom=std::chrono::high_resolution_clock::now();
    {
        Arena arena(256*1024);
        PoolAllocator<24,10000>pool(arena);
        Allocator<int,PoolAllocator<24,10000>>alloc(pool);
        std::list<int, Allocator<int, PoolAllocator<24, 10000>>>v(alloc);
        for(int i=0;i<10000;i++){
            v.push_back(i);
        }
    }
    auto end_custom=std::chrono::high_resolution_clock::now();
    auto duration_custom=std::chrono::duration_cast<std::chrono::microseconds>(end_custom-start_custom);
    std::cout<<"Custom Allocators test ended.\n";
    std::cout<<"Default Allocators test started.\n";
    auto start_default=std::chrono::high_resolution_clock::now();
    {
        std::list<int>l;//Empty list size not specified if specified it will create list with 10000 elements.
        for(int i=0;i<10000;i++){
            l.push_back(i);
        }
    }
    auto end_default=std::chrono::high_resolution_clock::now();
    auto duration_default=std::chrono::duration_cast<std::chrono::microseconds>(end_default-start_default);
    std::cout<<"Default Allocators test ended.\n";
    std::cout<<"All lists operations completed successfully.\n";
    std::cout<<"Custom Allocators Time taken: "<<duration_custom.count()<<" microseconds.\n";
    std::cout<<"Default Allocators Time taken for 10000 insertions: "<<duration_default.count()<<" microseconds.\n";
}
