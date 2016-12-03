#include <unordered_map>

#include "Application.hpp"
#include "Stats.hpp"
#include "iostream"

  //METHODES
void Stats::addGraph (unsigned int const& identifiant, 
					  std::string const& title,
					  std::vector<std::string> const& series, 
					  double min, 
					  double max, Vec2d const& size)
{
  auto paire = m_graphes.find (identifiant);  
  
  if (paire != m_graphes.end())
  {
	//on fait appel au reset des unique_ptr (remplace l'adresse actuelle 
	//du unique_ptr par le nouveau pointeur que l'on fournit) 
    ((paire->second).second).reset (new Graph (series, size, min, max));
    // on ne peut pas stocké les valeurs dans des variables car
    // on ne peut pas avoir de unique ptr qui pointe sur la meme valeur
    (paire->second).first = title;
  } 
  else
  {
	//on ne peut pas déclarer de variable contenant un unique ptr
	// comme un TitreGraph qu'on associe après à la clef dans le map
	// car les unique ptr ne peuvent pas pointer sur 2 elements en meme
	// temps. C'est pour ça qu'on utilise la fonction move des 
	// unique ptrs qui supprime au préalable au lcase mémoire pointée
	// par le premier pointeur
	std::unique_ptr<Graph> ptrGraph (new Graph (series, size, min, max));
	TitreGraph titreGraph (title, std::move (ptrGraph));
    m_graphes[identifiant] = std::move (titreGraph); 
  }
  
  setActive (identifiant);
}

void Stats::reset()
{
  for (auto& pairTitreGraphe : m_graphes)
  {
    // on ne peut pas stocké les valeurs dans des variables car
    // on ne peut pas avoir de unique ptr qui pointe sur la meme valeur
    ((pairTitreGraphe.second).second)->reset();
  }
}

void Stats::setActive (unsigned int const& identifiant)
{
  m_identifiantActif = identifiant;
}

  //METHODE VIRTUELLE PURES OVERRIDE
  
void Stats::drawOn (sf::RenderTarget& target) const
{
  auto paire = m_graphes.find (m_identifiantActif); 
   
  if (paire == m_graphes.end())
  {
    throw std::invalid_argument ("pas de clef trouvé");
  } 
  // on ne peut pas stocké les valeurs dans des variables car
  // on ne peut pas avoir de unique ptr qui pointe sur la meme valeur
  ((paire->second).second)->drawOn (target);
}

void Stats::update (sf::Time dt)
{
  //On incrémente à chaque appel 
  m_compteur += dt;
  double refreshRate (getAppConfig()["stats"]["refresh rate"].toDouble()); 
  
  if (m_compteur > sf::seconds (refreshRate))
    {
	  //on met à jour les deux graphes en même temps (pour avoir des 
	  //courbes qui se suivent au moment oû l'on switch de graphe 		  
      for (auto& pairTitreGraphe : m_graphes)
      {
        std::string title ((pairTitreGraphe.second).first);
        std::unordered_map <std::string, double> newData (getAppEnv().fetchData (title));
        // on ne peut pas stocké les valeurs dans des variables car
        // on ne peut pas avoir de unique ptr qui pointe sur la meme valeur
        ((pairTitreGraphe.second).second)->updateData (m_compteur, newData);
      }
      
      m_compteur = sf::Time::Zero;
    }
}
