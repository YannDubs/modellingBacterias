/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Random/Random.hpp>

bool bernoulli(double p)
{
    std::bernoulli_distribution dist(p);

    return dist(getRandomGenerator());
}

double normal(double mu, double sigma2)
{
    std::normal_distribution<double> dist(mu, std::sqrt(sigma2));

    return dist(getRandomGenerator());
}

double exponential(double lambda)
{
    std::exponential_distribution<double> dist(lambda);

    return dist(getRandomGenerator());
}

double gamma(double alpha, double beta)
{
    std::gamma_distribution<double> dist(alpha, beta);

    return dist(getRandomGenerator());
}
