#ifndef LIB_CONFIG__H
#define LIB_CONFIG__H

// FWD Declarations
class Tile;
template <typename T>
class ObservableNavEnvironment;
//

namespace config
{
    namespace navigation
    {
        using DefaultVolumeType = Tile;
        using DefaultEnvironment = ObservableNavEnvironment<DefaultVolumeType>;
    };
    namespace agent
    {
        using DefaultEnergy = int;
    };
};
#endif /* LIB_CONFIG__H */