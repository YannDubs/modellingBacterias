/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Utility/DiffEqSolver.hpp>

DiffEqResult stepDiffEq(Vec2d position, Vec2d speed, sf::Time dt, DiffEqFunction const& eq, DiffEqAlgorithm algo)
{
    auto const& ds = dt.asSeconds();

    switch (algo) {
    case DiffEqAlgorithm::EC: {
        auto const& newSpeed = speed + ds * eq.f(position, speed);
        auto const& newPosition = position + ds * newSpeed;
        return { newPosition, newSpeed };
    }

    case DiffEqAlgorithm::RG4: {
        // clang-format off
        auto const& k1  = speed;
        auto const& k1p = eq.f(position, speed);
        auto const& k2  = speed + ds / 2 * k1p;
        auto const& k2p = eq.f(position + ds / 2 * k1, speed + ds / 2 * k1p);
        auto const& k3  = speed + ds / 2 * k2p;
        auto const& k3p = eq.f(position + ds / 2 * k2, speed + ds / 2 * k2p);
        auto const& k4  = speed + ds * k3p;
        auto const& k4p = eq.f(position + ds / 2 * k3, speed + ds / 2 * k3p);

        auto const& newPosition = position + ds / 6 * (k1  + 2 * k2  + 2 * k3  + k4);
        auto const& newSpeed    = speed    + ds / 6 * (k1p + 2 * k2p + 2 * k3p + k4p);
        return { newPosition, newSpeed };
        // clang-format on
    }
    } // switch algo
}
