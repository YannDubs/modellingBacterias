#ifndef NUTRIMENT_HPP
#define NUTRIMENT_HPP

#include "CircularBody.hpp"

#include "Utility/Utility.hpp"
#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"

class Bacterium; 
class SimpleBacterium; 
class TwitchingBacterium; 
class SwarmBacterium; 
class PlasmidBacterium; 

class Nutriment : public CircularBody, public Drawable, public Updatable
{	
public:
  //CONSTRUCTEUR
  Nutriment (Quantity const& quantite, Vec2d const& position);
    
  //DESTRUCTEURS
  virtual ~Nutriment ();
  
  //METHODES
  virtual Quantity takeQuantity (Quantity quantite);
  virtual void setQuantity (Quantity const& quantite);
  //dessine nutriment et taille sur la frame
  virtual void drawOn (sf::RenderTarget& target) const override ;
  //on pourrait aussi la mettre en virtuel pure mais on préfaire garder en
  //pas virtual pure afin d'écrire moins dans les sous nutriments car getConfig()
  //des sous nutriments appelera celle la
  virtual void updateRadius();
  virtual void update (sf::Time dt) override;
  bool isEmpty() const;
  double positionScore (const Vec2d& position) const;
  unsigned int getQuantity() const;
  
  //METHODES VIRTUELLES PURES 
  virtual Quantity eatableBy (Bacterium& bacterie)  = 0;
  virtual Quantity eatableBy (SimpleBacterium& bactrie) = 0;
  virtual Quantity eatableBy (TwitchingBacterium& bacterie) = 0;
  virtual Quantity eatableBy (SwarmBacterium& bacterie) = 0;
  virtual Quantity eatableBy (PlasmidBacterium& bacterie) = 0;
  virtual j::Value const& getConfig() const = 0;
		 
protected:
  Quantity m_quantite;
}; 

class NutrimentA : public Nutriment
{	
public: 
  NutrimentA (Quantity const& quantite, Vec2d const& position);
  virtual j::Value const& getConfig() const override;
  
  //METHODE VIRTUELLES PURES OVERRIDE
  virtual Quantity eatableBy (Bacterium& bacterium) override;
  virtual Quantity eatableBy (SimpleBacterium& bacterium) override;
  virtual Quantity eatableBy (TwitchingBacterium& bacterium) override;
  virtual Quantity eatableBy (SwarmBacterium& bacterium) override; 
  virtual Quantity eatableBy (PlasmidBacterium& bacterium) override;
  
private:
	
}; 

class NutrimentB : public Nutriment
{	
public: 
  NutrimentB (Quantity const& quantite, Vec2d const& position);
  virtual j::Value const& getConfig() const override;
  
  //EXTENSION
  virtual void update (sf::Time dt) override;
  bool isPerimer() const;
  virtual void drawOn (sf::RenderTarget& target) const override;
  virtual Quantity takeQuantity (Quantity quantite) override;
  //METHODE VIRTUELLES PURES OVERRIDE
  virtual Quantity eatableBy (Bacterium& bacterium) override;
  virtual Quantity eatableBy (SimpleBacterium& bacterium) override;
  virtual Quantity eatableBy (TwitchingBacterium& bacterium) override;
  virtual Quantity eatableBy (SwarmBacterium& bacterium) override; 
  virtual Quantity eatableBy (PlasmidBacterium& bacterium) override;

private:
  sf::Time m_tempsPeremption;
  bool m_isPerimer;
}; 

#endif
