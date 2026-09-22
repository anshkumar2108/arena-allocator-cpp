#include <iostream>
#include <new>

// Three Syntax rules
/*
Rule1(Syntax): new(ptr) T(args); //construct a T at address ptr.

Rule2(Manually call the destructor): obj->~T(); //Ends lifetime, does not free memory.

Note: Never use delete on placement new object as you dont own the memory pool does.

Rule3: Memory must be alligned with T
alignas(T) std::byte buffer[sizeof(T)]; //correct

std::byte buffer[sizeof(T)]; //Wrong - may be misalligned.
*/

struct Sensor
{
    int id;
    Sensor(int i) : id(i)
    {
        std::cout << "Sensor(" << id << ") constructed\n";
    }
    ~Sensor()
    {
        std::cout << "Sensor(" << id << ") destructed\n";
    }
};
struct Motor{
    int rpm;
    Motor(int i):rpm(i){
        std::cout<<"Constructor("<<rpm<<") called.\n";
    }
    ~Motor(){
        std::cout<<"Destructor("<<rpm<<") called.\n";
    }
};
int main()
{
    // Step 1: obtain raw memory, no initialization, no object.
    alignas(Sensor) std::byte buffer[sizeof(Sensor)];

    // Step 2: Construct object in that memory.(lifetime starts)
    Sensor *s = new (buffer) Sensor(42);

    // Step 3 — end object lifetime manually (lifetime ends, memory still exists)
    s->~Sensor();

    // Step 4 — construct a NEW object in the same memory (reuse)
    s = new (buffer) Sensor(99);
    std::cout << "id = " << s->id << "\n";

    // Step 5 — end lifetime again before buffer goes out of scope
    s->~Sensor();

    // buffer destructs here — just raw bytes, no object lifetime issue

    // This is the core of a pool allocator — the pool owns the memory, you control when objects are born and die inside it.

    // WRONG — double destruction
    //Sensor *s = new (buffer) Sensor(42);
    //delete s; // WRONG: tries to free buffer's memory, which you don't own
              // also calls destructor — so destructor runs, then free crashes

    // CORRECT
    //ss->~Sensor(); // destructor only — no free
    // memory returned to pool manually



    alignas(Motor) std::byte buffer1[sizeof(Motor)];
    Motor*m=new(buffer1) Motor(3000);
    std::cout<<m->rpm<<std::endl;
    m->~Motor();
    m=new(buffer1) Motor(6000);
    std::cout<<m->rpm<<std::endl;
    m->~Motor();


}   