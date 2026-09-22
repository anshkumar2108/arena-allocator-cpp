#include<iostream>
#include<cstddef>
class SmartBuffer{
    private:
    std::byte* buffer;
    size_t size;
    public:
    //Constructor: Acquires the resource
    SmartBuffer(size_t buffer_size){
        size=buffer_size;
        buffer=new std::byte[1024];
        std::cout<<"Resource acquired: Allocated "<<size<<" bytes.\n";
    }
    //Destructor: Destroys the resource.
    ~SmartBuffer(){
        delete []buffer;
        std::cout<<"Resource Released: Freed "<<size<<" bytes\n";
    }
    //Method to simulate doing work.
    void writeData(){
        std::cout<<"Writing data to buffer.\n";
    }
};
int main(){
    std::cout<<"Entering main:\n";
    {//Create a temporary scope block.
        std::cout<<"Entering inner scope..\n";
        //Object created on the stack 
        //Memory manages on the heap.
        SmartBuffer myBuffer(1024);
        myBuffer.writeData();
        std::cout<<"Exiting inner scope"<<std::endl;

    }//Thing to notice is when the scope got over destructor was called.(object lifetime.)
    std::cout<<"Back to main"<<std::endl;
    return 0;
}
