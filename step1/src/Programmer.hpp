/**
 * @file Programmer.hpp
 *
 * @brief Declaration of programmer class.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

#ifndef PROGRAMMER_HPP_
#define PROGRAMMER_HPP_

/* INCLUDES ******************************************************************/

#include "IJob.hpp"

/* CLASS DECLARATION *********************************************************/

class Programmer : public IJob {
public:
    /**
     * @brief Gets programmer job name.
     *
     * @return The name of the current job.
     */
    std::string GetName() const override;

    /**
     * @brief Gets programmer job name.
     *
     * @return The desciption of the current job.
     */
    std::string GetDescription() const override;

    /**
     * @brief Gets programmer hours required.
     *
     * @return The hours required by the programmer.
     */
    int GetHoursRequired() const override;

    /**
     * @brief Starts to execute the job as programmer.
     */
    void DoWork() const override;
};

#endif //PROGRAMMER_HPP_