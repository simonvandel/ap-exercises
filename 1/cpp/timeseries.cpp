#include "timeseries.h"

#include <algorithm>
#include <climits>
#include <exception>
#include <functional>
#include <random>

using namespace sw805f17g2;

TimeSeries::TimeSeries(const std::vector<int> &data) : data(data) {
  auto temp_min = std::min_element(data.begin(), data.end());
  if (temp_min != data.end()) {
    minimum = *temp_min;
  } else {
    throw new std::invalid_argument("data");
  }

  // we are sure that the data parameter is not empty
  maximum = *std::max_element(data.begin(), data.end());
}

template <typename T>
void zip_with(std::vector<T> &left, const std::vector<T> &right,
              std::function<void(T &, const T &)> f) {
  // make sure left and right have the same length. Extend the smallest one.
  auto longer = left.size() > right.size() ? left.size() : right.size();
  // resize default initializes the new elements (e.g. 0 for int)
  left.resize(longer);

  auto l_cur = left.begin();
  auto r_cur = right.begin();
  for (; l_cur != left.end() && r_cur != right.end(); ++l_cur, ++r_cur) {
    f(*l_cur, *r_cur);
  }
}

void TimeSeries::operator+=(const TimeSeries &other) {
  zip_with<int>(this->data, other.data, [](int &a, const int &b) { a += b; });
}

TimeSeries operator+(TimeSeries left, const TimeSeries &right) {
  left += right;
  return left;
}

long TimeSeries::amplitude() const {
  return static_cast<long>(maximum) - minimum;
}

TimeSeries TimeSeries::MakeRandom(const int length) {
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(INT_MIN, INT_MAX);
  std::vector<int> data_buffer;
  data_buffer.reserve(length);
  for (size_t i = 0; i < length; i++) {
    data_buffer.push_back(uniform_dist(e1));
  }
  return TimeSeries(data_buffer);
}