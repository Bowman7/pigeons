#ifndef DS_HPP
#define DS_HPP

//c stype stack
class Stack{
public:
  void Push(int a){
    if(top==-1){
      top = 0;
      stack[top] = a;
    }
  }
private:
  int top = -1;
  int size = 10;
  int *stack = (int*)malloc(10*sizeof(int));
};


#endif
