#include <iostream>
int main()
{
    /*
    - A pointer to T can point to a value type of T
    an lvalue reference to T can bind only to lvalue of type T
    - int *pi=&3; //Compile error: cant apply & to 3
    - int &ri=3; //Compile error: cant bind this,either */

    /*
    int i;
    double *pd=&i; //Compile error: cant convert int* to double*
    dobule &rd=i;  //Compile error: cant bind this*/

    /*There is an exception to this rule above given.
    lvalue reference to const T can bind to e, even if e is an rvalue or has a type other than T.

    T const &r=e; // e need not be lvalue of Type T
    However its valid only if there's a conversion from e's type to T
    */
    /*As with T(7).f(0) the compiler materializes a temporary object holding a copy of the converted rvalue
    The lvalue reference then binds to the temporary.*/

    /*
    double const &rd=1;
    when executed:
    -It converts from int to double
    -It creates a temporary to hold the result of the conversion.
    -It binds rd to the temporary

    The program destroys the temporary when execution leaves the scope containing rd.
    */

    /*Compilers dont bind references to temporaries unnecessarily.

    int i;
    int const &ri=i;
    ri binds directly to i;
    The compiler doesnt invent a temporary to hold copy of i;*/

    /*Rvalue references
    &----> lvalue reference
    &&----> ravalue references
    int &&ri=10;

    Can use rvalue references as function parameter and return types as in:
    double &&f(int&&ri)

    Can also have rvalue reference to const as:
    int const &&rci=20;
    */

    /*Rvalue references bind only to rvalues
    This is true even for "rvalue reference to const"
    int n=10;
    int &&ri=n;      //compile error: n is an lvalue
    int const &&rj=n; //compile error: n is an lvalue*/

    /*Can overload a function that accepts an lvalue refrence parameter with a function that accepts an rvalue reference parameter
    class my_string{
    public:
     my_string(char const*str);
     my_string(my_string const&s);
     my_string(my_string&&);
     private:
     };
     my_string operator+(my_string const&s1 ,my_string const&s2)


     when we create a my_string, the compiler selects the constructor based on whether the argument is an lvalue or an rvalue

     my_string s1;
     my_string s2{s1}; //calls my_string(my_string const &)
     my_string s3{s1+s2}; //calls my_string(my_string&&)
     s1 is an lvalue
     s2+s1 is rvalue*/

    /*Binding an rvalue reference to rvalue const triggers a temporary materialization conversion
    int &&ri=3;//Ok: binds ri to temporary;
    ri is reference to non_const can modify the temporary
    ++ri;
     */

    // Rvalue references and temporaries
    /*Temporary objects i.e rvalues, are destroyed at the end of the statement in which they are created.
    suppose compiler encounters this:
    string s1=s2+s3;
    s1=s2+s3;
    it generates something like this:
    my_string s1,s2,s3;
    my_string temp{s2+s3};//Create a temporary
    s1=temp;
    temp.~my_string(); //Destroy the temporary.
    */

    /* when constructing a new my_string from a my_string rvalue,you need not to allocate a new array.

    - you can take the array from the soon-to-be-destroyed source my_string and use it to construct the target my_string,

    my_string::my_string(my_string&&s){
    actual_str=s.actual_str;
    stored_length=s.stored_length;
    s.actual_str=nullptr;//Do not forget to do this.
    }
    If we dont set s.actual_str to null, the my_string destructor will delete the array that was just transffered. */

    /*Copy constructor takes lvalue
    Move constructor takes rvalue

    - A copy constructor for a class T is typically declared ads:
    T(T const&x); //Copy constructor

    - A move constructor for a class T is typically declared as:
    T(T&&x); //Move constructor

    - Copy constructor performs a non destructive copy.
    -Move constructor performs a destructive copy.
    */

    /*Using a move constructor written with an lvalue reference parameter yields surprising behaviour
    my_string s1{"867-5309"}; //non-copy/non-move constructor
    my_string s2{s1};//Move resources from s1 to s2;
    cout<<s1<<endl;//Prints nothing.
    cout<<s2<<endl; //Prints "867-5309"

    - The first output statement prints nothing because s2's definition sets s1's value to the empty string.
    - move constructors are often faster than copy constructors but destroying the source object is rarely a desirable default.

    */

    /* a class can have move assignment operators as in :
    class my_string{
    public:
    my_string(my_string const&);//Copy constructor
    my_string&operator=(my_string const&);// Copy assignment
    my_string(my_string &&); //Move constructor
    my_string&operator=(my_string&&);//Move assignment

    A class is said to have move semantics if it has both move constructor and operator.

    }*/

    /*
    the definition for a move assignment often looks similar to the definition of its companion move constructor:
    my_string &my_string::operator=(my_string&&s){
    if(this!=&s){ //check for self-assignment
    delete []actual_str; //discard the old value
    actual_str=s.actual_str;
    stored_lenght=s.stored_length;
    s.actual_str=nullptr;
    }
    return *this;
    - Move assignment should return an lvalue reference.

    }*/

    // REVISITING SWAPPING OBJECTS

    /*
    we need one more piece to implement a swap that uses move semantics.
    -as written this version still uses copy semantics even if the type substituted for T has move semantics:
    template<typename T>
    void swap(T&x,T&y){
    T temp(x);
    x=y;
    y=temp;


    -- We will use the std::move function
    - We can get the compiler to favour move semantics over copy semantics by using the standard move function template:
    template<typename T>
    void swap(T&x,T&y){
    T temp{std::move(x)};//favours T(T&&)
    x=std::move(y); //favours T&operator=(T&&)
    y=std::move(temp); //favours T&operator=(T&&)
    }
    - This version uses move semantics if T supports them and copy semantics if it doesent.

    - std::move(x) returns x as an rvalue by casting it to an rvalue reference and return values dont have names.
    - calling std::move(x) doesn't actually move x itself;
    - std::move(x) tells the compiler that it's safe to move from x, even if x isn't about to expire.

    }*/
    /*
    Since they have names, even rvalue reference parameters are treated as lvalues inside the functions to which they belong.

    void f(my_string&&){
    my_string temp{s}; //Calls my_string(my_string const&)
    }
    Within f, s exists for the duration of the function.
    In this context it's an lvalue.

    void f(my_string&&s){
    my_string temp{std::move(s)}; //calls my_string(my_string&&)
    }

    Return expressions are often an exception to the previous rule.

    The operand of a return statement is implicitly movable. if its 
    - a function parameter.
    - a local object.

    my_string g(int x){
    my_string temp;
    return temp;
    }

    The term implicitly movable doesn't tell the whole story.
    The compiler may be able to arrange it so that returning from g involves neither a copy nor a move.

    */

    // Return value optimization 
    /*
    main's local s and g's local temp would be separate objects, each with its own memory address.
    my_string g(int x){
    my_string temp;
    return temp;
    }
    int main(){
    my_string s{g(10)};
    }
    upon return from g, the system would initialize s by passing temp to the move (or copy) constructor.
    */
   /*
    As of C++17, the compiler uses the same location for main's local s and g's local temp:
   - Thus returning from g requires neither a move nor a copy- the value is (and always has been)stored in s
   */
  /*
  This preocess- storing a local value directly in its return value location- is often called named return value optimization(NRVO).
  - C++ has always allowed NRVO as an optimization.
  - As of C++17 it is not optional the compiler must do it.
  - As such it's no longer really and optimization- its just how the language works.
  */
 /*
 return std::move(temp);
 It can result in less efficient code.
 */

    int i = 10;
    int const &ri = i;
    std::cout << ri << " " << i;

    return 0;
}