/*
 * prjsv 2015
 * 2013, 2014
 * Marco Antognini
 */

#include <Config.hpp>
#include <Lab/SimpleBacterium.hpp>
#include <Lab/Swarm.hpp>
#include <Lab/SwarmBacterium.hpp>
#include <Lab/TwitchingBacterium.hpp>
#include <Lab/PlasmidBacterium.hpp>
#include <FinalApplication.hpp>

#include <cassert>

IMPLEMENT_MAIN(FinalApplication);

void FinalApplication::onRun()
{
    // Basic setup
    Application::onRun();


    // Simple bacteria
    // energy
    packIntoLeftMenuNewSlider("Simple Bacterium", "energy consumption",
                              { "simple bacterium", "energy", "consumption factor" }, 0.01, 2.5, 0.01);
    packIntoLeftMenuNewSlider("Simple Bacterium", "energy division", { "simple bacterium", "energy", "division" }, 5,
                              200, 5);
    // meal
    packIntoLeftMenuNewSlider("Simple Bacterium", "meal delay", { "simple bacterium", "meal", "delay" }, 0.1, 2, 0.1);
    packIntoLeftMenuNewSlider("Simple Bacterium", "meal quantity", { "simple bacterium", "meal", "max" }, 0, 100, 1);
    // size
    packIntoLeftMenuNewSlider("Simple Bacterium", "initial max size", { "simple bacterium", "radius", "max" }, 1, 100,
                              1);
    packIntoLeftMenuNewSlider("Simple Bacterium", "initial min size", { "simple bacterium", "radius", "min" }, 1, 100,
                              1);
    // speed
    packIntoLeftMenuNewSlider("Simple Bacterium", "initial speed", { "simple bacterium", "speed", "initial" }, 0, 100,
                              1);
    packIntoLeftMenuNewSlider("Simple Bacterium", "speed mutation rate", { "simple bacterium", "speed", "rate" }, 0, 1,
                              0.01);
    packIntoLeftMenuNewSlider("Simple Bacterium", "speed mutation standard deviation",
                              { "simple bacterium", "speed", "sigma" }, 0, 50, 0.1);
    // tumble better
    packIntoLeftMenuNewSlider("Simple Bacterium", "initial tumble (better)",
                              { "simple bacterium", "tumble", "better", "initial" }, 0, 100, 1);
    packIntoLeftMenuNewSlider("Simple Bacterium", "tumble (better) mutation rate",
                              { "simple bacterium", "tumble", "better", "rate" }, 0, 1, 0.01);
    packIntoLeftMenuNewSlider("Simple Bacterium", "tumble (better) mutation standard deviation",
                              { "simple bacterium", "tumble", "better", "sigma" }, 0, 50, 0.1);
    // tumble worse
    packIntoLeftMenuNewSlider("Simple Bacterium", "initial tumble (worse)",
                              { "simple bacterium", "tumble", "worse", "initial" }, 0, 100, 1);
    packIntoLeftMenuNewSlider("Simple Bacterium", "tumble (worse) mutation rate",
                              { "simple bacterium", "tumble", "worse", "rate" }, 0, 1, 0.01);
    packIntoLeftMenuNewSlider("Simple Bacterium", "tumble (worse) mutation standard deviation",
                              { "simple bacterium", "tumble", "worse", "sigma" }, 0, 50, 0.1);


    // Twitching bacteria
    // energy
    packIntoLeftMenuNewSlider("Twitching Bacterium", "energy consumption, move",
                              { "twitching bacterium", "energy", "consumption factor", "move" }, 0.01, 2.5, 0.01);
    packIntoLeftMenuNewSlider("Twitching Bacterium", "energy division", { "twitching bacterium", "energy", "division" },
                              5, 200, 5);
    packIntoLeftMenuNewSlider("Twitching Bacterium", "energy consumption, tentacle",
                              { "twitching bacterium", "energy", "consumption factor", "tentacle" }, 0.01, 2.5, 0.01);
    // meal
    packIntoLeftMenuNewSlider("Twitching Bacterium", "meal delay", { "twitching bacterium", "meal", "delay" }, 0.1, 2,
                              0.1);
    packIntoLeftMenuNewSlider("Twitching Bacterium", "meal quantity", { "twitching bacterium", "meal", "max" }, 0, 100,
                              1);
    // size
    packIntoLeftMenuNewSlider("Twitching Bacterium", "initial max size", { "twitching bacterium", "radius", "max" }, 1,
                              100, 1);
    packIntoLeftMenuNewSlider("Twitching Bacterium", "initial min size", { "twitching bacterium", "radius", "min" }, 1,
                              100, 1);
    // tentacle length
    packIntoLeftMenuNewSlider("Twitching Bacterium", "initial tentacle length",
                              { "twitching bacterium", "tentacle", "length", "initial" }, 0, 500, 5);
    packIntoLeftMenuNewSlider("Twitching Bacterium", "tentacle length mutation rate",
                              { "twitching bacterium", "tentacle", "length", "rate" }, 0, 1, 0.01);
    packIntoLeftMenuNewSlider("Twitching Bacterium", "tentacle length mutation standard deviation",
                              { "twitching bacterium", "tentacle", "length", "sigma" }, 0, 50, 0.1);
    // tentacle speed
    packIntoLeftMenuNewSlider("Twitching Bacterium", "initial tentacle speed",
                              { "twitching bacterium", "tentacle", "speed", "initial" }, 0, 100, 1);
    packIntoLeftMenuNewSlider("Twitching Bacterium", "tentacle speed mutation rate",
                              { "twitching bacterium", "tentacle", "speed", "rate" }, 0, 1, 0.01);
    packIntoLeftMenuNewSlider("Twitching Bacterium", "tentacle speed mutation standard deviation",
                              { "twitching bacterium", "tentacle", "speed", "sigma" }, 0, 50, 0.1);
    // seed factor
    packIntoLeftMenuNewSlider("Twitching Bacterium", "move speed factor", { "twitching bacterium", "speed factor" }, 0,
                              1, 0.01);


    // Swarm bacteria: generate two swarms, with their default settings if needed, and add sliders
    //Swarm::generateDefaultSettingForId("1", 25, sf::Color::Green);
    //Swarm::generateDefaultSettingForId("2", 50, sf::Color::Cyan);
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
    // force
    packIntoLeftMenuNewSlider("Swarm Bacterium", "swarm 1 force factor", { "swarms", "1", "force factor" }, 0, 100, 5);
    packIntoLeftMenuNewSlider("Swarm Bacterium", "swarm 2 force factor", { "swarms", "2", "force factor" }, 0, 100, 5);
    
    //CHANGER
    // Plasmid bacteria
        // energy
    packIntoLeftMenuNewSlider("plasmid bacterium", "energy consumption",
                              { "plasmid bacterium", "energy", "consumption factor" }, 0.01, 2.5, 0.01);
    packIntoLeftMenuNewSlider("plasmid bacterium", "energy division", { "simple bacterium", "energy", "division" }, 5,
                              200, 5);
    // meal
    packIntoLeftMenuNewSlider("plasmid bacterium", "meal delay", { "simple bacterium", "meal", "delay" }, 0.1, 2, 0.1);
    packIntoLeftMenuNewSlider("plasmid bacterium", "meal quantity", { "simple bacterium", "meal", "max" }, 0, 100, 1);
    // speed
    packIntoLeftMenuNewSlider("plasmid bacterium", "initial speed", { "simple bacterium", "speed", "initial" }, 0, 100,
                              1);
    packIntoLeftMenuNewSlider("plasmid bacterium", "speed mutation rate", { "simple bacterium", "speed", "rate" }, 0, 1,
                              0.01);
    packIntoLeftMenuNewSlider("plasmid bacterium", "speed mutation standard deviation",
                              { "plasmid bacterium", "speed", "sigma" }, 0, 50, 0.1);
    // tumble better
    packIntoLeftMenuNewSlider("plasmid bacterium", "initial tumble (better)",
                              { "plasmid bacterium", "tumble", "better", "initial" }, 0, 100, 1);
    packIntoLeftMenuNewSlider("plasmid bacterium", "tumble (better) mutation rate",
                              { "plasmid bacterium", "tumble", "better", "rate" }, 0, 1, 0.01);
    packIntoLeftMenuNewSlider("plasmid bacterium", "tumble (better) mutation standard deviation",
                              { "plasmid bacterium", "tumble", "better", "sigma" }, 0, 50, 0.1);
    // tumble worse
    packIntoLeftMenuNewSlider("plasmid bacterium", "initial tumble (worse)",
                              { "plasmid bacterium", "tumble", "worse", "initial" }, 0, 100, 1);
    packIntoLeftMenuNewSlider("plasmid bacterium", "tumble (worse) mutation rate",
                              { "plasmid bacterium", "tumble", "worse", "rate" }, 0, 1, 0.01);
    packIntoLeftMenuNewSlider("plasmid bacterium", "tumble (worse) mutation standard deviation",
                              { "plasmid bacterium", "tumble", "worse", "sigma" }, 0, 50, 0.1);
                              
    // resistance
    packIntoLeftMenuNewSlider("plasmid bacterium", "initial epaisseur",
                              { "plasmid bacterium", "resistance", "epaisseur", "initial" }, 0, 5, 1);
    packIntoLeftMenuNewSlider("plasmid bacterium", "epaisseur mutation rate",
                              { "plasmid bacterium", "resistance", "epaisseur", "rate" }, 0, 1, 0.1);
    packIntoLeftMenuNewSlider("plasmid bacterium", "epaisseur mutation standard deviation",
                              { "plasmid bacterium", "resistance", "epaisseur", "sigma" }, 0, 3, 1);
    packIntoLeftMenuNewSlider("plasmid bacterium", "probabilite resistance",
                              { "plasmid bacterium", "resistance", "probabilite resistance" }, 0, 1, 0.05);
    packIntoLeftMenuNewSlider("plasmid bacterium", "probabilite echange plasmid",
                              { "plasmid bacterium", "resistance", "probabilite echange" }, 0, 1, 0.3);
}

