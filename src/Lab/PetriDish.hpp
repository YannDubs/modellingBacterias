#ifndef PETRIDISH_HPP
#define PETRIDISH_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Nutriment.hpp"
#include "CircularBody.hpp" //inclue automatiquement Vec2d
#include "Bacterium.hpp"
#include "Swarm.hpp"

#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"


//ensemble ou population de bacteries:
typedef std::vector<Bacterium*> Bacteries;
//on aurait aussi pu écrire:
//using Bacteries = std::vector<Bacterium*>

//ensemble de nutriments:
typedef std::vector<Nutriment*> Nutriments;
//ensemble de swarms:
typedef std::map<std::string, Swarm*> Swarms;
//ensemble de swarms:
typedef std::vector<PlasmidBacterium*> Plasmidbacteriums;

class PetriDish : public CircularBody, public Drawable, public Updatable
{
public:
  //CONSTRUCTEUR de sous classe PetriDish et de super classe Circularbody
  PetriDish (Vec2d const& vect, double const& rayon);
  //SUPPRESSION DU CONSTRUCTEUR DE COPIE
  PetriDish (PetriDish const&) = delete;

  //METHODES
  //pas virtuel car plus de polymorphisme car un seul swarm 
  bool addSwarm (Swarm*);
  virtual bool addBacterium (Bacterium*);
  virtual bool addNutriment (Nutriment*);
  void update (sf::Time dt);
  void drawOn (sf::RenderTarget& targetWindow)const;
  Nutriment* getNutrimentColliding (CircularBody const& body) const;
  Swarm* getSwarmWithId (std::string const& identificateur) const;
  void reset();
  double getTemperature() const;
  double positionScore(const Vec2d& position) const;
  PlasmidBacterium* getPlasmidProche (PlasmidBacterium* bacterie) const;
  size_t getNutrimentsSize() const;
  unsigned int getNutrimentsQuantity() const;
  bool deletePlasmidBacterium (PlasmidBacterium* bacterie);
  bool addPlasmidBacterium (PlasmidBacterium* bacterie);
  
  //DESTRUCTEUR
  ~PetriDish();
	
private: 
  Swarms m_swarms;
  Bacteries m_bacteries; 
  Nutriments m_nutriments;
  Plasmidbacteriums m_plasmidBacteriums;
}; 

#endif
