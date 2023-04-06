/**
 * @file IJob.hpp
 *
 * @brief Declaration of job interface.
 *
 * @author   gustavo.garcia
 * @date     Apr 02, 2023
 * @revision 1
 *
 */

#ifndef IJOB_HPP_
#define IJOB_HPP_

/* INCLUDES ******************************************************************/

#include <string>

/* DEFINITIONS ***************************************************************/

class IJob {
public:
    virtual std::string GetName() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual int GetHoursRequired() const = 0;
    virtual void DoWork() const = 0;

};

#endif //IJOB_HPP_