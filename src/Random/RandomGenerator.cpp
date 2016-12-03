/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Random/RandomGenerator.hpp>

std::mt19937& getRandomGenerator()
{
    static std::mt19937 algo;

    static bool initialise = true;
    if (initialise) {
        initialise = false;
        std::random_device rd;
        algo.seed(rd());
    }

    return algo;
}
