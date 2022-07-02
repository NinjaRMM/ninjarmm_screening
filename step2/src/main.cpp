#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace jobs {

template <typename NAME, typename DESCRIPTION, typename HOURS> class Job_t {
public:
  using Name = NAME;
  using Description = DESCRIPTION;
  using Hours = HOURS;

  using Ptr = std::shared_ptr<Job_t<NAME, DESCRIPTION, HOURS>>;

public:
  Job_t() = delete;
  Job_t(const Job_t &) = delete;
  Job_t(Job_t &&) = delete;

  template <typename JOB> static Ptr create(Hours pHours) {
    return std::make_shared<JOB>(pHours);
  }

  virtual Name name() const { return mName; }
  virtual Description description() const { return mDescription; }
  virtual Hours hours() const { return mHours; }

  template <typename t_iostream = std::ostream>
  void DoWork(t_iostream &out) const {
    out << "My work involves " << mDescription << '\n';
  }

  Job_t &operator=(const Job_t &) = delete;
  Job_t &operator=(Job_t &&) = delete;

protected:
  Job_t(Name &&pName, Description &&pDescription, Hours pHours)
      : mName(std::move(pName)), mDescription(std::move(pDescription)),
        mHours(pHours) {}

protected:
  Name mName;
  Description mDescription;
  Hours mHours;
};

template <typename NAME, typename DESCRIPTION, typename HOURS>
class Programmer_t : public Job_t<NAME, DESCRIPTION, HOURS> {
public:
  Programmer_t(HOURS pHours)
      : Job_t<NAME, DESCRIPTION, HOURS>("Programmer",
                                        "Writes ninja level programs", pHours) {
  }
};

template <typename NAME, typename DESCRIPTION, typename HOURS>
class Pilot_t : public Job_t<NAME, DESCRIPTION, HOURS> {
public:
  Pilot_t(HOURS pHours)
      : Job_t<NAME, DESCRIPTION, HOURS>("Pilot", "Flies all over the world",
                                        pHours) {}
};

template <typename NAME, typename DESCRIPTION, typename HOURS>
using Jobs_t = std::vector<typename Job_t<NAME, DESCRIPTION, HOURS>::Ptr>;

template <typename JOBS, typename JOB>
void add_job(JOBS &pJobs, typename JOB::Hours hours) {
  pJobs.push_back(JOB::template create<JOB>(hours));
}

template <typename JOBS, typename JOB_PTR>
void traverse_jobs(JOBS &pJobs, std::function<void(JOB_PTR)> visit) {

  for (JOB_PTR jobPtr : pJobs) {
    visit(jobPtr);
  }
}
} // namespace jobs

class Color {
public:
  Color(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}
  bool operator==(const Color &color) const {
    return (red == color.red) && (green == color.green) && (blue == color.blue);
  }

  bool operator>=(const Color &color) const {
    if ((red >= color.red) && (green >= color.green) && (blue >= color.blue)) {
      return true;
    }
    return false;
  }

  bool operator<=(const Color &color) const {
    if ((red <= color.red) && (green <= color.green) && (blue <= color.blue)) {
      return true;
    }
    return false;
  }

