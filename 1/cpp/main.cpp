#include "timeseries.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>

using namespace sw805f17g2;

void timeit(std::function<void()> f) {
  auto t0 = std::chrono::high_resolution_clock::now();
  f();
  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout
      << std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()
      << "nanosec\n";
}

// conclusion:
// without optimizations: raw pointer is the fastest.
// with optimizations: value is faster than raw pointer,
// generally, unique_ptr is the slowest.
int main() {
  // step 2
  const int num_timeseries = 1000000;
  const int num_datapoints = 100;

  std::vector<TimeSeries> timeseries;
  timeseries.reserve(num_timeseries);
  for (size_t i = 0; i < num_timeseries; i++) {
    timeseries.emplace_back(TimeSeries::MakeRandom(num_datapoints));
  }

  auto random_timeseries = TimeSeries::MakeRandom(num_datapoints);

  std::vector<TimeSeries> timeseries1;
  timeseries1.reserve(num_timeseries);
  for (auto &elem : timeseries) {
    timeseries1.emplace_back(elem);
  }

  for (auto &ts : timeseries1) {
    ts += random_timeseries;
  }

  // std::for_each(timeseries.begin(), timeseries.end(), [](TimeSeries const&
  // elem) {
  //     std::cout << elem.amplitude() << std::endl;
  // });
  timeit([&]() { std::sort(timeseries1.begin(), timeseries1.end()); });
  // std::cout << std::endl;
  // std::for_each(timeseries.begin(), timeseries.end(), [](TimeSeries const&
  // elem) {
  //     std::cout << elem.amplitude() << std::endl;
  // });

  // step 3
  std::vector<std::unique_ptr<TimeSeries>> timeseries2;
  timeseries2.reserve(num_timeseries);
  for (auto &elem : timeseries) {
    timeseries2.emplace_back(new TimeSeries(elem));
  }

  for (auto &ts : timeseries2) {
    *ts += random_timeseries;
  }

  timeit([&]() {
    std::sort(timeseries2.begin(), timeseries2.end(),
              [](const std::unique_ptr<TimeSeries> &left,
                 const std::unique_ptr<TimeSeries> &right) {
                return *left < *right;
              });
  });

  // step 3.1 (raw pointer)
  // we believe this is faster than step3, because we the overhead from
  // unique_ptr
  // std::vector<TimeSeries *> timeseries3;
  // timeseries3.reserve(num_timeseries);
  // for (auto &elem : timeseries) {
  //   timeseries3.emplace_back(new TimeSeries(elem));
  // }

  // for (auto &ts : timeseries3) {
  //   *ts += random_timeseries;
  // }

  // timeit([&]() {
  //   std::sort(timeseries3.begin(), timeseries3.end(),
  //             [](TimeSeries *const &left, TimeSeries *const &right) {
  //               return *left < *right;
  //             });
  // });
}