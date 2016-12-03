#include "Application.hpp"
#include "SwarmBacterium.hpp"

#include "Utility/MutableNumber.hpp"

//on initialise le compteur du nombre de bactérie à zero hors de 
// la classe car variable static ne peut pas etre initialiser dans
// classe
unsigned int  SwarmBacterium::swarmBacteriumCompteur = 0;

//CONSTRUCTEUR
SwarmBacterium::SwarmBacterium (Vec2d const& position, Swarm* ptrSwarm)
  :  Bacterium (uniform (getConfig()["energy"]["min"].toDouble(),
                         getConfig()["energy"]["max"].toDouble())
               , position
               , Vec2d::fromRandomAngle()
               , (uniform (getConfig()["radius"]["min"].toDouble(),
                           getConfig()["radius"]["max"].toDouble()))
               , ptrSwarm->getColor()
               )
     , m_isLeader (false)
     , m_ptrSwarm (ptrSwarm)
     , m_probabiliteBasculement (0.0)
     , m_tempsBasculement (sf::Time::Zero)
{
  //ajoute la bactérie a son swarm
  m_ptrSwarm->addSwarmBacterium (this);
  
  //si la bacterie est seul alors elle devient leaders
  if (m_ptrSwarm->tableauIsAlone())
  {
	(*this).beLeader(); 
  }

  //incrément le compteur de 1 à chaque appel du constructeur
  ++swarmBacteriumCompteur;
}

//CONSTRUCTEUR DE COPIE 
// on fait une surcharge pour faciliter la maintenance pour ne
// par avoir à changer le constructuer de copie
SwarmBacterium::SwarmBacterium (SwarmBacterium const& autre, int const& valeur)
: SwarmBacterium (autre) 
{
  swarmBacteriumCompteur += valeur; 
  m_ptrSwarm->addSwarmBacterium (this);
}

//METHODES
void SwarmBacterium::updateProbabiliteBasculement (Vec2d const& anciennePosition)
{
  double ancienScore (getAppEnv().getPositionScore (anciennePosition));
  double nouveauScore (getAppEnv().getPositionScore (m_position));
  
  //on donne des valeurs au bol pour les lambda car vous avez demandez 
  // de ne plus avoir tumble better et tumble worse qui peuvent muter
  double lambda (0.0);
  if (nouveauScore >= ancienScore)
  {
    lambda = 5;
  }
  else
  {
	lambda = 0.5;  
  }
  
  m_probabiliteBasculement = 1 - exp (-m_tempsBasculement.asSeconds() / lambda);
}


void SwarmBacterium::basculement()
{
  //si je peux basculer
  bool isBascule (bernoulli (m_probabiliteBasculement));
  if (isBascule)
  {
	  setBestDirection();
  } 
	
  //remet le compteur a zero si elle a basculé
  m_tempsBasculement = sf::Time::Zero;
}

void SwarmBacterium::beLeader ()
{
  m_isLeader = true;
}

void SwarmBacterium::noLeader ()
{
  m_isLeader = false;
}

//GETTEURS
Vec2d SwarmBacterium::getSpeedVector (int const& valeur) const
{
  return valeur * m_direction;
}

//METHODES OVERRIDE
void SwarmBacterium::drawOn (sf::RenderTarget& target) const
{
  Bacterium::drawOn (target);
   
  if (isDebugOn() && m_isLeader)
  {
	double radius (getConfig()["radius"]["max"].toDouble());
	const double epaisseurAnneau (2.0);
    auto const border = buildAnnulus (m_position, radius*1.2 , sf::Color::Red 
									 , epaisseurAnneau);
    target.draw (border);
  }
}

//VIRTUAL PUR OVERRIDE
void SwarmBacterium::move (sf::Time dt)
{
  //on stock l'ancienne position pour le basculemnt avant de la changer
  //il faudrait le faire que si c'est le leader mais ça prend peu de place
  // et on a a peur que la bactérie change son is leader entre temps
  Vec2d anciennePosition (m_position);

  double speedValeur (getConfig()["speed"]["initial"].toDouble());
  Vec2d speed (getSpeedVector (speedValeur));
  DiffEqResult tableauPositionSpeed (stepDiffEq (m_position, speed, dt, *this));
  Vec2d deplacement (tableauPositionSpeed.position - m_position);
  double deplacementCarre (deplacement.lengthSquared());
  
  if (deplacementCarre > 0.001)
  {
    m_position = tableauPositionSpeed.position;
  }
  double longueurDeplacement (deplacement.length());
  Quantity consoEnergy (longueurDeplacement * getConsoEnergy());
  consumeEnergy (consoEnergy);
  
  //seul le leader doit choisir la bonne direction
  if (m_isLeader)
  {
  
  //BASCULEMENT
  //on incremente a chaque fois le compteur pour le mettre à jour
  m_tempsBasculement += dt;
  
  //on met a jour la probabilite de basculement apres le move 
  //car il faut connaitre la nouvelle et ancienne position
  updateProbabiliteBasculement (anciennePosition);
  basculement();
  }
}

j::Value& SwarmBacterium::getConfig() const
{
  return getAppConfig()["swarm bacterium"];
}

//la fonction prend 2 arguments meme si c'est des arguments de classe
//car elle doit overrider la fonction de  DiffEqFunction
Vec2d SwarmBacterium::f(Vec2d position, Vec2d speed) const
{
  double coefForceSwarm (m_ptrSwarm->getConfig()["force factor"].toDouble());
  Vec2d positionLeader (m_ptrSwarm->getPositionLeader());
  // la fonction get position leader renvois la position nul si il 
  // n'y pas de leader (ex au début) alors il faut pas que les
  // bactéries soit attirée la bas + il n'y a pas de risque si la 
  // position est vraiment 0.0 car hors du dish
  const Vec2d vecteurNul (0,0);
  if  (positionLeader == vecteurNul)
  {
    return coefForceSwarm * (m_position - position);
  }
  
  return coefForceSwarm * (positionLeader - position);
}

Bacterium* SwarmBacterium::copie () const
{
  // il faut que la bactérie cloner ne soit pas un leader 
  // pour ne pas avoir de plusieurs leader clone de leader
  //on ne peut pas faire -> no leader car dans ptr de bacterie
  SwarmBacterium swarmBacterie (*this);
  swarmBacterie.noLeader(); 
  //il faut incrémenter de 2 vu que juste au dessus on crée une bactérie
  //pour lui faire un noLeader
  Bacterium* ptrSwarmBacterie (new SwarmBacterium (swarmBacterie, 2));
  return ptrSwarmBacterie;
}

Quantity SwarmBacterium::getConsoEnergy() const
{
  return getConfig()["energy"][ "consumption factor"].toDouble(); 
}

//SURCHARGE DE METHODE VIRTUELLE PURES OVERRIDE
Quantity SwarmBacterium::eatableQuantity (NutrimentA& nutriment) 
{
  return nutriment.eatableBy(*this); 
}

Quantity SwarmBacterium::eatableQuantity (NutrimentB& nutriment) 
{
  return nutriment.eatableBy(*this); 
}

//STATIC
unsigned int SwarmBacterium::getSwarmCompteur()
{
  return swarmBacteriumCompteur;
}

void SwarmBacterium::resetSwarmCompteur()
{
  swarmBacteriumCompteur = 0; 
}

//DESTRUCTEUR
SwarmBacterium::~SwarmBacterium()
{
  m_ptrSwarm-> deleteBacterium (this); 

  //décremente le compteur de 1 à chaque appel du constructeur
  --swarmBacteriumCompteur;
}
  

















