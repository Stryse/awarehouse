#ifndef I_OBSERVABLE_ENVIRONMENT__H
#define I_OBSERVABLE_ENVIRONMENT__H

//Forward Declarations
class Info;
template <typename T>
class DirectionVector;
template <typename T>
class Point;
//

/**
 * @brief Egy megfigyelhető környezet adatszerkezet interfésze.
 * 
 * Olyan környezetet ír le, amelyből információ nyerhető különböző módokon.
 */
class IObservableEnvironment
{
    using DirectionVector = DirectionVector<int>;
    using Point = Point<int>;

public:
    virtual ~IObservableEnvironment() {}

    /**
     * @brief Sugárvetés adott irányba, 
     * visszaadja a sugár által végighaladt elemek információját.
     * @param direction Sugár iránya
     * @param depth Sugárvetés mélysége
     * @return Info* Megfigyelt információ
     */
    virtual Info *rayCast(const DirectionVector &direction, int depth) const = 0;

    /**
     * @brief Adott középpontból minden irányba letapogatás adott távolságig
     * 
     * @param center Szkennelés középpontja
     * @param range Maximum távolság
     * @return Info* Megfigyelt információ
     */
    virtual Info *scan(const Point &center, int range) const = 0;

    /**
     * @brief Konkrét pont megfigyelése
     * 
     * @param targetPoint Megfigyelendő pont
     * @return Info* Megfigyelt információ
     */
    virtual Info *getInfoAtPoint(const Point &targetPoint) const = 0;
};

#endif