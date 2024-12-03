#ifndef STATISTICS_H
#define STATISTICS_H
#include <algorithm>
#include <cstddef>
#include <istream>
#include <map>
#include <ostream>
#include <vector>

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

  float Average() const;

  size_t Size() const;

  int Minimum() const;

  int Maximum() const;

  float Deviation() const;

  template<typename T>
  void RemoveIf(const T &predicate);

  ContainerType Histogram(const size_t bin_count, const int begin, const int end) const;

  void DrawHistogramH(const size_t bin_count, const int begin, const int end);

  void DrawHistogramV(const size_t bin_count, const int begin, const int end);

  std::map<int, int> OccuresMoreThan(int min) const;
};

template<typename T>
Statistics::Statistics(T const *b, T const *e) : data(e - b) {
  std::copy(b, e, data.begin()); // we can use regular pointers to specify ranges!
}

template<typename T>
void Statistics::RemoveIf(const T &predicate) {
  data.erase(std::remove_if(data.begin(), data.end(), predicate), data.end());
}

#endif
