/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#ifndef INFOSV_DIFFEQSOLVER_HPP
#define INFOSV_DIFFEQSOLVER_HPP

#include <Utility/Vec2d.hpp>

#include <SFML/System.hpp>

#include <functional>

struct DiffEqResult {
    Vec2d position;
    Vec2d speed;
};

// Inherit from this class to define the force
class DiffEqFunction
{
public:
    virtual Vec2d f(Vec2d position, Vec2d speed) const = 0;

    virtual ~DiffEqFunction() = default;
};

enum class DiffEqAlgorithm {
    RG4, // Runge-Kutta fourth-order method
    EC   // Euler-Cromer, quite unstable
};

/**
 *  @brief Numerical integration of differencial equation
 *
 *  @param position current position, x
 *  @param speed    current speed, ẋ
 *  @param dt       small time step, ∆t
 *  @param eq       force function, f
 *  @param algo     numerical integrator to use
 *
 *  @return the position and speed after dt
 */
DiffEqResult stepDiffEq(Vec2d position, Vec2d speed, sf::Time dt, DiffEqFunction const& eq,
                        DiffEqAlgorithm algo = DiffEqAlgorithm::RG4);

#endif // INFOSV_DIFFEQSOLVER_HPP
