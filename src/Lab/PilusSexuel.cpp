#include "PilusSexuel.hpp"
#include "Utility/Utility.hpp"

//CONSCTRUCTEUR
PilusSexuel::PilusSexuel (Vec2d const& position, double const& radius)
  : CircularBody (position, radius) 
  {} 

//METHODES VIRTUELLE OVERRIDE
void PilusSexuel
::drawOn (sf::RenderTarget& target, MutableColor const& color) const
{
  auto const circle = buildCircle (m_position, m_radius, color.getColor());
  target.draw (circle);
}
