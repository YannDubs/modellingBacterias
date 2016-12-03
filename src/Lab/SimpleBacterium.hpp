#ifndef SIMPLEBACTERIUM_HPP
#define SIMPLEBACTERIUM_HPP

#include "Bacterium.hpp"

#include "Utility/Vec2d.hpp"
#include "Utility/DiffEqSolver.hpp"

//il faut prédeclarer chaque nutriment pour casser la dependance circulaire
class Nutriment;
class NutrimentA;
class NutrimentB;

class SimpleBacterium : public Bacterium, public DiffEqFunction
{
public:
  //CONSTRUCTEUR
  SimpleBacterium (Vec2d const& position);
  
  //CONSTRUCTEUR DE COPIE 
  //on fait une surcharge pour faciliter la maintenance pour ne
  //pas avoir à changer le constructeur de copie
  SimpleBacterium (SimpleBacterium const& autre, int const& valeur); 
  //on réinitialise le constructeur par défaut pour pouvoir le 
  //rappeler dans la copie (nous sert justepour incrémenter le compteur
  SimpleBacterium (SimpleBacterium const& autre) = default; 
  
  //METHODES
  void updateAngle (sf::Time dt);
  void updateProbabiliteBasculement (Vec2d const& anciennePosition);
  void basculement();
  
  //GETTEURS
  Vec2d getSpeedVector (int const& valeur) const;
    
  //METHODES OVERRIDE
  virtual void drawOn (sf::RenderTarget& target) const override;

  
  //VIRTUAL PUR OVERRIDE
  //la fonction prend 2 arguments meme si c'est des arguments de classe
  //car elle doit overrider la fonction de  DiffEqFunction
  virtual Vec2d f (Vec2d position, Vec2d speed) const override;
  virtual Bacterium* copie() const override;
  virtual Quantity getConsoEnergy() const override; 
  virtual void move (sf::Time dt) override;
  j::Value& getConfig() const  override;
  
  //SURCHARGE DE METHODE VIRTUELLE PURES OVERRIDE
  virtual Quantity eatableQuantity (NutrimentA& nutriment) override;
  virtual Quantity eatableQuantity (NutrimentB& nutriment) override;
  
  //STATIC
  static unsigned int getSimpleCompteur();
  static void resetSimpleCompteur(); 
    
  //DESTRUCTEUR
  virtual ~SimpleBacterium();
  
private:
  double m_probabiliteBasculement;
  sf::Time m_tempsBasculement;
  double m_tFlagelle;
  static unsigned int simpleBacteriumCompteur;
}; 

#endif
