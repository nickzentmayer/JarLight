#pragma once
using namespace std;

template <class T> class FifoBuffer {
public:
FifoBuffer();

void add(T value);
T peek(int index = 0);
T read();
T* read(int num);
bool available();
void flush();
void shift();
void expand();
private:

T* array = nullptr;
int len;
int pos;
int end;
};

template <class T> FifoBuffer<T>::FifoBuffer() {
    len = 255;
    array = new T[len];
    pos = 0;
    end = 0;
}

template <class T> void FifoBuffer<T>::add(T value) {
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

template <class T> T FifoBuffer<T>::peek(int index) {
    return array[pos + index];
}

template <class T> T FifoBuffer<T>::read() {
    if(pos < end) return array[pos++];
    else return (T)NULL;
}

template <class T> T* FifoBuffer<T>::read(int num) {
    T* temp = new T[num];
    for(int i = 0; i < num; i++) temp[i] = array[pos++];
    return temp;
}

template <class T> void FifoBuffer<T>::flush() {
    pos = 0;
    end = 0;
}

template <class T> void FifoBuffer<T>::shift() {
    int nend = end - pos;
    for(int i = 0; pos != end; i++) array[i] = array[pos++];
    pos = 0;
    end = nend;
}

template <class T> void FifoBuffer<T>::expand() {
    len *= 2;
    T* old = array;
    array = new T[len];
    for(int i = 0; pos != end; i++) array[i] = old[i];
    delete old;
}

template <class T> bool FifoBuffer<T>::available() {
    return ((end - pos) > 0);
}