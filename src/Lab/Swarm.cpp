#include "Application.hpp"

#include "Swarm.hpp"

//CONSTRUCTEUR
Swarm::Swarm (std::string identificateur)
  : m_identificateur (identificateur)
  , m_ptrLeader (nullptr)
{}

//DESTRUCTEUR
Swarm::~Swarm()
{
  for (auto& bacterie : m_swarmBacteriums)
  {
    delete bacterie;
    bacterie = nullptr;
  }
  m_swarmBacteriums.clear();
}

//METHODES
Vec2d Swarm::getPositionLeader() const
{
  if (m_ptrLeader != nullptr)
  { 
    return m_ptrLeader->getPosition();
  }
  Vec2d vecteurNul(0,0);
  return vecteurNul;
}

std::string Swarm::getIdentificateur() const
{
  return m_identificateur;
}

MutableColor Swarm::getColor() const
{
  return getConfig()["color"];
}

j::Value& Swarm::getConfig() const
{
  return getAppConfig()["swarms"][m_identificateur];
}

void Swarm::addSwarmBacterium (SwarmBacterium* bacterie)
{
  m_swarmBacteriums.push_back(bacterie);
}

bool Swarm::tableauIsAlone() const 
{
  return (m_swarmBacteriums.size() == 1); 
}
  
//METHODES OVERRIDE
void Swarm::update (sf::Time dt) 
{
  m_swarmBacteriums.erase (std::remove(m_swarmBacteriums.begin(),
                                       m_swarmBacteriums.end(), nullptr),
									   m_swarmBacteriums.end());
  //trouve la bactérie avec la meilleure position => score
  //score négatif pour que dès le début la premiere bacterie 
  //à le meilleur score pour être sur
  double scoreMax (0.0);
  SwarmBacterium* leader (nullptr);
  
  for (auto const& bacterium : m_swarmBacteriums)
  {
	double score (getAppEnv().getPositionScore (bacterium->getPosition()));
	
	if (score >= scoreMax)
	{
	  leader = bacterium;
	  scoreMax = score;
	}
  }
  
  //si l'ancien leader est différent du nouveau alors on change de leader
  // sans oublier de changer la variable isLeader des 2 bactéries
  if (m_ptrLeader != leader && leader != nullptr)
  {
	if (m_ptrLeader != nullptr)
	{
      m_ptrLeader->noLeader();
	}
	
    m_ptrLeader = leader;
    m_ptrLeader->beLeader();
    //on ne peut pas suprrimer la case mémoire de leader sinon ça
    //supprimera aussi celle de m_ptrLeader mais on veut plus du pointeur
  }
}

//permet de supprimer la bactérie morte de son tableau de swarm, en plus
//de la supprimer au niveau de son destructeur en tant que bactérie à 
//swarm (elle est supprimer au niveau de PétriDish
void Swarm::deleteBacterium (SwarmBacterium* bacterie) 
{
   m_swarmBacteriums.erase (std::remove(m_swarmBacteriums.begin(),
                                        m_swarmBacteriums.end(), bacterie),
									    m_swarmBacteriums.end());
}
