#include "Lab.hpp"
#include "Application.hpp"

//pour accéder aux compteur static
#include "TwitchingBacterium.hpp"
#include "SimpleBacterium.hpp"
#include "SwarmBacterium.hpp"
#include "PlasmidBacterium.hpp"

#include "Utility/Constants.hpp"

//CONSTRUCTEUR
Lab::Lab() 
  : m_nutriCreate()
  , m_boitePetri ((getApp().getCentre())
  , ((getApp().getWorldSize().x) * 0.95 * 0.5))
{}

//DESTRUCTEUR
Lab::~Lab()
{}

//METHODES
void Lab::drawOn (sf::RenderTarget& targetWindow) const
{
  m_boitePetri.drawOn (targetWindow);
}
		 
void Lab::update (sf::Time dt)
{
  m_boitePetri.update (dt);
  m_nutriCreate.update (dt);
} 

void Lab::reset()
{
  m_boitePetri.reset();
  SwarmBacterium::resetSwarmCompteur();
  TwitchingBacterium::resetTwitchingCompteur();
  SimpleBacterium::resetSimpleCompteur();
  PlasmidBacterium::resetPlasmidCompteur();
  m_nutriCreate.reset();
}

bool Lab::doesCollideWithDish (CircularBody const& body) const
{
  return (m_boitePetri.isColliding (body) && 
		  not (m_boitePetri.isInside (body)));
}

Nutriment* Lab::getNutrimentColliding (CircularBody const& body) const
{
  return m_boitePetri.getNutrimentColliding (body);
}

Swarm* Lab::getSwarmWithId (std::string const& identificateur) const
{
  return m_boitePetri.getSwarmWithId (identificateur);
}
//une telle méthode est demandé mais elle nous est inutile puisque nous faisons
//les tests nécessaires ailleurs:
//isInsidePetri n'est jamais utilisée,
//nous passons par des test dans petridish::addNutriment. Néanmoins 
//pour le principe de montrer que nous aurions été capables de la 
//programmer nous la laissons en commentaire: 
/*bool Lab::isInsidePetri(CircularBody const& body) const 
{
  return m_boitePetri.isInside(body);
}*/

bool Lab::addSwarm (Swarm* swarm)
{
  return m_boitePetri.addSwarm (swarm);
}

bool Lab::addNutriment (Nutriment* nutriment)
{
  //on voulait borner le nombre de nutriment à 200 pour se conformer 
  //au cadre du graphe 
  unsigned int maxNutriment (200); 
  if (m_boitePetri.getNutrimentsSize() < maxNutriment) 
  {
    return m_boitePetri.addNutriment (nutriment);
  }
}

bool Lab::addBacterium (Bacterium* bacterie)
{ 
  //On aimerait changer le code afin de bloquer l'ajout d'un certain 
  //type de bactéries à 200 (en bloquant directment les touches 
  //"pleines" avant même de construire )
  return m_boitePetri.addBacterium (bacterie);
}

double Lab::getPositionScore (const Vec2d& position) const
{
  // le calcul score(p) = taille(s) / (distance(p,centre(s))puissance 
  // est fait dans le petridish pour avoir accès au nutriments
  //sans casser l'encapsulation
  return m_boitePetri.positionScore (position);
}

Data Lab::fetchData (std::string const& title) const
{
  // dans application c'est en double de nombre mais on trouve plus
  // logique en unsigned int 
  Data newData ;
  //on ne peut pas utiliser des switch avec des string
  if (title == s::GENERAL)
  {
	double dishTemperature (getAppConfig()["petri dish"]["temperature"].toDouble());
	double compteurSwarm (SwarmBacterium::getSwarmCompteur());
	double compteurTwitching (TwitchingBacterium::getTwitchingCompteur());
	double compteurSimple (SimpleBacterium::getSimpleCompteur());
	double compteurPlasmid (PlasmidBacterium::getPlasmidCompteur());
	// notons que pour les swarms on pourrait simplement faire un swarm.size()
	// CHANGER nous avon décidé de mettre la température aussi
    newData = {
			    {"simple bacteria", compteurSimple},
                {"twitching bacteria", compteurTwitching},
                {"swarm bacteria", compteurSwarm},
                {"plasmid bacteria", compteurPlasmid},
                {"nutriment sources", m_boitePetri.getNutrimentsSize()},
                {"temperature", dishTemperature}
              };
  }
  else if (title == s::NUTRIMENT_QUANTITY)
  {
	newData  = {{"nutriment quantity", m_boitePetri.getNutrimentsQuantity()}};
  }
  
  return newData ;
}

PlasmidBacterium* Lab::getPlasmidProche (PlasmidBacterium* bacterie) const
{
  return m_boitePetri.getPlasmidProche (bacterie);
}

bool Lab::addPlasmidBacterium (PlasmidBacterium* bacterie)
{
  return m_boitePetri.addPlasmidBacterium (bacterie);
}

bool Lab::deletePlasmidBacterium (PlasmidBacterium* bacterie)
{
  return m_boitePetri.deletePlasmidBacterium (bacterie);
}
