#ifndef POD_HOLDER_MODULE__H
#define POD_HOLDER_MODULE__H

#include "PodHolder.h"

class PodHolderModule
{
public:
    // Getter
    const PodHolder &getPodHolder() const { return podHolder; }
    PodHolder &getPodHolder() { return podHolder; }

private:
    PodHolder podHolder;
};

#endif /* POD_HOLDER_MODULE__H */