#include<iostream>
//Non vacuous Initialization:
/*
-Classes with constructors
-Classes with virtual functions
*/

//Vacuous Initialization
/*
Classes with basic initial members such as int,float char.
*/

//When lifetime starts and finishes:
/*
The lifetime of an object of type T begins when:
-storage with the proper alignment and size for type T is obtained.
AND
-if the object has non vacuous initialization, its initialization is complete(constructor complete)

The lifetime of an object of type T ends when:
-if T is a class type with non-trivial destructor and destructor call starts.
OR
-the storage which the object occupies is released, or is reused by an object that is not nested within o.
*/
struct Foo{
    char r;
    Foo(){
        std::cout<<"Foo()"<<std::endl;
        throw std::runtime_error("Exception in constructor.");
    }
    Foo(char a){
        r=a;
        std::cout<<"Foo("<<r<<")"<<std::endl;
    }
    ~Foo(){std::cout<<"~Foo("<<r<<")"<<std::endl;}
};

struct Fooint{
    int m_i;
    Fooint(int i):m_i(i){std::cout<<"Fooint("<<m_i<<")"<<std::endl;}
    ~Fooint(){std::cout<<"~Fooint("<<m_i<<")"<<std::endl;}
};

//RAII usage: is tied with object lifetime.
/*
Heap memory allocation
Mutexes
Threads
File management

Already uses RAII:
std::unique_ptr, std::smart_ptr
std::lock_guard, std::shared_lock,std::unique_lock
std::jthread
std::ofstream* */ 
int main(){
    Foo a('a');//Foo()
    {
        Foo b('b');//Foo()
    }//~Foo()

    Foo*b=nullptr;//Pointer is an object that points to another object(in this case the other object not been initialized yet.)
    {
        b=new Foo('c');//Foo()
    }// Dynamic variable not going to happen anything here.(Foo still in memory.)
    delete b;//~Foo()


    //Constructors and destructors(3)
    try{
        Foo d;//We will see the message because we passed the message before exception.
        //But the fact is object is not initialized so destructor will not work.

    }catch(...){};


    //Fooint constructors.
    auto v1=Fooint(1);//Fooint(1)
    {
        auto v2=Fooint(2);//Fooint(2)
    }//Fooint(2)
    auto v3=Fooint(3);//Fooint(3)
    auto v4=new Fooint(4);//Fooint(4)
    return 0;
}//~Foo() :Fooint constructors in reverse : ~Fooint(3), ~Fooint(1); 