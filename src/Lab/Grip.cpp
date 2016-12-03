#include "Grip.hpp"
#include "Utility/Utility.hpp"

//CONSCTRUCTEUR
Grip::Grip (Vec2d const& position, double const& radius)
  : CircularBody (position, radius) 
  {} 

//METHODES VIRTUELLE OVERRIDE
void Grip::drawOn (sf::RenderTarget& target, MutableColor const& color) const
{
  auto const circle = buildCircle (m_position, m_radius, color.getColor());
  target.draw (circle);
}
