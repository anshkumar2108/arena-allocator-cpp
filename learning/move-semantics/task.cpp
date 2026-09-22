#include<iostream>
#include<cstddef>
#include<utility>//Move semantics.
#include<cstring>

class SmartBuffer{
    private:
    size_t size;
    std::byte*buffer;
    public:
    //Normal constructor
    SmartBuffer(size_t buffer_size){
        size=buffer_size;
        buffer=new std::byte[size];
        std::cout<<"[Constructor] Allocated "<<size<<" bytes.\n";
    }
    //Destructor
    ~SmartBuffer(){
        if(buffer!=nullptr){
            delete []buffer;
            std::cout<<"[Destructor] Freed "<<size<<" bytes\n";
        }else{
            std::cout<<"[Destructor] Buffer was empty, nothing to free.\n";
        }
    }

    //Copy constructor(Deep copy)
    //Triggered when you make normal copy of the object.
    SmartBuffer(const SmartBuffer&other){
        size=other.size;
        buffer=new std::byte[size];//Allocating the brand new memory.
        //We will use memcpy to copy the actual data.
        std::cout<<"[COPY Constructor] Made a heavy deep copy of "<<size<<"bytes\n";
    }
    // Copy Assignment operator
    // a=b; (both objects already exists)
    SmartBuffer&operator=(const SmartBuffer&other){
        if(this==&other)return *this;//self assignment guard
        delete[]buffer;
        size=other.size;
        buffer=new std::byte[size];
        std::memcpy(buffer,other.buffer,size);
        std::cout<<"[Copy Assign] Deep copied "<<size<<" bytes.\n";
        return *this;
    }
    //Move constructor
    //Binds to value : SmartBuffer c=std::move(a);
    //Triggered when using std::move() or returning a temporary object.
    //The noexcept keyword is critical for systems programming optimization
    SmartBuffer(SmartBuffer&&other)noexcept{
        size=other.size;
        buffer=other.buffer;//Steal the pointer of the object.
        // Null out the old object so its destructor doesn't free our stolen memory
        other.size=0;
        other.buffer=nullptr;
        std::cout << "[MOVE Constructor] Stole pointer. No allocation performed.\n";
    }

    //Move Assignment Operator
    // a=std::move(b); (both objects already exists.)
    SmartBuffer &operator=(SmartBuffer&&other)noexcept{
        if(this==&other)return *this;
        delete[]buffer;
        buffer=other.buffer;
        size=other.size;
        other.size=0;
        other.buffer=nullptr;
        std::cout << "[Move Assign]      Stole pointer. No allocation.\n";
        return *this;
    }
    // SmartBuffer(const SmartBuffer&)=delete;//For exclusive ownership no copies.
    // SmartBuffer& operator=(const SmartBuffer&)=delete;


};

// swap using std::move — exactly from your notes
template<typename T>
void my_swap(T&x,T&y){
    T temp{std::move(x)}; // move constructor  — favours T(T&&)
    x=std::move(y); // move assignment   — favours T& operator=(T&&)
    y=std::move(temp); // move assignment   — favours T& operator=(T&&)
}
int main() {
    std::cout << "=== 1. Normal constructor ===\n";
    SmartBuffer A(1024);
 
    std::cout << "\n=== 2. Copy constructor (lvalue source) ===\n";
    SmartBuffer B = A;
 
    std::cout << "\n=== 3. Move constructor (rvalue source) ===\n";
    SmartBuffer C = std::move(A);   // A is now moved-from
 
    std::cout << "\n=== 4. Copy assignment (D already exists) ===\n";
    SmartBuffer D(512);
    D = B;
 
    std::cout << "\n=== 5. Move assignment (E already exists) ===\n";
    SmartBuffer E(256);
    E = std::move(B);               // B is now moved-from
 
    std::cout << "\n=== 6. swap using move semantics (from your notes) ===\n";
    SmartBuffer X(64), Y(128);
    my_swap(X, Y);
    std::cout << "Swap complete — 3 move ops, 0 allocations.\n";
 
    std::cout << "\n=== End of scope — destructors fire in reverse construction order ===\n";
}