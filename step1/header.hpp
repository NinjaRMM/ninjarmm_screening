#include <string>
#include <functional>
#include <algorithm>

class Job {
  public:
    Job() {  };
    virtual ~Job() {  };

    virtual void performTask() const noexcept = 0;
    virtual void isEmployed() const noexcept = 0;
    virtual void helpTeammate() const noexcept = 0;
    virtual void attendMeeting() const noexcept = 0;
};

class Programmer : public Job {
  public:
    Programmer(const std::string& firstName, const std::string& lastName)
      : m_firstName { firstName }, m_lastName { lastName } {  };
    ~Programmer() {  };
  
    void performTask() const noexcept override { };
    void isEmployed() const noexcept override {  };
    void helpTeammate() const noexcept override {  };
    void attendMeeting() const noexcept override {  };

    const std::string& GetFirstName() const noexcept { return m_firstName; };
    void SetFirstName(const std::string& firstName) { m_firstName = firstName; };
    const std::string& GetLastName() const noexcept { return m_lastName; };
    void SetLastName(const std::string& lastName) { m_lastName = lastName; };
  
  private:
    std::string m_firstName { "" };
    std::string m_lastName { "" };
};

class Pilot : public Job {
  public:
    Pilot() {  };
    ~Pilot() {  };

    void performTask() const noexcept override {  };
    void isEmployed() const noexcept override {  };
    void helpTeammate() const noexcept override {  };
    void attendMeeting() const noexcept override {  };
};

template<typename T>
bool IsInBounds(const T& httpResponseCode, const T& min, const T& max) {
  return httpResponseCode >= min && httpResponseCode <= max;
}

size_t ContainsTheString(std::function<bool(const std::string&)> comp, const std::vector<std::string>& theStrings) {
  return std::count_if(theStrings.begin(), theStrings.end(), comp);
} 