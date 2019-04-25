//
// Created by to on 19/04/19.
//

#ifndef IOTOOL_ACQUISITION_SCHEDULER_HPP
#define IOTOOL_ACQUISITION_SCHEDULER_HPP


#include <memory>

class AcquisitionScheduler
{
public:

    virtual void start() = 0;
    virtual void stop() = 0;

    typedef std::shared_ptr<AcquisitionScheduler> Ptr;
};


#endif //IOTOOL_ACQUISITION_SCHEDULER_HPP
