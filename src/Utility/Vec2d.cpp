/*
 * prjsv 2015
 * 2013, 2014
 * Marco Antognini
 */

#include <Random/Random.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>

#include <cassert>
#include <cmath>

Vec2d Vec2d::fromAngle(double rad)
{
    return { std::cos(rad), std::sin(rad) };
}

Vec2d Vec2d::fromRandomAngle(double min, double max)
{
    return fromAngle(uniform(min, max));
}

Vec2d Vec2d::fromJSON(j::Value json)
{
    return { json["x"].toDouble(), json["y"].toDouble() };
}

Vec2d::Vec2d()
: Vec2d(0.0, 0.0)
{
    // Done.
}

Vec2d::Vec2d(double x_, double y_)
: x(x_)
, y(y_)
{
    // Done.
}

Vec2d::Vec2d(sf::Vector2f const& sfvect)
: Vec2d(sfvect.x, sfvect.y)
{
    // Done.
}

Vec2d::Vec2d(sf::Vector2i const& sfvect)
: Vec2d(sfvect.x, sfvect.y)
{
    // Done.
}

Vec2d::operator sf::Vector2f() const
{
    return { static_cast<float>(x), static_cast<float>(y) };
}

Vec2d::operator sf::Vector2i() const
{
    return { static_cast<int>(x), static_cast<int>(y) };
}

j::Value Vec2d::save() const
{
    auto vector = j::object();

    vector.set("x", j::number(x));
    vector.set("y", j::number(y));

    return vector;
}

Vec2d Vec2d::operator-() const
{
    return { -x, -y };
}

Vec2d Vec2d::operator-(Vec2d const& b) const
{
    return *this + (-b);
}

Vec2d Vec2d::operator+(Vec2d const& b) const
{
    Vec2d ret = *this;
    return ret += b;
}

Vec2d Vec2d::operator*(double c) const
{
    Vec2d ret = *this;
    return ret *= c;
}

Vec2d operator*(double c, Vec2d const& a)
{
    return a * c;
}

Vec2d Vec2d::operator/(double c) const
{
    return *this * (1.0 / c);
}

Vec2d& Vec2d::operator-=(Vec2d const& b)
{
    return *this += (-b);
}

Vec2d& Vec2d::operator+=(Vec2d const& b)
{
    x += b.x;
    y += b.y;
    return *this;
}

Vec2d& Vec2d::operator*=(double c)
{
    x *= c;
    y *= c;
    return *this;
}

Vec2d& Vec2d::operator/=(double c)
{
    return *this *= (1.0 / c);
}

bool Vec2d::operator==(Vec2d const& b) const
{
    return isEqual(x, b.x) && isEqual(y, b.y);
}

bool Vec2d::operator!=(Vec2d const& b) const
{
    return !(*this == b);
}

double& Vec2d::operator[](int axis)
{
    switch (axis) {
    case 0:
        return x;

    case 1:
        return y;

    default:
        // This should not happen
        assert(false);
        break;
    }
}

double Vec2d::operator[](int axis) const
{
    switch (axis) {
    case 0:
        return x;

    case 1:
        return y;

    default:
        // This should not happen
        assert(false);
        break;
    }
}

double Vec2d::lengthSquared() const
{
    return x * x + y * y;
}

double Vec2d::length() const
{
    return std::sqrt(lengthSquared());
}

Vec2d Vec2d::normalised() const
{
    double const len = length();
    return !isEqual(len, 0.0) ? *this / len : *this;
}

Vec2d Vec2d::normal() const
{
    return { y, -x };
}

double distance(Vec2d const& x, Vec2d const& y)
{
    return (x - y).length();
}

Vec2d normal(Vec2d const& a, Vec2d const& b)
{
    return (a - b).normal();
}

double Vec2d::angle() const
{
    return std::atan2(y, x);
}

double Vec2d::dot(Vec2d const& other) const
{
    return x * other.x + y * other.y;
}

int Vec2d::sign(Vec2d const& other) const
{
    if (isEqual(other.lengthSquared(), 0.0) || *this == other) {
        return 0;
    } else {
        return y * other.x > x * other.y ? -1 : 1;
    }
}

std::ostream& operator<<(std::ostream& out, Vec2d const& v)
{
    return out << "(" << v.x << ", " << v.y << ")";
}
