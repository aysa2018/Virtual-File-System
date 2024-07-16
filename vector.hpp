//============================================================================
// Name         : vector.hpp
// Author       : Aysa Binte masud
// Version      : 01
// Date Created : 5 November 2023
// Date Modified: 12 November 2023
// Description  : Vector implmentation in C++
//============================================================================


#ifndef VECTOR_H
#define VECTOR_H

#include<cstdlib>
#include<iostream>
#include<sstream>
#include<vector>
#include <stdexcept>
using namespace std;

template <typename T>
class Vector
{
    private:
        T *data;                        //pointer to int(array) to store elements
        int v_size;                     //current size of vector (number of elements in vector)
        int v_capacity;                 //capacity of vector
    public:
        Vector(int cap=0);          //Constructor
        ~Vector();                  //Destructor
        int size() const;               //Return current size of vector
        int capacity() const;           //Return capacity of vector
        bool empty() const;             //Rturn true if the vector is empty, False otherwise
        const T& front();               //Returns reference of the first element in the vector
        const T& back();                //Returns reference of the Last element in the vector
        void push_back(T element);      //Add an element at the end of vector
        void insert(int index, T element); //Add an element at the index 
        void erase(int index);          //Removes an element from the index
        T& operator[](int index);           //Returns the reference of an element at given index
        T& at(int index);               //return reference of the element at given index
        void shrink_to_fit();           //Reduce vector capacity to fit its size
        void display();
};

// Create a construcotr which creates a vector with the specified capacity.
template <typename T>
Vector<T>::Vector(int cap) : v_size(0), v_capacity(cap) {
    data = new T[v_capacity];//create an array based vector with the given capacity
}

// Create a destructor which cleans up dynamically allocated memory.
template <typename T>
Vector<T>::~Vector() {
    delete[] data;
}

// Create a function that returns the current size (number of elements) of the vector.

template <typename T>
int Vector<T>::size() const {
    return v_size;//return the size of the vector
}

// Create a function that returns the capacity (maximum number of elements before resizing) of the vector.
template <typename T>
int Vector<T>::capacity() const {
    return v_capacity;// return the capacity of a vector
}

// create a function that returns true if the vector is empty, false otherwise.
template <typename T>
bool Vector<T>::empty() const {
    return (v_size == 0);//return true if empty, otherwise false
}

//Create a function that returns a reference to the first element in the vector.Throws an out_of_range exception if the vector is empty.

template <typename T>
const T& Vector<T>::front() {
    if (empty()) {
        throw out_of_range("Vector is empty");
    }
    return data[0];//return the data at front index
}

// Create a function that returns a reference to the last element in the vector. Throws an out_of_range exception if the vector is empty.

template <typename T>
const T& Vector<T>::back() {
    if (empty()) {
        throw out_of_range("Vector is empty");
    }
    return data[v_size - 1];//return the data at back index
}

template <typename T>
void Vector<T>::push_back(T element) {
    // Double the capacity if vector is full
    if (v_size >= v_capacity) {
        v_capacity = (v_capacity == 0) ? 1 : v_capacity * 2;
        T *newData = new T[v_capacity];
        for (int i = 0; i < v_size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
    data[v_size++] = element;
}

template <typename T>
void Vector<T>::insert(int index, T element) {
    if (index < 0 || index > v_size) {
        throw out_of_range("Index out of range");
    }
    if (v_size >= v_capacity) {
        // Double the capacity if vector is full
        v_capacity = (v_capacity == 0) ? 1 : v_capacity * 2;
        T *newData = new T[v_capacity];
        for (int i = 0; i < index; ++i) {
            newData[i] = data[i];
        }
        newData[index] = element;
        for (int i = index; i < v_size; ++i) {
            newData[i + 1] = data[i];
        }
        delete[] data;
        data = newData;
    } else {
        for (int i = v_size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = element;
    }
    v_size++;
}

template <typename T>
void Vector<T>::erase(int index) {
    if (index < 0 || index >= v_size) {
        throw out_of_range("Index out of range");
    }

    for (int i = index; i < v_size - 1; ++i) {
        data[i].~T(); // Manually call the destructor for the element
        new (&data[i]) T(data[i + 1]); // Move construct to shift elements
    }

    data[v_size - 1].~T(); // Call the destructor for the last element
    v_size--;
}

template <typename T>
T& Vector<T>::operator[](int index) {
    if (index < 0 || index >= v_size) {
        throw out_of_range("Index out of range");
    }
    return data[index];
}

template <typename T>
T& Vector<T>::at(int index) {
    return (*this)[index];
}


// create a function that reduces the vector’s capacity to fit its size.
template <typename T>
void Vector<T>::shrink_to_fit() {
    
    if (v_capacity > v_size) {// if the capacity is larger than the size of the vector, make them equal
        v_capacity = v_size;
        T* new_data = new T[v_capacity];//create a new array with new capacity
        for (int i = 0; i < v_size; i++) {
            new_data[i] = data[i];//transfer the elements front the old array to new array
        }
        delete[] data;//delete the elements of  the older array 
        data = new_data;//assign the new array to the older array
    }
}
template <typename T>
void Vector<T>::display() {
    for (int i = 0; i < v_size; ++i) {
        cout << data[i] << " ";
    }
    cout << endl;
}

#endif