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

float Statistics::Average() const {
  return std::accumulate(data.begin(), data.end(), 0.f) / static_cast<float>(Size());
}

size_t Statistics::Size() const { return data.size(); }

int Statistics::Minimum() const {
  if (Size() == 0) {
    return 0;
  }

  return *std::min_element(data.begin(), data.end());
}

int Statistics::Maximum() const {
  if (Size() == 0) {
    return 0;
  }

  return *std::max_element(data.begin(), data.end());
}

namespace {
  /**
   * @brief Functor for folding an iterable set to the sum of squared differences
   * of a fixed value
   */
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
  /**
   * @class OccurenceInc
   * @brief Functor to record frequency information, discarding any that do not
   * match a constant threshold
   */
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
  /**
   * @class HistogramBucket
   * @brief Functor for recording histogram data  in a given range with a number of buckets
   */
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

      const int index = static_cast<int>(bin_count * static_cast<float>(v - begin) / (end - begin));

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

namespace {
  /**
   * @class DrawHistogramH
   * @brief Functor printing a single line of a vertical histogram
   *
   */
  struct DrawHistogramH {
    void operator()(const int x) const {
      std::cout << std::setfill('*') << std::setw(x) << "";
      std::cout << std::endl;
    }
  };
} // namespace

void Statistics::DrawHistogramH(const size_t bin_count, const int begin, const int end) const {
  std::vector<int> histogram = Histogram(bin_count, begin, end);

  std::for_each(histogram.begin(), histogram.end(), ::DrawHistogramH());
}

namespace {
  /**
   * @class Range
   * @brief Functor for generating an integer range
   */
  struct Range {
    size_t i;

    Range() : i(0) {}

    int operator()() { return i++; }
  };

  /**
   * @class DrawHistogramChar
   * @brief
   *
   */
  struct DrawHistogramChar {
    const size_t height;

    DrawHistogramChar(const size_t height) : height(height) {}

    void operator()(const size_t freq) const { std::cout << (freq >= height ? '*' : ' '); }
  };

  /**
   * @class DrawHistogramLine
   * @brief Functor for drawing a line of a vertical histogram
   *
   */
  struct DrawHistogramLine {
    const size_t height;
    std::vector<int> &histogram;

    DrawHistogramLine(std::vector<int> &histogram, const size_t height) : height(height), histogram(histogram) {}

    void operator()(const size_t i) const {
      std::for_each(histogram.begin(), histogram.end(), DrawHistogramChar(height - i));
      std::cout << std::endl;
    }
  };
} // namespace

void Statistics::DrawHistogramV(const size_t bin_count, const int begin, const int end) const {
  std::vector<int> histogram = Histogram(bin_count, begin, end);

  if (histogram.empty()) {
    return;
  }

  const int height = *std::max_element(histogram.begin(), histogram.end());

  std::vector<size_t> range(height);
  std::generate(range.begin(), range.end(), Range());
  std::for_each(range.begin(), range.end(), ::DrawHistogramLine(histogram, height));
}
