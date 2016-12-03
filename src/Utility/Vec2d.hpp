/*
 * prjsv 2015
 * 2013, 2014
 * Marco Antognini
 */

#ifndef INFOSV_VEC2D_HPP
#define INFOSV_VEC2D_HPP

#include <JSON/JSON.hpp>
#include <Utility/Constants.hpp>

#include <SFML/System.hpp>

/*!
 * @class Vec2d
 *
 * @brief Bridge vector class to sf::Vector2i/f that provides some common math methods.
 *
 * It can be implicitly constructed from a sf::Vector2i or sf::Vector2f
 * and can be converted implicitly to those same types.
 */
class Vec2d
{
public:
    /*!
     * @brief Create a unitary Vec2d from an angle
     *
     * @param rad the angle
     * @return the corresponding Vec2d
     */
    static Vec2d fromAngle(double rad);

    /*!
     *  @brief Create a random vector
     *
     *  @param min minimal angle in rad
     *  @param max maximal angle in rad
     *
     *  @return a random vector
     */
    static Vec2d fromRandomAngle(double min = 0, double max = TAU);

    /*!
     * @brief Construct a vector from its internal representation in JSON format
     *
     * @throw j::BadConversion or j::NoSuchElement if internal format is not valid
     *
     * @see save
     */
    static Vec2d fromJSON(j::Value json);

public:
    Vec2d();
    Vec2d(double x, double y);

    Vec2d(sf::Vector2f const& sfvect);
    Vec2d(sf::Vector2i const& sfvect);

    Vec2d(Vec2d const& other) = default;

    // We don't need:
    //    Vec2d& operator=(sf::Vector2f const& sfvect);
    //    Vec2d& operator=(sf::Vector2i const& sfvect);
    // Because C++ will automatically construct a Vec2d
    // with the appropriate constructor and the following
    // assignment operator will be called:

    Vec2d& operator=(Vec2d const& other) = default;

    operator sf::Vector2f() const;
    operator sf::Vector2i() const;

    /*!
     * @brief Save internal representation as JSON value
     */
    j::Value save() const;

    /*!
     * @brief Computes the length of the vector (squared).
     *
     * @return the square module of this
     */
    double lengthSquared() const;

    /*!
     * @brief Computes the length of the vector.
     *
     * @return the module of this
     */
    double length() const;

    /*!
     * @brief Computes the normalized vector.
     *
     * @return w such that |w| and w * |this| = this
     */
    Vec2d normalised() const;

    /*!
     * @brief Computes the normal (orthogonal) vector for this vector.
     *
     * @return n such that this · n = 0
     */
    Vec2d normal() const;

    /*!
     * @brief Computes the angle of the vector in polar coordinates.
     *
     * @return the angle of this, in [-PI, PI]
     */
    double angle() const;

    /*!
     * @brief Computes the dot product between this and another vector.
     *
     * @param other another vector
     * @return the inner product
     */
    double dot(Vec2d const& other) const;

    /*!
     * @brief Compares the angle between another vector and this one.
     *
     * @return 1 if other is clockwise of this vector, -1 otherwise (anticlockwise),
     *         or 0 if other is null or equal to this vector.
     */
    int sign(Vec2d const& other) const;

    Vec2d operator-() const; ///< Negation
    Vec2d operator-(Vec2d const& b) const;
    Vec2d operator+(Vec2d const& b) const;
    Vec2d operator*(double c) const;
    Vec2d operator/(double c) const;

    Vec2d& operator-=(Vec2d const& b);
    Vec2d& operator+=(Vec2d const& b);
    Vec2d& operator*=(double c);
    Vec2d& operator/=(double c);

    bool operator==(Vec2d const& b) const;
    bool operator!=(Vec2d const& b) const;

    /*!
     * @brief Accesses the coordinates by dimension, read-write.
     *
     * @param axis only value 0 and 1 are allowed
     * @return x if axis is 0, y if axis if 1, undefined otherwise
     */
    double& operator[](int axis);

    /*!
     * @brief Accesses the coordinates by dimension, read-only.
     *
     * @param axis only value 0 and 1 are allowed
     * @return x if axis is 0, y if axis if 1, undefined otherwise
     */
    double operator[](int axis) const;

public:
    double x, y; ///< DATA
};


Vec2d operator*(double c, Vec2d const& a);


/*!
 * @brief Computes the distance between two given points.
 *
 * @param x a point
 * @param y another point
 * @param the distance between x and y
 */
double distance(Vec2d const& x, Vec2d const& y);

/*!
 * @brief Computes the normal vector of a line segment.
 *
 * @param a beginning of the segment
 * @param b end of the segment
 * @return normal vector of [a, b]
 */
Vec2d normal(Vec2d const& a, Vec2d const& b);

std::ostream& operator<<(std::ostream& out, Vec2d const& v);

#endif // INFOSV_VEC2D_HPP
