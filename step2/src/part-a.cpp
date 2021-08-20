#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <memory>

struct Job{
  using ptr = std::shared_ptr<Job>;
  using vector = std::vector<ptr>;
  void DoWork() const noexcept{
    std::cout << "My work involved " << Description();
  }
  virtual const char* Description() const noexcept = 0;
  virtual const char* Name() const noexcept = 0;
protected:
  std::chrono::hours _duration;
};

struct Programmer :  Job{
  virtual const char* Description() const noexcept override { return "Programmer Job"; }
  virtual const char* Name() const noexcept override { return "Programmer"; }
};

struct Pilot : Job {
  virtual const char* Description() const noexcept override { return "Pilot Job"; }
  virtual const char* Name() const noexcept override { return "Pilot"; }
};

template <typename _ty> bool IsInBounds(_ty Test, _ty LowerBound, _ty UpperBound) noexcept {
  auto bRet = std::islessequal<_ty>(Test, UpperBound) &&
    std::isgreaterequal<_ty>(Test, LowerBound);
  std::cout << Test << (bRet ? " is between " : " is not between ") << LowerBound << " and " << UpperBound << '\n';
  return bRet;
}

template <typename _fn_t> uint32_t ContainsTheString(_fn_t fn, std::vector<std::string>& theStrings) {
  uint32_t iRet = 0;
  for (const auto& oString : theStrings) { if (fn(oString)) ++iRet; }
  return iRet;
}


int main() {
  Job::vector oJobs;
  oJobs.push_back(std::make_shared<Programmer>());
  oJobs.push_back(std::make_shared<Pilot>());
  for (const auto& oJob : oJobs) {
    std::cout << oJob->Name() << " : " << oJob->Description() << '\n';
    oJob->DoWork();
  }

  for (uint32_t httpResonseCode = 495; httpResonseCode < 505; ++httpResonseCode) {
    IsInBounds<uint32_t>(httpResonseCode, 500, 599);
  }

  auto theStrings = std::vector<std::string>{ "one", "two",  "test"};
  auto count = ContainsTheString([](const std::string& tested) { return tested == "test"; }, theStrings);
  std::cout << "ContainsTheString('test', theStrings) returned " << count << '\n';
  return 0;
}

