#ifndef IINFO_PROVIDER__H
#define IINFO_PROVIDER__H

template <typename InfoType>
class IInfoProvider
{
public:
    virtual void getInfo(InfoType &target) const = 0;
};

template <typename... InfoTypes>
class IMultiInfoProvider : public IInfoProvider<InfoTypes>...
{
};

#endif /* IINFO_PROVIDER__H */