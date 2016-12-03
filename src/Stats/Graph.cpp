/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Application.hpp>
#include <Stats/Graph.hpp>

#include <algorithm>
#include <cassert>

namespace
{

std::vector<sf::Color> const& COLORS = { sf::Color::Red, sf::Color::Green, sf::Color::Yellow, sf::Color::Cyan,
                                         sf::Color::White, sf::Color::Magenta };

} // anonymous

Graph::Graph(std::vector<std::string> const& titles, Vec2d const& size, double min, double max)
: mSize(size)
, mYMin(std::min(max, min))
, mYMax(std::max(max, min))
{
    assert(titles.size() <= COLORS.size());

    for (std::size_t i = 0; i < titles.size(); ++i) {
        mSeries.push_back({ titles[i], COLORS[i], mYMin }); // with empty vertices vectors
    }
}

void Graph::updateData(sf::Time deltaEpoch, std::unordered_map<std::string, double> const& newData)
{
    auto newEpoch = mLastEpoch + deltaEpoch;

    auto const X_SCALE = 10.0; // The x-axis scale is 10px per second
    auto const Y_SCALE = 1.0 / (mYMax - mYMin) * mSize.y;
    auto const TIME_SCALE = sf::seconds(mSize.x / X_SCALE);

    // Reset epoch if needed
    while (newEpoch > TIME_SCALE) {
        newEpoch -= TIME_SCALE;
    }

    for (auto& serie : mSeries) {
        auto x = newEpoch.asSeconds() * X_SCALE;
        auto y = (newData.at(serie.title) - mYMin) * Y_SCALE;
        y = mSize.y - y; // Reverse SFML axis
        auto newVertex = sf::Vertex({ static_cast<float>(x), static_cast<float>(y) }, serie.color);

        // Check if we should swap the two buffers and clear the (new) present buffer
        if (newEpoch < mLastEpoch) {
            std::swap(serie.presentVertices, serie.pastVertices);
            serie.presentVertices.clear();
        }

        // Remove vertices from the previous buffer that overlap with the current buffer
        auto shouldBeRemove = [&](sf::Vertex const& v) { return v.position.x <= newEpoch.asSeconds() * X_SCALE; };
        serie.pastVertices.erase(std::remove_if(serie.pastVertices.begin(), serie.pastVertices.end(), shouldBeRemove),
                                 serie.pastVertices.end());

        // Add the new point in the current buffer
        serie.presentVertices.push_back(newVertex);

        // Update last value!
        serie.lastValue = newData.at(serie.title);
    }

    mLastEpoch = newEpoch;
}

void Graph::reset()
{
    mLastEpoch = sf::Time::Zero;

    for (auto& serie : mSeries) {
        serie.pastVertices.clear();
        serie.presentVertices.clear();
    }
}

void Graph::drawOn(sf::RenderTarget& target) const
{
    auto const LEGEND_MARGIN = 10;
    auto const FONT_SIZE = 10;
    auto lastLegendX = LEGEND_MARGIN;
    // Draw for each serie:
    for (auto const& serie : mSeries) {
        // The curve
        target.draw(&serie.pastVertices[0], serie.pastVertices.size(), sf::PrimitiveType::LinesStrip);
        target.draw(&serie.presentVertices[0], serie.presentVertices.size(), sf::PrimitiveType::LinesStrip);

        // The legend
        auto value = std::to_string(serie.lastValue);
        value = value.substr(0, value.find_last_not_of("0"));
        auto text = serie.title + ": " + value;
        auto legend = sf::Text(text, getAppFont(), FONT_SIZE);
        legend.setPosition(lastLegendX, LEGEND_MARGIN);
        legend.setColor(serie.color);
        target.draw(legend);

        lastLegendX += LEGEND_MARGIN + legend.getLocalBounds().width;
    }
}
