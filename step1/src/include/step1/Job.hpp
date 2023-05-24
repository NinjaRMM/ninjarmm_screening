#pragma once

namespace step1 {
    class Job {
        public:
            Job();
            Job(Job &&) = default;
            Job(const Job &) = default;
            Job &operator=(Job &&) = default;
            Job &operator=(const Job &) = default;
            ~Job();

        private:

    };
} // namespace step1
