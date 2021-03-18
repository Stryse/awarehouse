#ifndef I_DEPLETING__H
#define I_DEPLETING__H

template <typename Energy>
class IDepleting
{
public:
    virtual void charge(const Energy &energy) = 0;
    virtual void deplete(const Energy &energy) = 0;
    virtual const Energy &getCharge() const = 0;
    virtual const Energy &getMaxCharge() const = 0;
    virtual bool isDepleted() const { return getCharge() == 0; }
};

#endif /* I_DEPLETING__H */