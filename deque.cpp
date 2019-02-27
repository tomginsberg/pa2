#include "deque.h"

/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

/* Constructor for the Deque class */
template <class T>
Deque<T>::Deque(){
    this->k2 = 0;
    this->k1 = 0;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T const& newItem)
{
    if (this->data.size() == static_cast<unsigned int>(k2))
        this->data.push_back(newItem);
    else {
        this->data[k2] = newItem;
    }
    this->k2 += 1;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The leftmost item of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T elm = this->data[k1];
    this->k1 += 1;
    if (k2-k1 <= k1) {
        vector<T> newData;
        for (int i = k1; i < k2; i++) {
            newData.push_back(data[i]);
        }
        this->data = newData;
        k1 = 0;
        k2 = this->data.size();
    }
    return elm;
}

/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The rightmost item of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    this->k2 -= 1;
    T elm = this->data.back();
    this->data.pop_back();
    
    if (k2 - k1 <= k1) {
        vector<T> newData;
        for (int i = k1; i < k2; i++) {
            newData.push_back(data[i]);
        }
        this->data = newData;
        k1 = 0;
        k2 = this->data.size();
    }
    return elm;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the left of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return this->data[k1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return this->data[k2-1];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return (this->k2 - this->k1 == 0);
}
