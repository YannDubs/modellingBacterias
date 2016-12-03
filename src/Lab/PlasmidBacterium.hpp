#ifndef PLASMIDBACTERIUM_HPP
#define PLASMIDBACTERIUM_HPP

#include <array>

#include "Bacterium.hpp"
#include "PilusSexuel.hpp"

#include "Utility/DiffEqSolver.hpp"
#include "Utility/Vec2d.hpp"

//il faut prédeclarer chaque nutriment pour casser la dependance circulaire
class Nutriment;
class NutrimentA;
class NutrimentB;

enum Etat
{
       SEEK,
       CONJUGATE,
       LIVE
};

typedef std::array <bool, 2> Resistance;

class PlasmidBacterium : public Bacterium, public DiffEqFunction
{
public:
  //CONSTRUCTEUR
  PlasmidBacterium (Vec2d const& position);
  PlasmidBacterium () = default;
  
  //CONSTRUCTEUR DE COPIE 
  // onfait une surcharge pour faciliter la maintenance pour ne
  // par avoir à changer le constructuer de copie
  PlasmidBacterium (PlasmidBacterium const& autre, int const& valeur); 
  //on réinitialise le constructeur par défaut pour pouvoir le 
  //rappeler dans la copie (nous sert justepour incrémenter le compteur
  PlasmidBacterium (PlasmidBacterium const& autre) = default; 

  //METHODE
  void exchange (PlasmidBacterium* const& bacterie);
  void deplacement (sf::Time dt);
  void updateProbabiliteBasculement (Vec2d const& anciennePosition);
  //notez qu'on aurait presque du faire une classe basculable (comme
  //drawable et upadatble)
  void basculement();
  
  //GETTEURS
  Vec2d getSpeedVector (int const& valeur) const;
  Etat getState() const;
  Resistance getResistance() const;
  PlasmidBacterium* getLastBacterie () const;
  
  //METHODES OVERRIDE
  virtual void drawOn (sf::RenderTarget& target) const override;
  j::Value& getConfig() const  override;
  virtual void update (sf::Time dt) override;
  
  //VIRTUAL PUR OVERRIDE
  //la fonction prend 2 arguments meme si c'est des arguments de classe
  //car elle doit overrider la fonction de  DiffEqFunction
  virtual Bacterium* copie() const override;
  virtual Quantity getConsoEnergy() const override; 
  virtual Vec2d f(Vec2d position, Vec2d speed) const override;
  virtual void move (sf::Time dt) override;
   
  //SURCHARGE DE METHODE VIRTUELLE PURES OVERRIDE
  virtual Quantity eatableQuantity(NutrimentA& nutriment) override;
  virtual Quantity eatableQuantity(NutrimentB& nutriment) override;
  
  //STATIC
  static unsigned int getPlasmidCompteur();
  static void resetPlasmidCompteur(); 
    
  //DESTRUCTEUR
  virtual ~PlasmidBacterium();
  
private:
  Etat m_state; 
  PlasmidBacterium* m_LastBacterie;
  bool m_PlasmidR;
  bool m_PlasmidRE;
  double m_probabiliteBasculement;
  sf::Time m_tempsPrive;
  sf::Time m_tempsBasculement;
  PilusSexuel m_pilusSexuel; 
  static unsigned int plasmidBacteriumCompteur;
};

#endif
