#include<iostream>
#include<cassert>
#include"../include/Arena.hpp"
#include"../include/PoolAllocator.hpp"

struct Particle{
    float x,y,z;
    int id;
    Particle(float x,float y,float z,int id):x(x),y(y),z(z),id(id){
        std::cout<<"Particle("<<id<<") constructed.\n";
    }
    ~Particle(){
        std::cout<<"Particle("<<id<<") destructed.\n";
    }
};
int main(){
    //Arena owns 4KB of raw memory.
    Arena arena(4096);

    //Pool carves it into 8 blocks of Particle size
    PoolAllocator<sizeof(Particle),8>pool(arena);

    //Allocate 3 particles
    void*raw1=pool.allocate();
    void*raw2=pool.allocate();
    void*raw3=pool.allocate();
    Particle* p1=new(raw1) Particle(1,2,3,4);
    Particle* p2=new(raw2) Particle(4,5,6,2);
    Particle* p3=new(raw3) Particle(7,8,9,3);
    std::cout<<"p1.id="<<p1->id
    <<" p2.id="<<p2->id<<" p3.id="<<p3->id<<"\n";

    //Free p2, destruct first then return to pool
    p2->~Particle();
    pool.deallocate(p2);
    p2=nullptr;

    void*raw4=pool.allocate();
    Particle*p4=new(raw4) Particle(10,11,12,4);
    std::cout<<"p4 reused slot: "<<((raw4==raw2)?"YES":"NO")<<"\n";

    //Cleanup- destruct in any order and return to pool
    p1->~Particle();pool.deallocate(p1);
    p3->~Particle();pool.deallocate(p3);
    p4->~Particle();pool.deallocate(p4);

    //Test exhaustion
    try{
        PoolAllocator<sizeof(Particle),2>small_pool(arena);
        void*a=small_pool.allocate();
        void*b=small_pool.allocate();
        (void)a;// tells compiler: yes I know a is unused, that's intentional
        (void)b;
        small_pool.allocate();//should throw
        assert(false&&"should have thrown");
    }catch(const std::bad_alloc&){
        std::cout<<"bad_alloc on exhaustion caught correctly\n";
    }
    std::cout<<"All pool tests passed.\n";

}
