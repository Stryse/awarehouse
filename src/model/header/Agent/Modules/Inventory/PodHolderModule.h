#ifndef POD_HOLDER_MODULE__H
#define POD_HOLDER_MODULE__H

#include "PodHolder.h"

template <typename TEnvironment>
class PodHolderModule
{
public:
    using Environment = TEnvironment;
    using PodHolder = ::PodHolder<Environment>;

public:
    // Getter
    const PodHolder &getPodHolder() const { return podHolder; }
    PodHolder &getPodHolder() { return podHolder; }

private:
    PodHolder podHolder;
};

#endif /* POD_HOLDER_MODULE__H */