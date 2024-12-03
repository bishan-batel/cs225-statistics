#include "statistics.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>

Statistics::Statistics() : data() {}

std::ostream &operator<<(std::ostream &out, Statistics const &stat) {
  std::copy(stat.data.begin(), stat.data.end(), std::ostream_iterator<int>(out, " "));
  return out;
}

std::istream &operator>>(std::istream &in, Statistics &stat) {
  std::copy(std::istream_iterator<int>(in), std::istream_iterator<int>(), std::back_inserter(stat.data));
  return in;
}

float Statistics::Average() const { return std::accumulate(data.begin(), data.end(), 0.f) / (float) Size(); }

size_t Statistics::Size() const { return data.size(); }

int Statistics::Minimum() const {
  if (Size() == 0) {
    return 0;
  }

  return std::accumulate(data.begin(), data.end(), data[0], std::min<int>);
}

int Statistics::Maximum() const {
  if (Size() == 0) {
    return 0;
  }

  return std::accumulate(data.begin(), data.end(), data[0], std::max<int>);
}

namespace {
  struct DiffSquared {
    const float mean;

    DiffSquared(const float value) : mean(value) {}

    float operator()(const float sum, const int x) const { return sum + (x - mean) * (x - mean); }
  };
} // namespace

float Statistics::Deviation() const {
  return std::sqrt(std::accumulate(data.begin(), data.end(), 0.f, DiffSquared(Average())) / Size());
}

namespace {
  struct OccurenceInc {
    std::map<int, int> &occurences;
    std::map<int, int> unfiltered_occurences;
    int min;

    OccurenceInc(std::map<int, int> &occurences, const int min) :
        occurences(occurences), unfiltered_occurences(), min(min) {}

    void operator()(const int val) {
      if (unfiltered_occurences.find(val) == unfiltered_occurences.end()) {
        unfiltered_occurences.insert(std::make_pair(val, 0));
      }

      if (++unfiltered_occurences[val] > min) {
        occurences[val] = unfiltered_occurences[val];
      }
    }
  };
} // namespace

std::map<int, int> Statistics::OccuresMoreThan(const int min) const {
  std::map<int, int> occurences;

  std::for_each(data.begin(), data.end(), OccurenceInc(occurences, min));

  return occurences;
}

namespace {
  struct HistogramBucket {
    const size_t bin_count;
    const int begin, end;
    std::vector<int> &values;

    HistogramBucket(std::vector<int> &values, const size_t bin_count, const int begin, const int end) :
        bin_count(bin_count), begin(begin), end(end), values(values) {}

    void operator()(const int v) const {
      if (v < begin or v >= end) {
        return;
      }

      const int index = static_cast<int>(static_cast<float>(bin_count) * (v - begin) / (end));

      if (static_cast<size_t>(index) < bin_count) {
        values[index]++;
      }
    }
  };
} // namespace

Statistics::ContainerType Statistics::Histogram(const size_t bin_count, const int begin, const int end) const {
  std::vector<int> values;

  values.reserve(bin_count);
  for (size_t i = 0; i < bin_count; i++) {
    values.push_back(0);
  }

  std::for_each(data.begin(), data.end(), HistogramBucket(values, bin_count, begin, end));

  return values;
}

void Statistics::DrawHistogramH(const size_t bin_count, const int begin, const int end) {
  std::vector<int> histogram = Histogram(bin_count, begin, end);

  for (size_t i = 0; i < histogram.size(); i++) {
    std::cout << std::setfill('*') << std::setw(histogram[i]) << "";
    std::cout << std::endl;
  }
}

void Statistics::DrawHistogramV(const size_t bin_count, const int begin, const int end) {
  Histogram(bin_count, begin, end);
}
