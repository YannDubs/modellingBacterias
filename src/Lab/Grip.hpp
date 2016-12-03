#ifndef GRIP_HPP
#define GRIP_HPP

#include "CircularBody.hpp"
#include "Utility/MutableColor.hpp"

class Grip : public CircularBody
{
public:
  //CONSTRUCTEUR
  Grip (Vec2d const& position, double const& radius); 
  
  //METHODES
  void drawOn (sf::RenderTarget& target, MutableColor const& color) const; 
  
private:
  
}; 

#endif

