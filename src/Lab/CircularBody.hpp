#ifndef CircularBody_HPP
#define CircularBody_HPP

#include "Utility/Vec2d.hpp"

class CircularBody
{
public:
  //CONSTRUCTEUR
  CircularBody (Vec2d const& m_position, double const& m_radius);
  CircularBody () = default; 
  //CONSTRUCTEUR DE COPIE
  CircularBody (CircularBody const& autre);
  //OPERATEUR INTERNE
  CircularBody& operator=(CircularBody const& source);
    
  //DESTRUCTEURS
  virtual ~CircularBody();
  
  //METHODES
  virtual void move (Vec2d const& vecteur);
  bool isInside (CircularBody const& other) const;
  bool isColliding (CircularBody const& other) const;
  bool isPointInside (Vec2d const& point) const;
  std::ostream& affiche (std::ostream& sortie) const;
  //GETTERS
  Vec2d getPosition() const;
  double getRadius() const;
  //SETTERS
  void setPosition (Vec2d const& position);
  void setRadius (double const& rayon);
		
protected:
  //protected:nécessaire ici car les sous classes héritent de ces attributs
  Vec2d m_position;
  double m_radius;
};

//OPERATEURS EXTERNES
  std::ostream& operator<<(std::ostream& sortie, 
						  CircularBody const& body);
  bool operator>(CircularBody const& body, CircularBody const& autre); 
  bool operator|(CircularBody const& body, CircularBody const& autre);
  bool operator>(CircularBody const& body, Vec2d const& point);  
	
#endif
