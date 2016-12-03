#include <cmath>
#include "PetriDish.hpp"
#include "Application.hpp"
#include "PlasmidBacterium.hpp"

#include "Utility/Utility.hpp"

//CONSTRUCTEUR
PetriDish::PetriDish (Vec2d const& vect, double const& rayon)
  : CircularBody (vect,rayon)
{}

//METHODES:

//Peuple l'assiette(Si on reussit à placer bacterie ou non)
bool PetriDish::addSwarm (Swarm* swarm)
{
  if (swarm != nullptr)
  {
  std::string identificateur (swarm->getIdentificateur());
  auto paire = m_swarms.find (identificateur);  
  // si la paire existe déjà alors on doit lancer une exception
  // car on veut seulement ajouter et pas modifié
  if (paire != m_swarms.end())
  {
	// le outOfRange ou InvalidArguments??????????
    throw std::invalid_argument ("La clef existe déjà");
  } 
  
  m_swarms[identificateur] = swarm; 
  return true;
  }
  
  //return false;
}

bool PetriDish::addBacterium (Bacterium* bacterie)
{
  //on test si la Bacterie que l'on veut poser existe et qu'elle
  // est dans la boite de Petri
  if (bacterie != nullptr && isInside (*bacterie))
  {
    m_bacteries.push_back (bacterie);
    return true;
  }
  
  return false;
}

//Ajoute nutriment et nous confirme si nutriment posé ou pas
bool PetriDish::addNutriment (Nutriment* nutriment)
{
  if (nutriment != nullptr && isInside (*nutriment))
  {
    m_nutriments.push_back (nutriment);
    return true;
  }
  
  return false;
//fait evoluer bacteries et nutriments à chaque tour de boucle
//on modifie le contenu du pointeur on ne peut passer en prédicat
}

void PetriDish::update (sf::Time dt)
{
  // m_bacteria est la collection de bactéries
  size_t taille (m_bacteries.size());
  // attention pas m_bacteria.size() ici 
  for (size_t i (0); i < taille; ++i)
  { 
    // si cela cause une division, la bactérie
    // issue de la division sera placée à la fin de m_bacteria
    m_bacteries[i]->update (dt); 
    if (m_bacteries[i]->dead())
    {
	  delete m_bacteries[i];
	  m_bacteries[i] = nullptr; 
	}
  }
  m_bacteries.erase (std::remove(m_bacteries.begin(),
                                 m_bacteries.end(), nullptr),
                     m_bacteries.end());
  
  for (auto& swarm : m_swarms)
  {
    (swarm.second)->update (dt);
  }
  
  //les autos doivent être en référence pour pouvoir supprimer
  //les cases du vrai tableau et pas de la copie
  for (auto& nutriment : m_nutriments)
  {
	if (nutriment != nullptr) 
	{
      if (nutriment->isEmpty())
      {
        delete nutriment;
        nutriment = nullptr;
      }
      else if (isInside (*nutriment))
      {
        nutriment->update (dt); //*(nutriment.update (dt))
      }
    }
  }
  //supprime les cases vides du tableau
  m_nutriments.erase (std::remove(m_nutriments.begin(),
                                  m_nutriments.end(), nullptr),
                      m_nutriments.end());
}
    
//Dessine sur le panel graphique
void PetriDish::drawOn (sf::RenderTarget& targetWindow) const
{
  auto border = buildAnnulus (m_position, m_radius, sf::Color::Black, 5);
  targetWindow.draw (border);
  for (auto const& nutriment : m_nutriments)
  {
    nutriment->drawOn (targetWindow);
  }
  for (auto const& bacterie : m_bacteries)
  {
      bacterie->drawOn (targetWindow);
  }
}
    
Nutriment* PetriDish::getNutrimentColliding (CircularBody const& body) const
{
  for (auto const& nutriment: m_nutriments)
  {
    if (nutriment->isColliding (body))
    {
      return nutriment;
    }
      //return nullptr;
      //à cette ligne segfaut car si la premiere bacterie
      // meurt alors on retourne toujours un nullptr
  }
  return nullptr;
}

Swarm* PetriDish::getSwarmWithId (std::string const& identificateur) const
{
  auto paire = m_swarms.find (identificateur);
  if (paire == m_swarms.end())
  {
	// le outOfRange ou InvalidArguments??????????
    throw std::out_of_range ("pas de clef trouvé");
  }
  return paire->second;
}
    
void PetriDish::reset()
{	
  for (auto& nutriment : m_nutriments)
  {
    delete nutriment;	//on vide la case mémoire
    nutriment = nullptr; //on desaffecte l'adresse de la case
  }
  for (auto& bacterie : m_bacteries)
  {
    delete bacterie;
    bacterie = nullptr;
  }
  m_nutriments.clear();
  m_bacteries.clear();
  //après avoir vider toutes les cases du tableau,
  //on reintialise le tabelau de bacteries et nutriments
  m_swarms.clear(); 
  m_plasmidBacteriums.clear(); 
}

double PetriDish::getTemperature() const
{
  return (getAppConfig()["petri dish"]["temperature"].toDouble());
}

double PetriDish::positionScore (const Vec2d& position) const
{
  double scoreP (0.0);
  
  for (auto nutriment : m_nutriments)
  {
	double scorePosition (nutriment->positionScore (position));
    scoreP += scorePosition; 
  }
  
  return scoreP;
}

size_t PetriDish::getNutrimentsSize() const
{
  return m_nutriments.size();
}

unsigned int PetriDish::getNutrimentsQuantity() const 
{
  unsigned int nutrimentsQuantity (0); 
  for (auto const& nutriment : m_nutriments)
  {
    nutrimentsQuantity += nutriment->getQuantity(); 
  }
  return nutrimentsQuantity; 
}
  
//DESTRUCTEUR
PetriDish::~PetriDish()
{
  reset();
}

PlasmidBacterium* PetriDish::getPlasmidProche (PlasmidBacterium* bacterie) const
{
	PlasmidBacterium* lastBacterie (bacterie->getLastBacterie());
  	if (m_plasmidBacteriums.size() != 1)
  	{
  	//on stock une variable pour connaitre le score max
	double distanceMin (1000);
	PlasmidBacterium* plusProchePlasmid (nullptr);
	for (auto const& plasmidBacterium: m_plasmidBacteriums)
    {
	  double lengh (distance (plasmidBacterium->getPosition(), bacterie->getPosition()));
	  //on doit regarder si la bactérie n'est ni elle meme ni la derniere
	  bool autreBacterie (plasmidBacterium != bacterie && plasmidBacterium != lastBacterie);
	  if (lengh < distanceMin && autreBacterie)
	  {
	    plusProchePlasmid = plasmidBacterium;
	    distanceMin = lengh;
	  }
    }
    return plusProchePlasmid;
   }
   return nullptr;
}

bool PetriDish::addPlasmidBacterium (PlasmidBacterium* bacterie)
{
  //on test si la Bacterie que l'on veut poser existe et qu'elle
  // est dans la boite de Petri
  if (bacterie != nullptr && isInside (*bacterie))
  {
    m_plasmidBacteriums.push_back (bacterie);
    return true;
  }
  return false;
}

bool PetriDish::deletePlasmidBacterium (PlasmidBacterium* bacterie)
{
   m_plasmidBacteriums.erase (std::remove (m_plasmidBacteriums.begin(),
                                        m_plasmidBacteriums.end(), bacterie)
							  , m_plasmidBacteriums.end());
}	
