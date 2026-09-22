#include<iostream>
struct Foo{};
struct Obj{
    Foo getFoo(){return Foo();}//Temporary is initialized directly in val's storage.
};

struct Bar{
    int m_i;
    Bar(){
        std::cout<<"Bar()"<<std::endl;
        m_i=3;

    }
};
const Bar& dangerous(){
    return Bar();
}
struct F{
    F(){
        std::cout<<"F()"<<std::endl;
    }
    const Bar&getBar(){
        return m_bar;
    }
    Bar m_bar;
};
int main(){
    Obj o;
    Foo val=o.getFoo();//Return value optimization.
    //Only one object is created here.

    F*f=new F();// Bar(), F()
    /*
    -In C++, when you create an object of a class or struct, its member variables are always initialized before the body of its own constructor executes.
    -This standard C++ initialization order guarantees that a class's internal members are ready to be used by the time its own constructor body runs.*/
    const Bar&b=f->getBar();
    auto i1=b.m_i;//OK
    delete f; //~F(), ~Bar()
    auto i2=b.m_i;//Undefined Behaviour.
    //Above these are dangling references.

    //Be careful with object whose lifetimes you dont know.

    //Reference lifetime extension
    /*
    struct Foo;
    struct Obj{
        Foo getFoo(){...}
    };
    int main(){
        Obj o;
        const Foo& val=o.getFoo();//Reference lifetime extension.
    }
    */

    //Temporary objects
    /*
    Temporary objects are destroyed as the last step of evaluating full expression that lexically contains the point where they were created.
    
    There are 3 contexts in which temporaries are destroyed at a different point than the end of full expression.

    The third context is when a reference is bound to a temporary.
    The temporary to which the reference is bound or the temporary that is the complete object of a subobject to which reference is bound persists for the lifetime of the reference except:

    -A temporary object bound to the reference parameter in a function call persists until the completion of the full expression containing the call.
    -The lifetime of a temporary bound to a returned value in a function return statement is not extended, the temporary is destroyed at the end of the full expression in the return statement.
    */
   const Bar&m=dangerous();//b will be dangling.
   std::cout<<m.m_i;//Undefined behaviour bar() is already dead.

   const Bar&n =Bar();//Temporary lifetime extended to match n's scope.
   // b is valid here.

    return 0;
}