#ifndef LIB_CONFIG__H
#define LIB_CONFIG__H

// FWD Declarations
class Tile;
//

/*******************************************************
 * @brief Contains default parameters of the library.
 *******************************************************/
namespace config
{
    namespace navigation
    {
        using DefaultVolumeType = Tile;
    };
    namespace agent
    {
        using DefaultEnergy = int;
    };
};
#endif /* LIB_CONFIG__H */
