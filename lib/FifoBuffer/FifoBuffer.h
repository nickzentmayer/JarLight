#pragma once
#ifndef __FIFOBUFFER_H__
#define __FIFOBUFFER_H__

template <typename T> class FifoBuffer {
public:
FifoBuffer();
void add(T value);
T peek(int index = 0);
T read();
T* read(int num);
bool available();
void flush();
private:
void shift();
void expand();
T* array = nullptr;
int len;
int pos;
int end;
};
#endif