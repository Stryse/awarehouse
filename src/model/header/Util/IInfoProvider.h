#ifndef IINFO_PROVIDER__H
#define IINFO_PROVIDER__H

/**************************************************************
 * @brief Interface which can provide info of type provided
 * in template argument
 * 
 * @tparam InfoType Type of info the implementor can provide.
 **************************************************************/
template <typename TInfoType>
class IInfoProvider
{
public:
    using InfoType = TInfoType;

public:
    /***********************************************************
     * @brief Returns information of type InfoType of the object
     * 
     * @param target The ouput parameter in which info will be written
     ***********************************************************/
    virtual void getInfo(InfoType &target) const = 0;
};

/****************************************************************
 * @brief Interface which can provide multiple types of infos.
 * 
 * @tparam InfoTypes Types of infos the implementor can provide.
 ****************************************************************/
template <typename... InfoTypes>
class IMultiInfoProvider : public IInfoProvider<InfoTypes>...
{
};

#endif /* IINFO_PROVIDER__H */