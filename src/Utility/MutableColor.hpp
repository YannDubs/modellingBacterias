#ifndef MutableColor_HPP
#define MutableColor_HPP

#include <array>
#include <SFML/Graphics.hpp>

#include "Random/Random.hpp"
#include "MutableNumber.hpp"

typedef std::array <MutableNumber, 4> RGBA;

class MutableColor
{
public: 
//CONSTRUCTEUR
  MutableColor (j::Value const& config);
	
//METHODES
  void mutate();
  //fonctions const ne peuvent pas appeler des fonctions pas const dans dans les paramètres 
  sf::Color getColor() const; 

private: 

  RGBA color;  
	
}; 

#endif
