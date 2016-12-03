/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Application.hpp>
#include <Lab/Bacterium.hpp>


class DummyBacterium : public Bacterium
{
public:
	DummyBacterium(Vec2d const& position)
		: Bacterium(60, position, Vec2d::fromRandomAngle(), 10,
					getAppConfig()["simple bacterium"]["color"])
		{
		}
	
	j::Value& getConfig() const {
		return getAppConfig()["simple bacterium"];
	}

	
protected:
	virtual Bacterium* clone() const
		{
			/* ne fait rien pour le moment
			(cette méthode devra être modifiée lorsque vous aurez codé
			la division)
			*/
			return nullptr;
		}
	
	virtual void move(sf::Time dt)
		{
			consumeEnergy(0.1);
		}
};

	
		  
class BacteriaTest : public Application
{
public:
    BacteriaTest(int argc, char const** argv)
    : Application(argc, argv)
    {
    }

    virtual ~BacteriaTest() = default;

    virtual void onRun() override final;

    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
};

IMPLEMENT_MAIN(BacteriaTest);

void BacteriaTest::onRun()
{
    // Basic setup
    Application::onRun();

    // Add sliders for simple bacteria
    packIntoLeftMenuNewSlider("Test bacterium", "energy consumption", { "simple bacterium", "energy", "consumption factor" },
                              0.01, 2.5, 0.01);
    packIntoLeftMenuNewSlider("Test bacterium", "meal delay", { "simple bacterium", "meal", "delay" }, 0.1, 2, 0.1);
}

void BacteriaTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::S:
            getEnv().addBacterium(new DummyBacterium(getCursorPositionInView()));
            break;
        }
    }
}
