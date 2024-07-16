//============================================================================
// Name         : queue.hpp
// Author       : Aysa Binte masud
// Version      : 01
// Date Created : 5 November 2023
// Date Modified: 15 November 2023
// Description  : Queue implmentation in C++
//============================================================================




#ifndef QUEUE_H
#define QUEUE_H
#include<cstdlib>
#include<stdexcept>
#include<sstream>
#include<vector>
using namespace std;

template <typename T>
class Queue
{
    private:
        T *array;
        int capacity;   //Max Capacity of the Queue
        int size;   //current number of elements in the Queue
        int front;      //front of the Queue
        int rear;       //index where a new element will be added
    public:
        
        Queue(int capacity=10);
        ~Queue();
        void enqueue(T); 
        T dequeue();
        bool isEmpty();
        bool isFull();
        T front_element();
        int getSize();
        friend void display();
    
};
//================================================


//constructor for initializing attributes
template <typename T>
Queue<T>::Queue(int cap) : capacity(cap), size(0), front(0), rear(-1) {
    array = new T[capacity];//make a new array with the given capacity
}

//destructor for freeing up space
template <typename T>
Queue<T>::~Queue() {
    delete[] array;//delete the array
}

//create a method that returns the size
template <typename T>
int Queue<T>::getSize() {
    return size;
}

//create a method that adds the one element in the queue
template <typename T>
void Queue<T>::enqueue(T element) {
    if (isFull()) {//if the queue is full, show an error message
        throw overflow_error("Queue is full");
    }
    rear = (rear + 1) % capacity;//extend the queue as a circular queue
    array[rear] = element;//add the element at the rear index
    size++;//increment the size
}

//create a method that removes the one element in the queue
template <typename T>
T Queue<T>::dequeue() {
    if (isEmpty()) {//if the queue is empty, show an error message
        throw underflow_error("Queue is empty");
    }
    T frontElement = array[front];
    front = (front + 1) % capacity;
    size--;
    return frontElement;
}

//check whether empty
template <typename T>
bool Queue<T>::isEmpty() {
    return size == 0;
}

//check whether full
template <typename T>
bool Queue<T>::isFull() {
    return size == capacity;
}

//return the front element
template <typename T>
T Queue<T>::front_element() {
    if (isEmpty()) {
        throw underflow_error("Queue is empty");
    }
    return array[front];
}




#endif