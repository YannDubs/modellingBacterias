#include "NutrimentGenerator.hpp"

#include "Random/Random.hpp"
#include "Application.hpp"

//CONSTRUCTEUR
NutrimentGenerator::NutrimentGenerator()
  : m_compteur (sf::Time::Zero)
{}

//METHODES
void NutrimentGenerator::update (sf::Time dt)
{
  //On incrémente à chaque appel
  m_compteur += dt;
  double nutrimentDelay (getAppConfig()["generator"]["nutriment"]["delay"].toDouble());
  
  //vu qu'on fait de l'allocation dynamique il faut donc supprimer la 
  //zone mémoire si on a pas pu adder le nutriment
  bool isAdded (false);
  Nutriment* NutrimentAdded;

  if (m_compteur > sf::seconds (nutrimentDelay))
    {
    m_compteur = sf::Time::Zero;
    double probabiliteGenerator (getAppConfig()["generator"]["nutriment"]["prob"].toDouble());
    double quantiteAMin (getAppConfig()["nutriments"]["A"]["quantity"]["min"].toDouble());
    double quantiteAMax (getAppConfig()["nutriments"]["A"]["quantity"]["max"].toDouble());
    double quantiteBMin (getAppConfig()["nutriments"]["B"]["quantity"]["min"].toDouble());
    double quantiteBMax (getAppConfig()["nutriments"]["B"]["quantity"]["max"].toDouble());
    double worldSizeX (getApp().getWorldSize().x);
    double positionX (normal ((worldSizeX) / 2, (worldSizeX) * (worldSizeX) / 16));
    double worldSizeY (getApp().getWorldSize().y);
    double positionY (normal ((worldSizeY) / 2,(worldSizeY) * (worldSizeY) / 16));
      
    //fonction qui donne le placement selon loi normale
    if (bernoulli(probabiliteGenerator))
    {
	  NutrimentAdded = new NutrimentA (uniform (quantiteAMin, quantiteAMax)
								      , Vec2d (positionX, positionY));
      isAdded = getAppEnv().addNutriment (NutrimentAdded);
    }
    else
    {
	  NutrimentAdded = new NutrimentB (uniform (quantiteAMin, quantiteAMax)
							          , Vec2d (positionX, positionY));
      isAdded = getAppEnv().addNutriment (NutrimentAdded);
    }
    
    if (!isAdded)
    {
	  delete NutrimentAdded;
	  NutrimentAdded = nullptr;
	}

  }
}

void NutrimentGenerator::reset()
{
  m_compteur = sf::Time::Zero;
}
