#ifndef HANOI_H
#define HANOI_H

#include <vector>
#include <string.h>
#include <iostream>

using namespace std;

class Hanoi{ // Student inherits the Person class

    private:
    int stackNum;
    vector<bool> stack1;
    vector<int> stackVal;
    int totalStack;
    string pegName;

    public:
        Hanoi(string name, int n, int total);
        Hanoi(string name,int total);
        ~Hanoi();
        void push(int inputVal);
        int pop();
        int getStackNum();
        int getPegNum();
        int getStackVal(int i);
        int getStackVal();
};
#endif
