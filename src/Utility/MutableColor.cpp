#include "MutableColor.hpp"

//CONSTRUCTEUR
MutableColor::MutableColor (j::Value const& config)
//constructeur par défault par défaut qui se lance implicitement 
{ 	
  //on iterre sur les cases du tableaux donc ça doit être dans les {}
  color[0]=config["r"];				
  color[1]=config["g"];
  color[2]=config["b"];
  color[3]=config["a"];
}

//méthodes
void MutableColor::mutate()
{
  for (MutableNumber& couleur : color)
  {
	couleur.mutate();
  }
}

sf::Color MutableColor::getColor() const
{
  //getValeur de MutableColor rend une SFML
  return 
  {
	static_cast<sf::Uint8>(color[0].get() * 255),  
	static_cast<sf::Uint8>(color[1].get() * 255),
	static_cast<sf::Uint8>(color[2].get() * 255), 
	static_cast<sf::Uint8>(color[3].get() * 255) 
  };
}


	
	
	
	
	
	
	
	

