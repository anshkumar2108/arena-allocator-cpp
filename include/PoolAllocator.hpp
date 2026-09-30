#pragma once
#include<cassert>
#include<cstddef> //size_t, std::byte
#include<stdexcept>//std::bad_alloc
#include "Arena.hpp"

template<size_t BlockSize,size_t BlockCount>
class PoolAllocator{
    public:
    //Each block must be large enough to hold 
    static constexpr size_t kBlockSize=BlockSize>=sizeof(void*)?BlockSize:sizeof(void*);
    
    //Constructor: Carves BlockCount blocks out of the arena
    explicit PoolAllocator(Arena& arena):m_freelist(nullptr){
        //Ask arena for one contiguous region big enough for all blocks.
        void*raw=arena.allocate(kBlockSize*BlockCount,alignof(std::max_align_t));

        //Build the freelist by walking through the region.
        //and linking each block to the next.
        auto*block=static_cast<std::byte*>(raw);
        for(size_t i=0;i<BlockCount;++i){
            auto* node=reinterpret_cast<FreeNode*>(block+i*kBlockSize);
            //point this node to the next one.
            node->next=m_freelist;
            m_freelist=node;
        }
    }
    //No move no copy- pool is tied to a specific arena region.
    PoolAllocator(const PoolAllocator&)=delete;
    PoolAllocator(PoolAllocator&&)=delete;
    PoolAllocator&operator=(const PoolAllocator&)=delete;
    PoolAllocator&operator=(PoolAllocator&&)=delete;
    
    //Allocate one block O(1);
    void*allocate(){
        if(!m_freelist)throw std::bad_alloc{};
        //Pop from front of freelist
        FreeNode*node=m_freelist;
        m_freelist=node->next;
        return node; //Caller will call their placement new here.
    }

    //Return one block to the pool.
    //Caller must have already called the object's destructor.
    void deallocate(void*ptr)noexcept{
        //push to front of freelist.
        auto*node=static_cast<FreeNode*>(ptr);
        node->next=m_freelist;
        m_freelist=node;
    }

    //Diagnostics and testing
    bool empty() const noexcept{return m_freelist==nullptr;}
    private:
    struct FreeNode{
        FreeNode*next;
    };
    FreeNode* m_freelist;

};