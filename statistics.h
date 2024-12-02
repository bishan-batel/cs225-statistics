#ifndef STATISTICS_H
#define STATISTICS_H
#include <algorithm> //copy
#include <cstddef>
#include <istream> //istream
#include <map> //map
#include <ostream> //ostream
#include <vector> //vector

class Statistics {
private:
  typedef std::vector<int> ContainerType;
  ContainerType data;

public:
  Statistics();

  template<typename T>
  Statistics(T const *b, T const *e);

  friend std::ostream &operator<<(std::ostream &out, Statistics const &stat);

  friend std::istream &operator>>(std::istream &in, Statistics &stat);

  int Average() const; // sum of all values divided by the size of data
  size_t Size() const; // size of the data, number of samples
  int Minimum() const; // obvious
  int Maximum() const; // obvious
  float Deviation() const; // standard deviation is a measure of how much the data is

  std::map<int, int> OccuresMoreThan(int min) const;
};

template<typename T>
Statistics::Statistics(T const *b, T const *e) : data(e - b) {
  std::copy(b, e, data.begin()); // we can use regular pointers to specify ranges!
}

#endif
