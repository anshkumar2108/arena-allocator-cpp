#include<cstddef>
#include<cassert>
#include"../include/Allocator.hpp"
#include"../include/PoolAllocator.hpp"
#include"../include/Arena.hpp"
#include<vector>
#include<list>
#include<iostream>
int main(){
    Arena arena(4096);
    PoolAllocator<24,64>pool(arena);
    Allocator<int,PoolAllocator<24,64>>alloc(pool);
    std::list<int, Allocator<int, PoolAllocator<24, 64>>>v(alloc);
    v.push_back(4);
    v.push_back(2);
    v.push_back(1);
    std::cout<<"All tests passed correctly.\n";

}
