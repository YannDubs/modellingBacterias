#ifndef LAB_HPP
#define LAB_HPP

#include <string>

#include "PetriDish.hpp" 
#include "NutrimentGenerator.hpp" 
#include <unordered_map>

#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"
#include "Swarm.hpp" 

typedef std::unordered_map<std::string, double> Data;

class Lab : public Drawable, public Updatable
{	
public:
  //CONSTRUCTEUR
  Lab();
    
  //DESTRUCTEUR
  ~Lab();

  //METHODES
  void drawOn (sf::RenderTarget& targetWindow) const;
  void update (sf::Time dt);
  void reset();
  bool doesCollideWithDish (CircularBody const& body) const;
  Nutriment* getNutrimentColliding (CircularBody const& body) const;
  Swarm* getSwarmWithId (std::string const& identificateur) const;
  //bool isInsidePetri(CircularBody const& body) const;
  //pas virtuel car plus de polymorphisme car un seul swarm 
  bool addSwarm (Swarm*);
  virtual bool addNutriment (Nutriment* nutriment);
  virtual bool addBacterium (Bacterium* bacterie);
  double getPositionScore (const Vec2d& position) const;
  Data fetchData (std::string const& title) const;
  PlasmidBacterium* getPlasmidProche (PlasmidBacterium* bacterie) const;
  bool addPlasmidBacterium (PlasmidBacterium* bacterie);
  bool deletePlasmidBacterium (PlasmidBacterium* bacterie);
  
private:
  PetriDish m_boitePetri;
  NutrimentGenerator m_nutriCreate;	
}; 

#endif