  friend std::ostream &operator<<(std::ostream &out, const Color &color) {
    out << '(' << static_cast<uint16_t>(color.red) << ','
        << static_cast<uint16_t>(color.green) << ','
        << static_cast<uint16_t>(color.blue) << ')';
    return out;
  }

private:
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

namespace bounds {

template <typename t_value = uint32_t>
bool IsInBounds(t_value value, t_value min, t_value max) {
  return ((value >= min) && (value <= max));
}

template <typename t_value = uint32_t>
void TestIsInBounds(t_value value, t_value min, t_value max) {
  std::cout << "Is " << value << " between " << min << " and " << max << "? "
            << (IsInBounds(value, min, max) ? "yes" : "no") << '\n';
}
} // namespace bounds

namespace contains {

template <typename ITERATOR>
void Print(ITERATOR begin, ITERATOR end, std::ostream &out = std::cout) {
  out << '[';
  for (ITERATOR ite = begin; ite != end; ++ite) {
    out << *ite;
    if (std::next(ite) != end) {
      out << ' ';
    }
  }
  out << ']';
}

template <typename TYPE> class Compare {
public:
  Compare() = delete;
  Compare(const Compare &) = delete;
  Compare(Compare &&) = default;
  Compare &operator=(const Compare &) = delete;
  Compare &operator=(Compare &&) = default;
  Compare(TYPE &&v) : value(std::move(v)) {}
  Compare(const TYPE &v) : value(v) {}
  bool operator()(const TYPE &v) { return v == value; }
  TYPE value;
};

template <typename ITERATOR>
uint16_t ContainsTheValue(Compare<typename ITERATOR::value_type> &&compare,
                          ITERATOR begin, ITERATOR end) {
  uint16_t count{0};
  for (ITERATOR ite = begin; ite != end; ++ite) {
    if (compare(*ite)) {
      ++count;
    }
  }
  return count;
}

template <typename ITERATOR>
void TestContainsTheValue(ITERATOR begin, ITERATOR end,
                          const typename ITERATOR::value_type &tested,
                          std::ostream &out = std::cout) {
  using value_type = typename ITERATOR::value_type;
  out << "Number of occurrences of " << tested << " in ";
  Print(begin, end, out);
  out << " is "
      << ContainsTheValue(Compare<value_type>{std::move(tested)}, begin, end)
      << '\n';
}

template <typename ITERATOR>
void TestContainsTheValue(ITERATOR begin, ITERATOR end,
                          typename ITERATOR::value_type &&tested,
                          std::ostream &out = std::cout) {
  using value_type = typename ITERATOR::value_type;
  out << "Number of occurrences of " << tested << " in ";
  Print(begin, end, out);
  out << " is "
      << ContainsTheValue(Compare<value_type>{std::move(tested)}, begin, end)
      << '\n';
}

template <typename CONTAINER>
inline void TestContainsTheValue(const CONTAINER &container,
                                 typename CONTAINER::value_type &&tested,
                                 std::ostream &out = std::cout) {
  using value_type = typename CONTAINER::value_type;
  TestContainsTheValue(container.begin(), container.end(),
                       std::forward<value_type>(tested), out);
}

template <typename CONTAINER>
inline void TestContainsTheValue(const CONTAINER &container,
                                 const typename CONTAINER::value_type &tested,
                                 std::ostream &out = std::cout) {
  TestContainsTheValue(container.begin(), container.end(), tested, out);
}

template <typename CONTAINER>
inline void TestContainsTheValue(CONTAINER &&container,
                                 const typename CONTAINER::value_type &tested,
                                 std::ostream &out = std::cout) {
  TestContainsTheValue(container.begin(), container.end(), tested, out);
}

template <typename CONTAINER>
inline void TestContainsTheValue(CONTAINER &&container,
                                 typename CONTAINER::value_type &&tested,
                                 std::ostream &out = std::cout) {
  using value_type = typename CONTAINER::value_type;
  TestContainsTheValue(container.begin(), container.end(),
                       std::forward<value_type>(tested), out);
}

} // namespace contains

int main() {
  {
    using namespace jobs;

    std::cout << "\n###### jobs\n";
    using Job = Job_t<std::string, std::string, uint16_t>;
    using Programmer = Programmer_t<Job::Name, Job::Description, Job::Hours>;
    using Pilot = Pilot_t<Job::Name, Job::Description, Job::Hours>;
    using Jobs = Jobs_t<Job::Name, Job::Description, Job::Hours>;
    using JobPtr = typename Jobs::value_type;

    Jobs someJobs;
    add_job<Jobs, Programmer>(someJobs, Programmer::Hours{1874});
    add_job<Jobs, Pilot>(someJobs, Pilot::Hours{193});

    traverse_jobs<Jobs, JobPtr>(someJobs, [](JobPtr jobPtr) -> void {
      std::cout << "Name = " << jobPtr->name() << ", description = '"
                << jobPtr->description() << "', hours = " << jobPtr->hours()
                << ". ";
      jobPtr->DoWork(std::cout);
      std::cout << '\n';
    });
  }

  {
    using namespace bounds;

    std::cout << "\n###### bounds\n";
    TestIsInBounds<>(520, 500, 599);
    TestIsInBounds<>(420, 500, 599);
    TestIsInBounds<int16_t>(520, 500, 599);
    TestIsInBounds<int16_t>(420, 500, 599);
    TestIsInBounds<char>('c', 'a', 'e');
    TestIsInBounds<char>('m', 'a', 'e');
    TestIsInBounds<float>(5.20, 5.00, 5.99);
    TestIsInBounds<float>(4.20, 5.00, 5.99);
    TestIsInBounds<int32_t>(-3, -5, 5);
    TestIsInBounds<int32_t>(-8, -5, 5);
    TestIsInBounds<int32_t>(8, -5, 5);
    TestIsInBounds<Color>(Color{20, 30, 40}, Color{10, 20, 30},
                          Color{30, 40, 50});
    TestIsInBounds<Color>(Color{20, 30, 40}, Color{30, 40, 50},
                          Color{80, 90, 100});
  }
  {
    using namespace contains;

    std::cout << "\n###### contains\n";
    auto theStrings{std::vector<std::string>{"one", "two", "test"}};
    TestContainsTheValue(theStrings, "test");

    TestContainsTheValue(std::vector{"one", "two", "test"}, "test");
    TestContainsTheValue(std::vector{2, 8, 1, 15}, 8);
    TestContainsTheValue(std::vector{2, 8, 1, 15, 8}, 8);
    TestContainsTheValue(std::vector{2, 8, 1, 15, 8}, 7);
    TestContainsTheValue(std::vector{Color{10, 100, 200}, Color{100, 23, 90},
                                     Color{90, 87, 182}},
                         Color{100, 23, 90});
    TestContainsTheValue(
        std::list{Color{10, 100, 200}, Color{100, 23, 90}, Color{90, 87, 182}},
        Color{100, 23, 90});
    std::list colors{Color{10, 100, 200}, Color{100, 23, 90},
                     Color{90, 87, 182}};

    TestContainsTheValue(colors.begin(), colors.end(), Color{100, 23, 90});
    TestContainsTheValue(colors, Color{100, 23, 90});
    TestContainsTheValue(
        std::list{
            Color{10, 100, 200},
            Color{100, 23, 90},
            Color{90, 87, 182},
            Color{10, 100, 200},
        },
        Color{10, 100, 200});
    Color c{10, 100, 200};
    TestContainsTheValue(
        std::list{
            Color{10, 100, 200},
            Color{100, 23, 90},
            Color{90, 87, 182},
            Color{10, 100, 200},
        },
        c);
  }
  return 0;
}
