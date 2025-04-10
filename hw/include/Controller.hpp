#pragma once

#include "ControllerBase.hpp"
#include <memory>
#include <queue>

namespace sim
{

template<typename DutT, typename StimulusT = int>
class Controller : public ControllerBase<DutT>
{
public:

    using QueueT = std::queue<StimulusT>;

    Controller() : theQueue{}, theDevice{} {};

    void init(std::shared_ptr<DutT> aDevice)
    override
    {
        theDevice = aDevice;
    }

    bool isControllerEmpty()
    override
    {
        return theQueue.empty();
    };

    StimulusT pop()
    {
        auto myFront = theQueue.front();
        theQueue.pop();
        return myFront;
    }

    StimulusT front() const
    {
        return theQueue.front();
    }

    void add(StimulusT anElement)
    {
        theQueue.push(anElement);
    }

    QueueT & getQueue()
    {
        return this->theQueue;
    }

    virtual ~Controller() {};

protected:
    std::shared_ptr<DutT> theDevice;
    QueueT theQueue;
};


}
