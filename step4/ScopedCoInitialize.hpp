#pragma once

class ScopedCoInitialize final {
public:
    ScopedCoInitialize();
    ~ScopedCoInitialize();
private:
    bool initialized_{false};
};
