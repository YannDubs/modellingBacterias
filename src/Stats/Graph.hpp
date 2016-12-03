/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#ifndef INFOSV_GRAPH_HPP
#define INFOSV_GRAPH_HPP

#include <Interface/Drawable.hpp>
#include <Utility/Vec2d.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <unordered_map>
#include <string>
#include <vector>

class Graph : public Drawable
{
public:
    /**
     *  @brief  Construct a graph
     *
     *  @param titles series' titles
     *  @param size   size on screen
     *  @param min    y-axis: min value expected
     *  @param max    y-axis: max value expected
     */
    Graph(std::vector<std::string> const& titles, Vec2d const& size, double min, double max);

    /**
     *  @brief Add some data to the graph
     *
     *  The data given as input is a dictionnary mapping the series' title to the new data.
     *
     *  @param deltaEpoch time since last update
     *  @param newData    the data for the present time
     */
    void updateData(sf::Time deltaEpoch, std::unordered_map<std::string, double> const& newData);

    // Reset the graph
    void reset();

    // Implement Drawable
    virtual void drawOn(sf::RenderTarget& target) const override final;

private:
    struct Serie {
        std::string title;
        sf::Color color;

        // When inserting a new vertex, lastValue must be updated
        double lastValue;

        // We use two buffers: one for the current (present) epoch
        // and one for the previous (past) epoch which we trim everytime
        // we add new data to the current buffer.
        std::vector<sf::Vertex> presentVertices;
        std::vector<sf::Vertex> pastVertices;
    };

private:
    sf::Time mLastEpoch = sf::Time::Zero; ///< Last time data was fetched
    std::vector<Serie> mSeries;           ///< Data
    Vec2d mSize;                          ///< Size of the graph
    double mYMin;                         ///< Y-axis: min value expected
    double mYMax;                         ///< Y-axis: max value expected
};

#endif // INFOSV_GRAPH_HPP
