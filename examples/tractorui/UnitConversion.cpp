#include "UnitConversion.h"

// 1 kilometer/hour = 0.621371 miles/hour
// 1 mile/hour = 1.60934 kilometers/hour
// Celsius to Fahrenheit: (°C × 9/5) + 32 = °F
// Fahrenheit to Celsius: (°F − 32) × 5/9 = °C
// 1 liter = 0.264172 gallons
// 1 gallon = 3.78541 liters

double UnitConversion::kmhToMph(double kmh)
{
    return kmh * 0.621371;
}

double UnitConversion::mphToKmh(double mph)
{
    return mph * 1.60934;
}

double UnitConversion::celsiusToFahrenheit(double celsius)
{
    return (celsius * 9.0 / 5.0) + 32.0;
}

double UnitConversion::fahrenheitToCelsius(double fahrenheit)
{
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

double UnitConversion::litersToGallons(double liters)
{
    return liters * 0.264172;
}

double UnitConversion::gallonsToLiters(double gallons)
{
    return gallons * 3.78541;
} 

double UnitConversion::litersPer100KmToMilesPerGallon(double litersPer100Km)
{
    return 100.0 / litersPer100Km;
}

double UnitConversion::milesPerGallonToLitersPer100Km(double milesPerGallon)
{
    return 100.0 / milesPerGallon;
}

double UnitConversion::kilometersToMiles(double kilometers)
{
    return kilometers * 0.621371;
}

double UnitConversion::milesToKilometers(double miles)
{
    return miles * 1.60934;
}

double UnitConversion::kilogramsToPounds(double kilograms)
{
    return kilograms * 2.20462;
}

double UnitConversion::poundsToKilograms(double pounds)
{
    return pounds * 0.453592;
}

UnitConversion::Unit UnitConversion::unit() const
{
    QLocale locale;
    switch (m_unitType) {
        case UnitType::Speed:
            return locale.measurementSystem() == QLocale::MetricSystem ? Unit::Kmh : Unit::Mph;
        case UnitType::Temperature:
            return locale.measurementSystem() == QLocale::MetricSystem ? Unit::Celsius : Unit::Fahrenheit;
        case UnitType::Volume:
            return locale.measurementSystem() == QLocale::MetricSystem ? Unit::Liters : Unit::Gallons;
        case UnitType::Weight:
            return locale.measurementSystem() == QLocale::MetricSystem ? Unit::Kilograms : Unit::Pounds;
        case UnitType::FuelConsumption:
            return locale.measurementSystem() == QLocale::MetricSystem ? Unit::LitersPer100Km : Unit::MilesPerGallon;
        case UnitType::Distance:
            return locale.measurementSystem() == QLocale::MetricSystem ? Unit::Kilometers : Unit::Miles;
        case UnitType::None:
            return Unit::None;
    }
}

QString UnitConversion::unitString() const
{
    switch (unit()) {
        case Unit::Kmh:
            return tr("km/h");
        case Unit::Mph:
            return tr("mph");
        case Unit::Celsius:
            return tr("°C");
        case Unit::Fahrenheit:
            return tr("°F");
        case Unit::Liters:
            return tr("L");
        case Unit::Gallons:
            return tr("gal");
        case Unit::Kilograms:
            return tr("kg");
        case Unit::Pounds:
            return tr("lb");
        case Unit::LitersPer100Km:
            return tr("Liters per 100km");
        case Unit::MilesPerGallon:
            return tr("Miles per gallon");
        case Unit::Kilometers:
            return tr("km");
        case Unit::Miles:
            return tr("mi");
        default:
            return m_unitString;
    }
}

void UnitConversion::setExplicitUnitString(const QString& unitString)
{
    if (unit() == Unit::None)
    {
        m_unitString = unitString;
    }
    else
    {
        qWarning() << "ignoring setting unit string for unit type" << m_unitType;
    }
}

qreal UnitConversion::convertedValue(qreal value) const
{
    switch (m_unitType) {
        case UnitType::Speed:
            return unit() == Unit::Kmh ? value : kmhToMph(value);
        case UnitType::Temperature:
            return unit() == Unit::Celsius ? value : celsiusToFahrenheit(value);
        case UnitType::Volume:
            return unit() == Unit::Liters ? value : litersToGallons(value);
        case UnitType::Weight:
            return unit() == Unit::Kilograms ? value : kilogramsToPounds(value);
        case UnitType::FuelConsumption:
            return unit() == Unit::LitersPer100Km ? value : milesPerGallonToLitersPer100Km(value);
        case UnitType::Distance:
            return unit() == Unit::Kilometers ? value : milesToKilometers(value);
        case UnitType::None:
            return value;
    }
}

QskIntervalF UnitConversion::convertedInterval(const QskIntervalF& interval) const
{
    return { convertedValue( interval.lowerBound() ), convertedValue( interval.upperBound() ) };
}   