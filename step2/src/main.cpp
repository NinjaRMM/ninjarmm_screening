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

/// \brief Namespace to group code related to jobs classes and functions
namespace jobs {

/// \brief Type parameterized base class for jobs
///
/// \tparam NAME is a string like type used to hold the name of the job
///
/// \tparam DESCRIPTION is a string like type used to hold the description of
/// the job
///
/// \tparam HOURS is a number like type to hold the number of hours necessary
/// for the job
template <typename NAME, typename DESCRIPTION, typename HOURS> class Job_t {
public:
  using Name = NAME;
  using Description = DESCRIPTION;
  using Hours = HOURS;

  /// \brief Pointer type for jobs types
  using Ptr = std::shared_ptr<Job_t<NAME, DESCRIPTION, HOURS>>;

public:
  /// \brief Job_t types can not be created without attribute definition
  Job_t() = delete;

  /// \brief Job_t types can not be copied
  Job_t(const Job_t &) = delete;

  /// \brief Job_t types can not be moved
  Job_t(Job_t &&) = delete;

  /// \brief Job_t types can only be dinamically created
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

  /// \brief Job_t types can not be copied
  Job_t &operator=(const Job_t &) = delete;

  /// \brief Job_t types can not be moved
  Job_t &operator=(Job_t &&) = delete;

  virtual ~Job_t() = default;

protected:
  /// \brief Protected constructor to be used by sub-classes
  Job_t(Name &&pName, Description &&pDescription, Hours pHours)
      : mName(std::move(pName)), mDescription(std::move(pDescription)),
        mHours(pHours) {}

protected:
  Name mName;
  Description mDescription;
  Hours mHours;
};

/// \brief Programmer job type
///
/// \tparam NAME is a string like type used to hold the name of the job
///
/// \tparam DESCRIPTION is a string like type used to hold the description of
/// the job
///
/// \tparam HOURS is a number like type to hold the number of hours necessary
/// for the job
template <typename NAME, typename DESCRIPTION, typename HOURS>
class Programmer_t : public Job_t<NAME, DESCRIPTION, HOURS> {
public:
  /// \brief The amount of hours can be freely defined
  Programmer_t(HOURS pHours)
      : Job_t<NAME, DESCRIPTION, HOURS>("Programmer",
                                        "Writes ninja level programs", pHours) {
  }
};

/// \brief Pilot job type
///
/// \tparam NAME is a string like type used to hold the name of the job
///
/// \tparam DESCRIPTION is a string like type used to hold the description of
/// the job
///
/// \tparam HOURS is a number like type to hold the number of hours necessary
/// for the job
template <typename NAME, typename DESCRIPTION, typename HOURS>
class Pilot_t : public Job_t<NAME, DESCRIPTION, HOURS> {
public:
  /// \brief The amount of hours can be freely defined
  Pilot_t(HOURS pHours)
      : Job_t<NAME, DESCRIPTION, HOURS>("Pilot", "Flies all over the world",
                                        pHours) {}
};

/// \brief Type of collection of (pointer to) jobs
template <typename NAME, typename DESCRIPTION, typename HOURS>
using Jobs_t = std::vector<typename Job_t<NAME, DESCRIPTION, HOURS>::Ptr>;

/// \brief Helper function to add jobs to a collection of jobs
template <typename JOBS, typename JOB>
void add_job(JOBS &pJobs, typename JOB::Hours hours) {
  pJobs.push_back(JOB::template create<JOB>(hours));
}

/// \brief Helper function to traverse a collection of jobs
template <typename JOBS, typename JOB_PTR>
void traverse_jobs(JOBS &pJobs, std::function<void(JOB_PTR)> visit) {

  for (JOB_PTR jobPtr : pJobs) {
    visit(jobPtr);
  }
}
} // namespace jobs

/// \brief A out of context class, to demonstrate template based functions
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

/// \brief Namespace for bound related functions
namespace bounds {

/// \brief Checks is a value is between two others
///
/// \tparam TYPE must implment >= and <=
template <typename TYPE = uint32_t>
bool IsInBounds(TYPE &&value, TYPE &&min, TYPE &&max) {
  return ((value >= min) && (value <= max));
}

/// \brief Helper function to test
template <typename TYPE = uint32_t>
void TestIsInBounds(TYPE &&value, TYPE &&min, TYPE &&max) {
  std::cout << "Is " << value << " between " << min << " and " << max << "? "
            << (IsInBounds(std::forward<TYPE>(value), std::forward<TYPE>(min),
                           std::forward<TYPE>(max))
                    ? "yes"
                    : "no")
            << '\n';
}
} // namespace bounds

/// \brief Namespace grouping function for the 'contains' part of the exercise
namespace contains {

/// \brief Helper function to print a sequence of values
///
/// \tparam ITERATOR is a iterator like type, and the type referenced must
/// implement the << operator
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

/// \brief Compare function object
///
/// \tparam TYPE is the type of the value to be compared, it must implement
/// operator ==
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

/// \brief Determines if a sequence of values contains a value
///
/// \tparam ITERATOR is a iterator like to the value
///
/// \param compare is the function object used to compare values
///
/// \param begin is the begining of the sequence of values
///
/// \param end is the end of the sequence of values
///
/// \return the number of times the value defined in \p compare is found between
/// \p begin and \p end
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

/// \brief Helper function to test \p ContainsTheValue
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

/// \brief Helper function to test \p ContainsTheValue
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

/// \brief Helper function to test \p ContainsTheValue
template <typename CONTAINER>
inline void TestContainsTheValue(const CONTAINER &container,
                                 typename CONTAINER::value_type &&tested,
                                 std::ostream &out = std::cout) {
  using value_type = typename CONTAINER::value_type;
  TestContainsTheValue(container.begin(), container.end(),
                       std::forward<value_type>(tested), out);
}

/// \brief Helper function to test \p ContainsTheValue
template <typename CONTAINER>
inline void TestContainsTheValue(const CONTAINER &container,
                                 const typename CONTAINER::value_type &tested,
                                 std::ostream &out = std::cout) {
  TestContainsTheValue(container.begin(), container.end(), tested, out);
}

/// \brief Helper function to test \p ContainsTheValue
template <typename CONTAINER>
inline void TestContainsTheValue(CONTAINER &&container,
                                 const typename CONTAINER::value_type &tested,
                                 std::ostream &out = std::cout) {
  TestContainsTheValue(container.begin(), container.end(), tested, out);
}

/// \brief Helper function to test \p ContainsTheValue
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
