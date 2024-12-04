#ifndef STATISTICS_H
#define STATISTICS_H
#include <algorithm>
#include <cstddef>
#include <istream>
#include <map>
#include <ostream>
#include <vector>

/**
 * @class Statistics
 * Class for handling integer statistic samples
 */
class Statistics {
private:
  typedef std::vector<int> ContainerType;
  ContainerType data;

public:
  Statistics();

  template<typename T>
  Statistics(T const *b, T const *e);

  /**
   * @brief Emits statistic values to stream
   */
  friend std::ostream &operator<<(std::ostream &out, Statistics const &stat);

  /**
   * @brief Reads statistic data in from stream
   */
  friend std::istream &operator>>(std::istream &in, Statistics &stat);

  /**
   * @brief Returns the average of all samples
   */
  float Average() const;

  /**
   * @brief Returns the number of samples
   */
  size_t Size() const;

  /**
   * @brief Returns the smallest sample
   */
  int Minimum() const;

  /**
   * @brief Returns the largest sample
   */
  int Maximum() const;

  /**
   * @brief Returns the standard deviation of all samples
   */
  float Deviation() const;

  /**
   * @brief Removes any sample that matches the predicate
   */
  template<typename T>
  void RemoveIf(const T &predicate);

  /**
   * @brief Creates a vector of frequency data for each sample in the requested range,
   */
  ContainerType Histogram(const size_t bin_count, const int begin, const int end) const;

  /**
   * @brief Prints out the histogram horizontally
   */
  void DrawHistogramH(const size_t bin_count, const int begin, const int end) const;

  /**
   * @brief Prints out the histogram vertically
   */
  void DrawHistogramV(const size_t bin_count, const int begin, const int end) const;

  /**
   * @brief Returns a map where each key is a sample value and the associated
   * value is its frequency in the set, this will only record values that have a
   * frequency of at least 'min'
   */
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
