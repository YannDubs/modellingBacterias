#ifndef TWITCHINGBACTERIUM_HPP
#define TWITCHINGBACTERIUM_HPP

#include "Bacterium.hpp"
#include "Grip.hpp"

#include "Utility/Vec2d.hpp"

//il faut prédeclarer chaque nutriment pour casser la dependance circulaire
class Nutriment;
class NutrimentA;
class NutrimentB;

enum States 
{
       IDLE,
       WAIT_TO_DEPLOY,
       DEPLOY,
       ATTRACT,
       RETRACT,
       EAT
};

class TwitchingBacterium : public Bacterium
{
public:
  //CONSTRUCTEUR
  TwitchingBacterium (Vec2d const& position);
  
  //CONSTRUCTEUR DE COPIE
  // onfait une surcharge pour faciliter la maintenance pour ne
  // par avoir à changer le constructuer de copie
  TwitchingBacterium(TwitchingBacterium const& autre, int const& valeur);
  //on réinitialise le constructeur par défaut pour pouvoir le 
  //rappeler dans la copie (nous sert justepour incrémenter le compteur 
  TwitchingBacterium(TwitchingBacterium const& autre) = default; 
  
  //METHODES
  //notre fonction moveGrip est fondamentalement pensée différemment 
  //afin d'éviter des duplications de code nous utilisions donc 
  //pour passer les test une valeur par defaut de dt qui est 1 
  //vu que pour nous le move grip a pas besoin de dt mais que
  //vous l'appeler sans dt (il faudrais mieux theoriquement 
  //changer votre code )
  void moveGrip (Vec2d const& direction, sf::Time const& dt = sf::seconds(1)); 
  
  //GETTEURS
  Quantity getConsommationMove() const; 
  Quantity getConsommationTentacle() const; 
  
  //METHODES OVERRIDE
  virtual void drawOn (sf::RenderTarget& target) const override;

  //VIRTUAL PUR OVERRIDE
  virtual Bacterium* copie() const override;
  virtual Quantity getConsoEnergy() const override; 
  virtual void move (sf::Time dt) override;
  j::Value& getConfig() const override;
  
  //SURCHARGE DE METHODE VIRTUELLE PURES OVERRIDE
  virtual Quantity eatableQuantity (NutrimentA& nutriment) override;
  virtual Quantity eatableQuantity (NutrimentB& nutriment) override;
  
  //STATIC
  static unsigned int getTwitchingCompteur();
  static void resetTwitchingCompteur();
  
  //DESTRUCTEUR
  virtual ~TwitchingBacterium();
  
private:
  Grip m_grip; 
  States m_state; 
  static unsigned int twitchingBacteriumCompteur;
}; 

#endif
