#ifndef SWARMBACTERIUM_HPP
#define SWARMBACTERIUM_HPP

#include "Bacterium.hpp"
#include "Swarm.hpp"
#include "Nutriment.hpp"

#include "Utility/DiffEqSolver.hpp"
#include "Utility/Vec2d.hpp"

//prédéclaration car dépendance circulaire
class Swarm;

class Nutriment;
class NutrimentA;
class NutrimentB;

class SwarmBacterium : public Bacterium, public DiffEqFunction
{
public:
  //CONSTRUCTEUR
  SwarmBacterium (Vec2d const& position, Swarm* ptrSwarm);
  
  //CONSTRUCTEUR DE COPIE 
  // on fait une surcharge pour faciliter la maintenance pour ne
  // par avoir à changer le constructuer de copie
  SwarmBacterium (SwarmBacterium const& autre, int const& valeur); 
  //on réinitialise le constructeur par défaut pour pouvoir le 
  //rappeler dans la copie (nous sert justepour incrémenter le compteur
  SwarmBacterium (SwarmBacterium const& autre) = default; 
  
  //METHODES
  void updateProbabiliteBasculement (Vec2d const& anciennePosition);
  void basculement();
  void beLeader ();
  void noLeader ();
  
  //GETTEURS
  Vec2d getSpeedVector (int const& valeur) const;
  
  //METHODES OVERRIDE
  virtual void drawOn (sf::RenderTarget& target) const override;

  //VIRTUAL PUR OVERRIDE
  virtual void move (sf::Time dt) override;
  j::Value& getConfig() const override;
  //la fonction prend 2 arguments meme si c'est des arguments de classe
  //car elle doit overrider la fonction de  DiffEqFunction
  virtual Vec2d f(Vec2d position, Vec2d speed) const override;
  virtual Bacterium* copie() const override;
  virtual Quantity getConsoEnergy() const override; 
  
  //SURCHARGE DE METHODE VIRTUELLE PURES OVERRIDE
  virtual Quantity eatableQuantity (NutrimentA& nutriment) override;
  virtual Quantity eatableQuantity (NutrimentB& nutriment) override;
  
  //STATIC
  static unsigned int getSwarmCompteur();
  static void resetSwarmCompteur(); 
  
  //DESTRUCTEUR
  virtual ~SwarmBacterium();
  
private:
  bool m_isLeader;
  Swarm* m_ptrSwarm; 
  double m_probabiliteBasculement;
  sf::Time m_tempsBasculement;
  static unsigned int swarmBacteriumCompteur;
};

#endif
