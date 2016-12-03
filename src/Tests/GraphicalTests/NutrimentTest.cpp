/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Application.hpp>
#include <Lab/Nutriment.hpp>

class NutrimentTest : public Application
{
public:
    NutrimentTest(int argc, char const** argv)
    : Application(argc, argv)
    {
    }

    virtual ~NutrimentTest() = default;

    virtual void onRun() override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;

private:
    Nutriment* lastCreated = nullptr;
};

IMPLEMENT_MAIN(NutrimentTest);

void NutrimentTest::onRun()
{
    // Basic setup
    Application::onRun();
}

void NutrimentTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::N:
            if (event.key.shift) {
                lastCreated = new NutrimentA(50, getCursorPositionInView());
            } else {
                lastCreated = new NutrimentB(50, getCursorPositionInView());
            }
            getEnv().addNutriment(lastCreated);
            break;

        // to test takeQuantity
        case sf::Keyboard::C:
            if (lastCreated != nullptr) {
                lastCreated->takeQuantity(15);
            }
            break;
        }
    }
}
