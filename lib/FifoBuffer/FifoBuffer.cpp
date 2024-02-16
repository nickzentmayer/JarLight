#include "FifoBuffer.h"

template <typename T> FifoBuffer<T>::FifoBuffer() {
    len = 2;
    array = new T[len];
    pos = 0;
    end = 0;
}

template <typename T> void FifoBuffer<T>::add(T value) {
    if(end < len) {
        array[end++] = value;
    }
    else {
        if(pos == 0) {
            expand();
        }
        else {
            shift();
        }
    }
}

template <typename T> T FifoBuffer<T>::peek(int index) {
    return array[pos + index];
}

template <typename T> T FifoBuffer<T>::read() {
    return array[pos++];
}

template <typename T> T* FifoBuffer<T>::read(int num) {
    T* temp = new T[num];
    for(int i = 0; i < num; i++) temp[i] = array[pos++];
    return temp;
}

template <typename T> void FifoBuffer<T>::flush() {
    pos = 0;
    end = 0;
}

template <typename T> void FifoBuffer<T>::shift() {
    int nend = end - pos;
    for(int i = 0; pos != end; i++) array[i] = array[pos++];
    pos = 0;
    end = nend;
}

template <typename T> void FifoBuffer<T>::expand() {
    len *= 2;
    T* old = array;
    array = new T[len];
    for(int i = 0; pos != end; i++) array[i] = old[i];
    delete old;
    pos = 0;
    end = nend;
}

template <typename T> bool FifoBuffer<T>::available() {
    return ((end - pos) > 0);
}