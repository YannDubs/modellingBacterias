#ifndef NutrimentGenerator_HPP
#define NutrimentGenerator_HPP

#include <SFML/Graphics.hpp>

class NutrimentGenerator 
{	
public :
  //CONSTRUCTEUR
  NutrimentGenerator();
	
  //METHODES
  void update (sf::Time dt);
  void reset();
		
private :
  sf::Time m_compteur;
		
}; 

#endif
