#include "statistics.h"
#include <cmath> //sqrt
#include <iomanip> //setfill setw
#include <iostream> //cout
#include <iterator> //ostream_iterator, back_inserter
#include <numeric> //accumulate

Statistics::Statistics() : data() {}

std::ostream &operator<<(std::ostream &out, Statistics const &stat) {
  std::copy(stat.data.begin(), stat.data.end(), std::ostream_iterator<int>(out, " "));
  return out;
}

std::istream &operator>>(std::istream &in, Statistics &stat) {
  std::copy(std::istream_iterator<int>(in), std::istream_iterator<int>(), std::back_inserter(stat.data));
  return in;
}

int Statistics::Average() const {
  const size_t length = Size();
  int sum = 0;

  for (size_t i = 0; i < length; i++) {
    sum += data[i];
  }

  return sum / length;
}

size_t Statistics::Size() const { return data.size(); }

int Statistics::Minimum() const {
  const size_t length = Size();

  if (length == 0) {
    return 0;
  }

  int min = data[1];
  for (size_t i = 0; i < length; i++) {
    min = std::min(min, data[i]);
  }

  return min;
}

int Statistics::Maximum() const {
  const size_t length = Size();

  if (length == 0) {
    return 0;
  }

  int max = data[1];
  for (size_t i = 0; i < length; i++) {
    max = std::max(max, data[i]);
  }

  return max;
}

float Statistics::Deviation() const {
  /* formula Deviation( x_1, ... , x_n ) = sqrt ( 1/n * sum ( x_i -a )^2 ) */
  const size_t length = Size();
  const int avg = Average();

  int sum = 0;
  for (size_t i = 0; i < length; i++) {
    sum += (data[i] - avg) * (data[i] - avg);
  }

  return std::sqrt(sum * 1.f / (length));
}

std::map<int, int> Statistics::OccuresMoreThan(const int min) const {
  std::map<int, int> final_output, tracker;

  const size_t length = Size();

  for (size_t i = 0; i < length; i++) {
    const int value = data[i];

    tracker[value]++;

    if (tracker[value] >= min) {
      final_output[value] = tracker[value];
    }
  }

  return final_output;
}
