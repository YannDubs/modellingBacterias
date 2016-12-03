/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Application.hpp>
#include <Lab/TwitchingBacterium.hpp>

class DummyTwitching : public TwitchingBacterium
{
public:
	using TwitchingBacterium::TwitchingBacterium;
	using TwitchingBacterium::moveGrip;
	using TwitchingBacterium::getRadius;
};

	
class GripTest : public Application
{
public:
    GripTest(int argc, char const** argv)
    : Application(argc, argv)
    {
    }

    virtual ~GripTest() = default;

    virtual void onRun() override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

IMPLEMENT_MAIN(GripTest);

void GripTest::onRun()
{
    // Basic setup
    Application::onRun();

    // Twitching bacteria
    // energy
    packIntoLeftMenuNewSlider("Test", "energy consumption, move",
                              { "twitching bacterium", "energy", "consumption factor", "move" }, 0.01, 2.5, 0.01);
    packIntoLeftMenuNewSlider("Test", "energy division", { "twitching bacterium", "energy", "division" }, 5, 200, 5);
    packIntoLeftMenuNewSlider("Test", "energy consumption, tentacle",
                              { "twitching bacterium", "energy", "consumption factor", "tentacle" }, 0.01, 2.5, 0.01);
    // meal
    packIntoLeftMenuNewSlider("Test", "meal delay", { "twitching bacterium", "meal", "delay" }, 0.1, 2, 0.1);
    packIntoLeftMenuNewSlider("Test", "meal quantity", { "twitching bacterium", "meal", "max" }, 0, 100, 1);
    // size
    packIntoLeftMenuNewSlider("Test", "initial max size", { "twitching bacterium", "radius", "max" }, 1, 100, 1);
    packIntoLeftMenuNewSlider("Test", "initial min size", { "twitching bacterium", "radius", "min" }, 1, 100, 1);
    // tentacle length
    packIntoLeftMenuNewSlider("Test", "initial tentacle length",
                              { "twitching bacterium", "tentacle", "length", "initial" }, 0, 500, 5);
    packIntoLeftMenuNewSlider("Test", "tentacle length mutation rate",
                              { "twitching bacterium", "tentacle", "length", "rate" }, 0, 1, 0.01);
    packIntoLeftMenuNewSlider("Test", "tentacle length mutation standard deviation",
                              { "twitching bacterium", "tentacle", "length", "sigma" }, 0, 50, 0.1);
    // tentacle speed
    packIntoLeftMenuNewSlider("Test", "initial tentacle speed",
                              { "twitching bacterium", "tentacle", "speed", "initial" }, 0, 100, 1);
    packIntoLeftMenuNewSlider("Test", "tentacle speed mutation rate",
                              { "twitching bacterium", "tentacle", "speed", "rate" }, 0, 1, 0.01);
    packIntoLeftMenuNewSlider("Test", "tentacle speed mutation standard deviation",
                              { "twitching bacterium", "tentacle", "speed", "sigma" }, 0, 50, 0.1);
    // seed factor
    packIntoLeftMenuNewSlider("Test", "move speed factor", { "twitching bacterium", "speed factor" }, 0, 1, 0.01);
}

void GripTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::T:
			DummyTwitching* twitching(new DummyTwitching(getCursorPositionInView()));
			twitching->moveGrip(Vec2d(5,5)*twitching->getRadius());
            getEnv().addBacterium(twitching);
            break;
        }
    }
}
