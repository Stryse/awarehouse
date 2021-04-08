#include "ControllerImpl.h"

ControllerImpl::ControllerImpl()
{
}

ControllerImpl::~ControllerImpl()
{
    networkAdapter.disconnect();
}

void ControllerImpl::tick()
{
}

void ControllerImpl::doActionAssignment()
{
}

const NetworkAdapter& ControllerImpl::getNetworkAdapter() const
{
    return networkAdapter;
}

NetworkAdapter& ControllerImpl::getNetworkAdapter()
{
    return networkAdapter;
}