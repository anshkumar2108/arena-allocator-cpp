#pragma once
#include<cassert>
#include<cstddef>
#include"../include/Arena.hpp"
#include"../include/PoolAllocator.hpp"
template<typename T,typename Pool>
struct Allocator{
    using value_type=T;
    Pool* pool_;
    Allocator(Pool&p):pool_(&p){}
    T*allocate(std::size_t n){
        assert(n==1);//Gives one block at a time.
        return static_cast<T*>(pool_->allocate());
    }
    void deallocate(T*p,std::size_t){
        pool_->deallocate(p);
    }
    template<typename U>
    Allocator(const Allocator<U,Pool>&other):pool_(other.pool_){}//Rebind constructor
    template<typename U>
    struct rebind{
        using other=Allocator<U,Pool>;
    };
};