void FinalApplication::onSimulationStart()
{
    getEnv().addSwarm(new Swarm("1"));
    getEnv().addSwarm(new Swarm("2"));
}

void FinalApplication::onEvent(sf::Event event, sf::RenderWindow&)
{
  //vu qu'on fait de l'allocation dynamique il faut donc supprimer la 
  //zone mémoire si on a pas pu adder le nutriment
  bool isAdded (true);
  Bacterium* BacteriumAdded;
  
	//CHANGER: on bloque chaque population de bactérie à 200 individus
	//pour toujours voir les graphes qui sont borné par 0 et 200
    if (event.type == sf::Event::KeyReleased) {
		//CHANGER:
		double compteurSwarm (SwarmBacterium::getSwarmCompteur());
		
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::S:
		{
			//CHANGER:
			double compteurSimple (SimpleBacterium::getSimpleCompteur());
			if (compteurSimple < 200)
            {
				BacteriumAdded = new SimpleBacterium(getCursorPositionInView());
				isAdded = getEnv().addBacterium(BacteriumAdded);
			}
            break;
        }

        case sf::Keyboard::T:
        {
            //CHANGER:
            double compteurTwitching (TwitchingBacterium::getTwitchingCompteur());
            if (compteurTwitching < 200)
            {
				BacteriumAdded = new TwitchingBacterium(getCursorPositionInView());
				isAdded = getEnv().addBacterium(BacteriumAdded);
			}
            break;
        }
        
        case sf::Keyboard::P:
        {
            //CHANGER:
            double compteurPlasmid (PlasmidBacterium::getPlasmidCompteur());
            if (compteurPlasmid < 200)
            {
				BacteriumAdded = new PlasmidBacterium(getCursorPositionInView());
				isAdded = getEnv().addBacterium(BacteriumAdded);
			}
            break;
        }
        
        case sf::Keyboard::Num1: 
        {
            //CHANGER:
            if (compteurSwarm < 200)
            {
				auto swarm = getEnv().getSwarmWithId("1");
				assert(swarm != nullptr);
				BacteriumAdded = new SwarmBacterium (getCursorPositionInView(), swarm);
				isAdded = getEnv().addBacterium(BacteriumAdded);
			}
        } break;

        case sf::Keyboard::Num2: 
        {	
            if (compteurSwarm < 200)
            {
				auto swarm = getEnv().getSwarmWithId("2");
				assert(swarm != nullptr);
				BacteriumAdded = new SwarmBacterium(getCursorPositionInView(), swarm);
				isAdded = getEnv().addBacterium(BacteriumAdded);
			}
        } 
        break;
        }
        
        if (!isAdded)
        {
		  delete BacteriumAdded;
		  BacteriumAdded = nullptr;
		}
    }
}
