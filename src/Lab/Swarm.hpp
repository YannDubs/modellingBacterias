#ifndef SWARM_HPP
#define SWARM_HPP

#include <string>
#include <vector>

#include "SwarmBacterium.hpp"

#include "Interface/Updatable.hpp"
#include "Utility/MutableColor.hpp"
#include "Utility/Vec2d.hpp"

typedef std::vector<SwarmBacterium*> SwarmBacteriums;

class Swarm : public Updatable 
{
public:
  //CONSTRUCTEUR
  Swarm (std::string identificateur);
  //DESTRUCTEUR
  virtual ~Swarm() ;

  //METHODES
  std::string getIdentificateur() const;
  Vec2d getPositionLeader() const;
  MutableColor getColor() const;
  j::Value& getConfig() const;
  void addSwarmBacterium (SwarmBacterium* bacterie);
  bool tableauIsAlone() const;
  void deleteBacterium (SwarmBacterium* bacterie);
  
  //METHODES OVERRIDE
  virtual void update (sf::Time dt) override;

private:
  std::string m_identificateur; 
  SwarmBacteriums m_swarmBacteriums; 
  SwarmBacterium* m_ptrLeader;
}; 

#endif
