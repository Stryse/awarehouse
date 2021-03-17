#ifndef LIB_CONFIG__H
#define LIB_CONFIG__H

// FWD Declarations
class Tile;
template <typename T>
class ObservableEnvironment;
//

namespace config
{
    namespace navigation
    {
        using DefaultVolumeType = Tile;
        using DefaultEnvironment = ObservableEnvironment<DefaultVolumeType>;
    };
};
#endif /* LIB_CONFIG__H */