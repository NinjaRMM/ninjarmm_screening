/**
 * @file Pilot.hpp
 *
 * @brief Declaration of pilot class.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

#ifndef PILOT_HPP_
#define PILOT_HPP_

/* INCLUDES ******************************************************************/

#include "IJob.hpp"

/* CLASS DECLARATION *********************************************************/

class Pilot : public IJob {
public:
    /**
     * @brief Gets pilot job name.
     *
     * @return The name of the current job.
     */
    std::string GetName() const override;

    /**
     * @brief Gets pilot job description.
     *
     * @return The description of the current job.
     */
    std::string GetDescription() const override;

    /**
     * @brief Gets hours required by a programmer.
     *
     * @return The hours required by the current job.
     */
    int GetHoursRequired() const override;

    /**
     * @brief Starts to execute the job as pilot.
     */
    void DoWork() const override;
};

#endif //PILOT_HPP_
