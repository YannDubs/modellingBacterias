/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Application.hpp>
#include <Lab/SimpleBacterium.hpp>

class SimpleBacteriaTest : public Application
{
public:
    SimpleBacteriaTest(int argc, char const** argv)
    : Application(argc, argv)
    {
    }

    virtual ~SimpleBacteriaTest() = default;

    virtual void onRun() override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

IMPLEMENT_MAIN(SimpleBacteriaTest);

void SimpleBacteriaTest::onRun()
{
    // Basic setup
    Application::onRun();

    // Add sliders for simple bacteria
    packIntoLeftMenuNewSlider("Test", "SB energy consumption", { "simple bacterium", "energy", "consumption factor" },
                              0.01, 2.5, 0.01);
    packIntoLeftMenuNewSlider("Test", "SB meal delay", { "simple bacterium", "meal", "delay" }, 0.1, 2, 0.1);
}

void SimpleBacteriaTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::S:
            getEnv().addBacterium(new SimpleBacterium(getCursorPositionInView()));
            break;
        }
    }
}
