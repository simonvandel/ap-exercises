#include <vector>

namespace sw805f17g2 {
class TimeSeries {
public:
  TimeSeries(const std::vector<int> &data);
  static TimeSeries MakeRandom(const int length);
  void operator+=(const TimeSeries &other);

  long amplitude() const;

private:
  std::vector<int> data;
  int minimum;
  int maximum;

  friend bool operator<(const TimeSeries &left, const TimeSeries &right);
};

TimeSeries operator+(const TimeSeries &left, const TimeSeries &right);

inline bool operator<(const TimeSeries &left, const TimeSeries &right) {
  auto this_amp = left.amplitude();
  auto other_amp = right.amplitude();

  return this_amp < other_amp;
}
}
