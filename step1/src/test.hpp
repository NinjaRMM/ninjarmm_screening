#include <cstdint>
#include <functional>
#include <string>
#include <vector>

// Contains the definition of Job, Programmer and Pilot classes; the templates
// for validating HTTP status codes and an API to return the number of strings
// that satisfies a predicate.

class Job {
public:
  enum class Status {
    kUninitialized,
    kProgrammerJobCreated,
    kProgrammerJobStarted,
    kProgrammerJobStopped,
    kPilotJobCreated,
    kPilotJobStarted,
    kPilotJobStopped,
  };

  virtual void   create() = 0;
  virtual void   start () = 0;
  virtual void   stop  () = 0;
  virtual Status status() const = 0;
protected:
  Status status_m = Status::kUninitialized;
};

class Programmer : public Job {
public:
  void   create() override { status_m = Status::kProgrammerJobCreated; }
  void   start () override { status_m = Status::kProgrammerJobStarted; }
  void   stop  () override { status_m = Status::kProgrammerJobStopped; }
  Status status() const override { return status_m; }
};

class Pilot : public Job {
public:
  void   create() override { status_m = Status::kPilotJobCreated; }
  void   start () override { status_m = Status::kPilotJobStarted; }
  void   stop  () override { status_m = Status::kPilotJobStopped; }
  Status status() const override { return status_m; }
};

uint32_t containsTheString(
  std::function<bool(const std::string&)> test,
  const std::vector<std::string>&         theStrings
);

template<typename T>
bool IsInBound(T value, T from , T to) {
  return value >= from && value <= to;
}

// I don't find this useful but accepts an hypothetical variable number
// of HTTP status codes and returns TRUE if ALL of them are valid status
// codes.
template<typename... T>
bool IsValidHttpResponseCode(T ... responseCode) {
  bool valid = true;

  (
    [&] {
      valid = valid && IsInBound(responseCode, 100, 599);
    } (),
    ...
  );

  return valid;
}
