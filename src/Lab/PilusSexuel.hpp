#ifndef PILUSSEXUEL_HPP
#define PILUSSEXUEL_HPP

#include "CircularBody.hpp"
#include "Utility/MutableColor.hpp"

class PilusSexuel : public CircularBody
{
public:
  //CONSTRUCTEUR
  PilusSexuel (Vec2d const& position, double const& radius); 
  PilusSexuel () = default; 
  //METHODES
  void drawOn (sf::RenderTarget& target, MutableColor const& color) const; 
  
private:
  
}; 

#endif

