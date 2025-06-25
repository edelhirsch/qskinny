#pragma once

#include <QskIntervalF.h>

#include <QLocale>
#include <QCoreApplication>

class UnitConversion
{
    Q_GADGET

    Q_DECLARE_TR_FUNCTIONS(UnitConversion)

public:
    enum class UnitType
    {
        None,
        Speed,
        Temperature,
        Volume,
        Weight,
        FuelConsumption,
        Distance,
    };
    Q_ENUM(UnitType)

    enum class Unit
    {
        None,
        Kmh,
        Mph,
        Celsius,
        Fahrenheit,
        Liters,
        Gallons,
        Kilograms,
        Pounds,
        LitersPer100Km,
        MilesPerGallon,
        Kilometers,
        Miles,
    };
    Q_ENUM(Unit)

    UnitConversion() : m_unitType(UnitType::None)
    {
    }

    UnitConversion(UnitType unitType) : m_unitType(unitType)
    {
    }

    Unit unit() const;
    void setUnitType(UnitType unitType);
    QString unitString() const;
    void setExplicitUnitString(const QString& unitString);
    qreal convertedValue(qreal value) const;
    QskIntervalF convertedInterval(const QskIntervalF& interval) const;

private:
    UnitType m_unitType;
    QString m_unitString;

    // Static conversion methods
    static double kmhToMph(double kmh);
    static double mphToKmh(double mph);
    static double celsiusToFahrenheit(double celsius);
    static double fahrenheitToCelsius(double fahrenheit);
    static double litersToGallons(double liters);
    static double gallonsToLiters(double gallons);
    static double kilogramsToPounds(double kilograms);
    static double poundsToKilograms(double pounds);
    static double litersPer100KmToMilesPerGallon(double litersPer100Km);
    static double milesPerGallonToLitersPer100Km(double milesPerGallon);
    static double kilometersToMiles(double kilometers);
    static double milesToKilometers(double miles);
}; 