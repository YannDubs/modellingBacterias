#ifndef STATS_HPP
#define STATS_HPP

#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Graph.hpp"

#include "Interface/Drawable.hpp"
#include "Interface/Updatable.hpp"

typedef std::pair<std::string, std::unique_ptr<Graph>> TitreGraph;
//map est déjà un tableau
typedef std::map <unsigned int, TitreGraph> Graphes;

class Stats : public Drawable, public Updatable
{
public:
  //METHODES
  void addGraph (unsigned int const& identifiant, std::string const& title,
				std::vector<std::string> const& series, double min, 
				double max, Vec2d const& size);
  void reset();
  void setActive (unsigned int const& identifiant);

  //VIRTUAL PUR OVERRIDE
  virtual void drawOn (sf::RenderTarget& target) const override;
  virtual void update (sf::Time dt) override;
  
private:
  Graphes m_graphes;
  unsigned int m_identifiantActif;
  sf::Time m_compteur;
}; 

#endif
