/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#ifndef INFOSV_RANDOM_HPP
#define INFOSV_RANDOM_HPP

#include <JSON/JSON.hpp>
#include <Random/RandomGenerator.hpp>
#include <Utility/Vec2d.hpp>

#include <type_traits>
#include <random>

/*!
 * @brief Randomly generate a number from a uniform distribution
 *
 * @param min lower bound
 * @param max upper bound
 * @return a random number fitting the uniform distribution
 */
template <typename T>
T uniform(T min, T max)
{
    using condition = typename std::is_integral<T>;
    using integer_dist = typename std::uniform_int_distribution<T>;
    using real_dist = typename std::uniform_real_distribution<T>;

    using distribution_type = typename std::conditional<condition::value, integer_dist, real_dist>::type;

    distribution_type dist(min, max);

    return dist(getRandomGenerator());
}

template <>
inline Vec2d uniform<Vec2d>(Vec2d topLeft, Vec2d bottomRight)
{
    return { uniform(topLeft.x, bottomRight.x), uniform(topLeft.y, bottomRight.y) };
}

/**
 *  @brief Randomly generate a number from a uniform distribution
 *
 *  @param object A JSON object with 'min' and 'max' key associated with a number
 *
 *  @return a random number
 */
template <typename T = double>
T uniform(j::Value object)
{
    return uniform(object["min"].toDouble(), object["max"].toDouble());
}

// Generate booleans according to Bernoulli's distribution of parameter p
bool bernoulli(double p);

/*!
 * @brief Randomly generate a number from a normal distribution
 *
 * @param mu mean
 * @param sigma2 variance
 * @return a random number fitting the normal(mu, sigma2) distribution
 */
double normal(double mu, double sigma2);

/*!
 * @brief Randomly generate a number from an exponential distribution
 *
 * @param lambda rate
 * @return a random number fitting the exp(lambda) distribution
 */
double exponential(double lambda);

// Generate random numbers from the gamma distribution of parameter (alpha, beta)
double gamma(double alpha, double beta);

#endif // INFOSV_RANDOM_HPP

