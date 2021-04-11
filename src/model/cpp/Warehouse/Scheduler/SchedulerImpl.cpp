#include "SchedulerImpl.h"

SchedulerImpl::SchedulerImpl()
{
}

SchedulerImpl::~SchedulerImpl()
{
    networkAdapter.disconnect();
}

void SchedulerImpl::tick(int timeStamp)
{
}

void SchedulerImpl::doTaskAssignment()
{
}

const NetworkAdapter &SchedulerImpl::getNetworkAdapter() const
{
    return networkAdapter;
}

NetworkAdapter &SchedulerImpl::getNetworkAdapter()
{
    return networkAdapter;
}