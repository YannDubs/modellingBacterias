#define NDEBUG

#include <cassert>
#include "CircularBody.hpp"

//CONSTRUCTEUR
CircularBody::CircularBody (Vec2d const& position, double const& radius)
  : m_position (position)
  , m_radius (radius)     //pas de destructeur, l'implicite suffisant
{
  //envoie assertion/test mais ne gère pas l'erreur
  assert(radius > 0);
}
//CONSTRUCUTEUR DE COPIE
CircularBody::CircularBody (CircularBody const& autre)
  : m_position (autre.m_position)
  , m_radius (autre.m_radius)
{}

//DESTRUCTEUR
CircularBody::~CircularBody()
{}

//OPERATEUR INTERNE
CircularBody& CircularBody::operator=(CircularBody const& source) 
{
  m_radius = source.m_radius;
  m_position = source.m_position;
  return *(this); //notez qu'on aurait pu utiliser un swap
}
//OPERATEURS EXTERNES
std::ostream& operator<<(std::ostream& sortie, CircularBody const& body)
{
  body.affiche (sortie);
  return sortie;
}

bool operator>(CircularBody const& body, CircularBody const& autre)
{
  if (body.isInside (autre))
  {
    return true;
  }
  return false;
}
	
bool operator|(CircularBody const& body,CircularBody const& autre) 
{
  if (body.isColliding (autre))
  {
    return true;
  }
  return false;
}
	
bool operator>(CircularBody const& body, Vec2d const& point) 
{
  if (body.isPointInside (point))
  {
    return true;
  }
  return false;
}
//METHODES
void CircularBody::move (Vec2d const& vecteur)
{
  m_position += vecteur;
}

bool CircularBody::isInside (CircularBody const& other) const
{	
  if ((other.m_radius <= m_radius)
      && (distance (other.m_position, m_position)
      <= (m_radius - other.m_radius)))
  {
    return true;
  }
  return false;
}
	
bool CircularBody::isColliding (CircularBody const& other) const
{	
  if (distance (other.m_position, m_position)
      <= (m_radius + other.m_radius))
  {
    return true;
  }
  return false;
}

bool CircularBody::isPointInside (Vec2d const& point) const
{	
  if (distance (m_position, point) <= m_radius)
  {
    return true;
  }
  return false;
}

std::ostream& CircularBody::affiche (std::ostream& sortie) const
{
  sortie << "CircularBody: position = " << m_position << " radius = "
  << m_radius;
  return sortie;
}	
//GETTERS
Vec2d CircularBody::getPosition() const
{
  return m_position;
}
	
double CircularBody::getRadius() const
{
  return m_radius;
}
//SETTERS
void CircularBody::setPosition (Vec2d const& position)
{
  m_position = position;
}

void CircularBody::setRadius (double const& rayon)
{
  m_radius = rayon;
}
