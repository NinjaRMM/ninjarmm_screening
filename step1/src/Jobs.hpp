#include <string>

namespace {
const uint PILOT_STRESS_LIMIT = 100;
const uint PILOT_STRESS_PORTION = 50;

const uint PROGRAMMER_STRESS_LIMIT = 200;
const uint PROGRAMMER_STRESS_PORTION = 10;
}

class Job {
public:

  Job(const uint stressLimit, const uint stressPortion)
  : m_stressLevel{0}
  , m_stressPortion{stressPortion}
  , m_stressLimit{stressLimit}
  {
  }

  virtual ~Job(){};

  uint currentStressLevel() const {
    return m_stressLevel;
  }

  bool needVacation() const {
    return m_stressLevel > m_stressLimit;
  }

  virtual void makeDecision(const bool isUnderStress){
    if (isUnderStress) {
      m_stressLevel =  m_stressLevel + m_stressPortion;
    }
  }

  virtual std::string flexOnParty() const = 0;

  protected:
  uint m_stressLevel;
  const uint m_stressPortion;
  
private:
  const uint m_stressLimit;
};

class Pilot : public Job {

public:
  Pilot()
  : Job(PILOT_STRESS_LIMIT, PILOT_STRESS_PORTION)
  {
  }

  std::string flexOnParty() const override {
    return "Gosh, tomorrow I'm scheduled to Paris!";
  }
};

class Programmer : public Job {

public:

  Programmer(const bool hasStandingDesk)
  : Job(PROGRAMMER_STRESS_LIMIT, PROGRAMMER_STRESS_PORTION)
  , m_hasStandingDesk{hasStandingDesk}
  {
  }

  void makeDecision(const bool isUnderStress) {
    if (isUnderStress && !m_hasStandingDesk) {
      m_stressLevel = m_stressLevel + m_stressPortion;
    }
  }

  std::string flexOnParty() const override {
    return "By the way, I use Arch!";
  }

private:

  const bool m_hasStandingDesk;

};


