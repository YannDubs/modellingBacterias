/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Application.hpp>
#include <Lab/Swarm.hpp>
#include <Lab/SwarmBacterium.hpp>

#include <array>
#include <cassert>

class SwarmBacteriaTest : public Application
{
public:
    SwarmBacteriaTest(int argc, char const** argv)
    : Application(argc, argv)
    {
    }

    virtual ~SwarmBacteriaTest() = default;

    virtual void onRun() override final;

    virtual void onSimulationStart() override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

IMPLEMENT_MAIN(SwarmBacteriaTest);

void SwarmBacteriaTest::onRun()
{
    // Basic setup
    Application::onRun();

    // Add sliders for swarm bacteria
    // energy
    packIntoLeftMenuNewSlider("Swarm Bacterium", "energy consumption",
                              { "swarm bacterium", "energy", "consumption factor" }, 0.01, 2.5, 0.01);
    packIntoLeftMenuNewSlider("Swarm Bacterium", "energy division", { "swarm bacterium", "energy", "division" }, 5, 200,
                              5);
    // meal
    packIntoLeftMenuNewSlider("Swarm Bacterium", "meal delay", { "swarm bacterium", "meal", "delay" }, 0.1, 2, 0.1);
    packIntoLeftMenuNewSlider("Swarm Bacterium", "meal quantity", { "swarm bacterium", "meal", "max" }, 0, 100, 1);
    // size
    packIntoLeftMenuNewSlider("Swarm Bacterium", "initial max size", { "swarm bacterium", "radius", "max" }, 1, 100, 1);
    packIntoLeftMenuNewSlider("Swarm Bacterium", "initial min size", { "swarm bacterium", "radius", "min" }, 1, 100, 1);
    // speed
    packIntoLeftMenuNewSlider("Swarm Bacterium", "initial speed", { "swarm bacterium", "speed", "initial" }, 0, 100, 1);
}

void SwarmBacteriaTest::onSimulationStart()
{
    for (auto i = 0; i < 10; ++i) {
        auto id = std::to_string(i);
        //Swarm::generateDefaultSettingForId(id, i * 5);
        getEnv().addSwarm(new Swarm(id));
    }
}

void SwarmBacteriaTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::Num0 <= event.key.code && event.key.code <= sf::Keyboard::Num9) {
            auto id = std::to_string(event.key.code - sf::Keyboard::Num0);
            auto swarm = getEnv().getSwarmWithId(id);
            assert(swarm != nullptr);
            getEnv().addBacterium(new SwarmBacterium(getCursorPositionInView(), swarm));
        }
    }
}
