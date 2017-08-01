#ifndef CBUFFER_H
#define CBUFFER_H


#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <cmath>

using namespace std;

template <class T>
class CircularBuffer
{
  private:
    vector<T> elements;
    uint length;
    uint ind;
    bool full;
    uint incrementIndex(uint ind);
    uint deincrementIndex(uint ind);

  public:
    CircularBuffer<T>(uint length);
    void push(T const &);
    void printAll();
    bool filled();
    vector<T> getThree(long int time);
    T getClosest(long int time);
    T getInterpolated(long int time);
    T getLastDataPoint();
};

template <class T>
CircularBuffer<T>::CircularBuffer(uint length) {
    this->length = length;
    ind = 0;
}

template <class T>
T CircularBuffer<T>::getLastDataPoint() {
    return elements[ind];
}

template <class T>
bool CircularBuffer<T>::filled() {
    if (elements.size() < length)
        return false;
    return true;
}

template <class T>
void CircularBuffer<T>::push(T const &elem) {
    if (!full) {
      elements.push_back(elem);
      ind++;
      if (this->filled()) {
        full = true;
      }
    } else {
      ind = incrementIndex(ind);
      elements[ind] = elem;
    }
}

template <class T>
void CircularBuffer<T>::printAll() {
  if (full){
    for (int i = 0; i < length; i++){
      cout << elements[i] << endl;
    }
  }
}

template <class T>
uint CircularBuffer<T>::incrementIndex(uint i) {
  return (i + 1) % (length);
}

template <class T>
uint CircularBuffer<T>::deincrementIndex(uint i) {
  i--;
  if (i > 40000000){
    i = length - 1;
  }
  return i;
}

template <class T>
vector<T> CircularBuffer<T>::getThree(long int time) {
    bool errorIncreasing = false;

    int tmperr = 0;
    uint tempInd = ind;

    int err = 1000000;
    uint foundInd = tempInd;

    uint i = 0;
    while (!errorIncreasing and i < length){
      tmperr = abs(elements[tempInd].time - time);
      if (tmperr <= err) {
        foundInd = tempInd;
        err = tmperr;
      } else {
        errorIncreasing = true;
      }
      i++;
      tempInd = deincrementIndex(tempInd);
    }

    vector<T> v;


    v.push_back(elements[deincrementIndex(foundInd)]);
    v.push_back(elements[foundInd]);
    v.push_back(elements[incrementIndex(foundInd)]);
    #ifdef DEBUG_INTERPOLATION 
      cout << v[0] << v[1] << v[2] << endl;
    #endif
    return v;
}

template <class T>
T CircularBuffer<T>::getClosest(long int time) {
  return getThree(time)[1].clone();
}

template <class T>
T CircularBuffer<T>::getInterpolated(long int time) {
  vector<T> v = getThree(time);
  if (time > v[1].time)
    return v[1].interpolate(v[2],time);
  else
    return v[1].interpolate(v[0],time);
}

#endif