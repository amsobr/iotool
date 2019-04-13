//
// Created by to on 12/04/19.
//

#include "output_channel_manager.hpp"

OutputChannelManager::~OutputChannelManager()
{

}

void OutputChannelManager::workerFunction()
{
    while( !myStopped ) {
        DataBucketPtr bucketPtr = myQueue.getNext();

        for ( auto channel : myOutputChannels ) {
            if ( channel->isCalled(bucketPtr->name) ) {
                channel->incomingBucket(bucketPtr);
            }
        }

    }
}
