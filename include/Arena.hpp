#pragma once
#include<cstddef>  //std::byte, size_t
#include<memory>  //std::align
#include<stdexcept>//std::bad_alloc
#include<cassert>//assert

class Arena{
    public:
    //Constructor- allocates the backing memory block
    explicit Arena(size_t size)
    : m_size(size),
    m_buffer(new std::byte[size]),
    m_current(m_buffer)
    {}
    //Destructor
    ~Arena(){
        delete[]m_buffer;
    }

    //Rule of five: Move only no copy as it owns raw memory.
    Arena(Arena&&other)noexcept
    : m_size(other.m_size),
    m_buffer(other.m_buffer),
    m_current(other.m_current){
        other.m_buffer=nullptr;
        other.m_current=nullptr;
        other.m_size=0;
    }

    Arena&operator=(Arena&&other)noexcept{
        if(this==&other)return *this;
        delete[]m_buffer;//delete the old data to store new
        m_buffer=other.m_buffer;
        m_current=other.m_current;
        m_size=other.m_size;
        other.m_current=nullptr;
        other.m_buffer=nullptr;
        other.m_size=0;
        return *this;
    }

    Arena(const Arena&)=delete;
    Arena&operator=(const Arena&)=delete;

    //Core allocation: Bump pointer with alignment

    void *allocate(size_t bytes,size_t alignment){
        //remaining space in the buffer.
        size_t space=remaining();
        void*ptr=m_current;

        //std::algin adjusts pointer to the next aligned address.
        //Reduces space by padding the bytes added.
        if(!std::align(alignment,bytes,ptr,space)){
            throw std::bad_alloc{};//Not enough space.
            //It's the standard C++ exception for memory allocation failure — the same one new throws when the system runs out of memory. By throwing it ourselves when the arena is full, our allocator behaves consistently with how the rest of C++ signals allocation failure.
        }

        //advance bump pointer past the newly allocated region.
        m_current=static_cast<std::byte*>(ptr)+bytes;
        return ptr;
    }

    //Reset- wipe all allocations, reuse the buffer
    //All objects that were alive in this arena must be manually destructed before calling reset()

    void reset() noexcept{
        m_current=m_buffer;
    }

    //The arena guarantees nothing about object lifetimes. You are the one responsible for the ordering: destruct everything first, then reset.
    
    //Diagnostics - useful for benchmarks and tests.

    size_t used() const noexcept{return static_cast<size_t>(m_current-m_buffer);}
    size_t remaining() const noexcept {return m_size-used();}
    size_t capacity()const noexcept{return m_size;}

    private:
    size_t m_size;
    std::byte* m_buffer;
    std::byte* m_current;//bump pointer.
};
