#include "Hanoi.h"

Hanoi::Hanoi(string name, int total){
    //initializes empty pegs
    pegName = name;
    stackNum = 0 - 1;
    totalStack = total - 1;
    stack1.resize(totalStack + 1);
    stackVal.resize(totalStack + 1);
    for (int i = 0; i <= totalStack; i++){
        stack1[i] = false;
        stackVal[i] = 0;
    }
}

Hanoi::Hanoi(string name, int n, int total){
    //initializes full pegs
    pegName = name;
    stackNum = n - 1;
    totalStack = total - 1;
    stack1.resize(totalStack + 1);
    stackVal.resize(totalStack + 1);
    for (int i = 0; i <= totalStack; i++){
        stack1[i] = true;
        stackVal[i] =  i;
    }
}

Hanoi::~Hanoi(){
}

void Hanoi::push(int inputVal){
    //puts in a disk from the peg
    if(stackNum == totalStack){
    }
    else{
        stackNum++;
        stack1[stackNum] = true;
        stackVal[stackNum] = inputVal;
    }
}
int Hanoi::pop(){
    //takes out a disk from the peg
    if(stackNum == -1){
    }
    else{
        stack1[stackNum] = false;
        int tempStackVal = stackVal[stackNum];
        stackNum--;
        return tempStackVal;
    }
}
int Hanoi::getStackNum(){
    //returns the number of pegs in the stack
    return stackNum + 1;
}

int Hanoi::getPegNum(){
    //returns the name of the peg (Left, Middle or Right)
    if(pegName == "Left"){
        return 1;
    }
    else if(pegName == "Middle"){
        return 3;
    }
    else if(pegName == "Right"){
        return 5;
    }
}

int Hanoi::getStackVal(int i){
    //returns the value of the disk. Used to check whether a disk can be placed on top of the other.
    if(stackNum > -1){
        return stackVal[i];
    }
    else{
        return -1;
    }
}

int Hanoi::getStackVal(){
    //returns the value of the disk. Used to check whether a disk can be placed on top of the other
    if(stackNum > -1){
        return stackVal[stackNum];
    }
    else{
        return -1;
    }
}
