#ifndef POD_HOLDER_MODULE__H
#define POD_HOLDER_MODULE__H

#include "PodHolder.h"

/*******************************************************
 * @brief A module which contains a pod holder.
 * An agent can derive from this class so it can use
 * and forward a pod holder. A pod holder can store
 * a pod in its inventory and it can transfer it to another
 * pod holding entity.
 *******************************************************/
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