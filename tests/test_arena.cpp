#include <iostream>
#include <cassert>
#include <new>
#include "../include/Arena.hpp"
struct Vec3
{
    float x, y, z;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    ~Vec3() { std::cout << "~Vec(" << x << ")\n"; }
};
int main()
{
    Arena arena(1024);
    std::cout << "Capacity: " << arena.capacity() << "\n";
    std::cout << "Used: " << arena.used() << "\n";

    // allocate a vec3 using placement new
    void *raw = arena.allocate(sizeof(Vec3), alignof(Vec3));
    Vec3 *v1 = new (raw) Vec3(1.0f, 2.0f, 3.0f);
    std::cout << "v1 = (" << v1->x << ", " << v1->y << ", " << v1->z << ")\n";
    std::cout << "Used after v1: " << arena.used() << "\n";

    // allocate second vec3
    void *raw2 = arena.allocate(sizeof(Vec3), alignof(Vec3));
    Vec3 *v2 = new (raw2) Vec3(4.0f, 5.0f, 6.0f);
    std::cout << "v2 = (" << v2->x << ", " << v2->y << ", " << v2->z << ")\n";
    std::cout << "used after v2: " << arena.used() << "\n";

    // manually destruct before reset
    v1->~Vec3();
    v2->~Vec3();

    // reset and reuse
    arena.reset();
    std::cout << "used after reset: " << arena.used() << "\n";
    assert(arena.used() == 0);

    Arena arena2=std::move(arena);
    std::cout<<"arena2 capacity: "<<arena2.capacity()<<"\n";

    //test bad alloc when arena is exhausted.
    try{
        Arena small(8);
        small.allocate(1024,1);//Should throw error.
        assert(false&&"should have thrown");
    }catch(const std::bad_alloc&){
        std::cout<<"bad_alloc caught correctly.\n";
    }
    std::cout<<"All tests passed correctly.\n";
}
