#ifndef BACTERIUM_HPP
#define BACTERIUM_HPP

#include <string>

#include "CircularBody.hpp"
#include "Nutriment.hpp"

//on part toujours de partie3
#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"
#include "Utility/MutableColor.hpp"

//il faut prédeclarer chaque nutriment pour casser la dependance circulaire
class Nutriment;
class NutrimentA;
class NutrimentB;

typedef double Radian;

//map est déjà un tableau
typedef std::map<std::string, MutableNumber> Parametres;

class Bacterium : public CircularBody, public Drawable, public Updatable
{
public:		
  //CONSTRUCTEUR
  Bacterium (Quantity const& energy, Vec2d const& position,
             Vec2d const& direction, double const& rayon,
             MutableColor const& couleur,
             Parametres const& parametres=Parametres(),
             bool const& abstinance=false);

  //METHODES
  void mutate();
  bool dead() const;
  //pas en const car modifie le compteur
  bool canEat (sf::Time dt);
  void consumeEnergy (Quantity const& qt);
  void addProperty (const std::string& key, MutableNumber parametre);
  virtual Bacterium* clone();
  void eat (Nutriment& nutriment);

  //SETTEURS
  void setEnergy (Quantity const& quantite);
  //pour eviter les duplications de code vu que toutes les bactéries
  //mettent a jour leur direction en fonction du gradient de nutriment
  void setBestDirection();
  
  //GETTEURS
  Quantity getEnergyMin() const;
  sf::Time getTemps() const;
  MutableNumber getProperty (const std::string& key) const;
  Quantity getMaxEatable() const; 
  
  //METHODES OVERRIDE
  //dessine nutriment et taille sur la frame
  virtual void drawOn (sf::RenderTarget& target) const override;
  virtual void update (sf::Time dt) override;
  
  //METHODES VIRTUAL PURES
  virtual void move (sf::Time dt) = 0;
  virtual j::Value& getConfig() const = 0;
  virtual Bacterium* copie() const = 0;
  virtual Quantity getConsoEnergy() const = 0; 
  virtual Quantity eatableQuantity (NutrimentA& nutriment) = 0;
  virtual Quantity eatableQuantity (NutrimentB& nutriment) = 0;

  //DESTRUCTEURS
  virtual ~Bacterium();
	  
protected:
  Quantity m_energy;
  Vec2d m_direction;
  MutableColor m_color;
  Parametres m_parametresMutable;
  bool m_abstinance;
  Radian m_angleRotation;
  sf::Time m_compteur;
}; 

#endif

