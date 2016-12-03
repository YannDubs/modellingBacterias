/*
 * prjsv 2015
 * 2013, 2014
 * Marco Antognini
 */

#ifndef INFOSV_UTILITY_HPP
#define INFOSV_UTILITY_HPP

#include <Utility/Types.hpp>
#include <Utility/Vec2d.hpp>

#include <SFML/Graphics.hpp>

#include <string>
#include <utility> // std::pair
#include <vector>

/*!
 * @brief Create a new unique id.
 *
 * @return a new unique id.
 */
Uid createUid();

/*!
 * @brief Construct a sf::Sprite object fully set up.
 *
 * @param position the position of the centre of the sprite
 * @param size size of the side
 * @param texture the texture
 *
 * @return a sprite with all these parameters set
 */
sf::Sprite buildSprite(Vec2d const& position, double size, sf::Texture const& texture);

/*!
 * @brief Construct a sf::Text object fully set up.
 *
 * @param msg text to display
 * @param position centre position of the text
 * @param font the font to use
 * @param size the font size to use
 * @param color the color of the text
 *
 * @return a text with all these parameters set
 */
sf::Text buildText(std::string const& msg, Vec2d const& position, sf::Font const& font, unsigned int size,
                   sf::Color color);

/*!
 * @brief Construct a circle with a sf::CircleShape.
 *
 * @param position the position of the centre of the circle
 * @param radius the radius of the circle
 * @param color the color of the circle
 *
 * @return a shape with all these parameters set
 */
sf::CircleShape buildCircle(Vec2d const& position, double radius, sf::Color color);

/*!
 * @brief Construct an annulus with a sf::CircleShape.
 *
 * @param position position of the centre of the ring
 * @param radius inner radius of the ring
 * @param color the color of the ring
 * @param thickness width of the ring
 *
 * @return a shape with all these parameters set
 */
sf::CircleShape buildAnnulus(Vec2d const& position, double radius, sf::Color color, double thickness);

/*!
 * @brief Construct a square with a sf::RectangleShape.
 *
 * @param position the centre of the square
 * @param side size of a side of the square
 * @param color the color of the square
 *
 * @return a shape with all these parameters set
 */
sf::RectangleShape buildSquare(Vec2d const& position, double side, sf::Color color);

/**
 *  @brief Construct
 *
 *  @param start     start point of the line
 *  @param end       end point of the line
 *  @param color     color of the line
 *  @param thickness thickness of the line
 *
 *  @return a shape that represents a line
 */
sf::RectangleShape buildLine(Vec2d const& start, Vec2d const& end, sf::Color color, double thickness);

/*!
 * @brief Check if x is equal to y, with a predefined precision.
 *
 * @param x a real number
 * @param y another real number
 *
 * @return true is the difference between x and y is less than a small epsilon
 */
bool isEqual(double x, double y);

/*!
 * @brief Check if x is equal to y with a precision of epsilon.
 *
 * @param x a real number
 * @param y another real number
 * @param epsilon precision of the equality test
 *
 * @return true if the difference between x and y is less than the given epsilon
 */
bool isEqual(double x, double y, double epsilon);

/*!
 * @brief Compute the delta between two angles in radians.
 *
 * @param a an angle (rad)
 * @param b a second angle (rad)
 *
 * @return (a-b) in [-PI, PI]
 */
double angleDelta(double a, double b);

/*!
 * @brief Split a string.
 *
 * @param str string to split
 * @param delim char delimiter
 *
 * @return an array of the tokens
 */
std::vector<std::string> split(std::string const& str, char delim);

/*!
 * @brief Copy src at the end of dest.
 *
 * @param src the data that should be copied
 * @param dest the data is appended to this container
 *
 * @tparam C a container like std::vector<T>
 * @tparam D another container of T.
 */
template <typename C, typename D>
void append(C const& src, D& dest);

/*!
 * @brief Copy [first, last) at the end of dest.
 *
 * @param first start of the data
 * @param last iterator after the last element to copy
 * @param dest the data is appended to this container
 *
 * @tparam C a container like std::vector<T>
 * @tparam InputIt an input iterator with element type T
 */
template <typename C, typename InputIt>
void append(InputIt first, InputIt last, C& dest);

/*!
 * @brief Implement erase + remove_if equivalent for std::map.
 *
 * @link http://stackoverflow.com/a/7008476
 */
template <typename Map, typename F>
void map_erase_if(Map& m, F pred);

#include "Utility.tpp"

#endif // INFOSV_UTILITY_HPP
