#include <string>

#include "PlasmidBacterium.hpp"
#include "Application.hpp"
#include "Nutriment.hpp"

#include "Random/Random.hpp"
#include "Utility/Constants.hpp"
#include "Utility/Vec2d.hpp"
#include "Random/Random.hpp"

//on initialise le compteur du nombre de bactérie à zero hors de 
// la classe car variable static ne peut pas etre initialiser dans
// classe
unsigned int PlasmidBacterium::plasmidBacteriumCompteur = 0;

//CONSTRUCTEUR
PlasmidBacterium::PlasmidBacterium (Vec2d const& position)
  : Bacterium(uniform (getConfig()["energy"]["min"].toDouble()
                       , getConfig()["energy"]["max"].toDouble())
               , position
               , Vec2d::fromRandomAngle()
               , (uniform (getConfig()["radius"]["min"].toDouble(),
                           getConfig()["radius"]["max"].toDouble()))
               , getConfig()["color"]
               )
    , m_state (SEEK)
    , m_LastBacterie (nullptr)
    , m_PlasmidR (false)
    , m_PlasmidRE (false)
    , m_probabiliteBasculement(0)
    , m_tempsPrive (sf::Time::Zero)
    , m_tempsBasculement (sf::Time::Zero)
 
{
  Vec2d longueurPilus (1.5 * m_radius, 1.5 * m_radius);
  m_pilusSexuel = (PilusSexuel (m_position + longueurPilus, 3));
  
  //ajout des parametre mutables
  //postive est une fonction static => résolution portée
  MutableNumber speed (MutableNumber::positive (getConfig()["speed"]));
  addProperty ("speed", speed);
  MutableNumber lambdaBetter (MutableNumber::positive (getConfig()
										["tumble"]["better"]));
  addProperty ("tumble better", lambdaBetter);
  MutableNumber lambdaWorse (MutableNumber::positive (getConfig()
										["tumble"]["worse"]));
  addProperty ("tumble worse", lambdaWorse);
  
  MutableNumber epaisseurMembrane (MutableNumber::positive (getConfig()
										["resistance"]["epaisseur"]));
  addProperty ("epaisseur membrane", epaisseurMembrane);
  
  //incrément le compteur de 1 à chaque appel du constructeur
  ++plasmidBacteriumCompteur;
  
  //s'ajoute dans le tableau de petridish
  getAppEnv().addPlasmidBacterium (this);
  
  //une partie aleatoire des bactéries seront de bases déjà résistantes
  double probabiliteGenerator (getConfig()["resistance"]
							  ["probabilite resistance"].toDouble());
  if (bernoulli (probabiliteGenerator))
  {
    m_PlasmidR= true;
  }
}

//CONSTRUCTEUR DE COPIE 
// on fait une surcharge pour faciliter la maintenance pour ne
// par avoir à changer le constructuer de copie
PlasmidBacterium::PlasmidBacterium (PlasmidBacterium const& autre, int const& valeur)
: PlasmidBacterium (autre) 
{
  plasmidBacteriumCompteur += valeur;
  getAppEnv().addPlasmidBacterium (this); 
}

//METHODE
void PlasmidBacterium::exchange (PlasmidBacterium* const& bacterie)
{
  bool plasmidR (bacterie->getResistance()[0]);
  // la bactérie a une chance sur 2 de prendre le plasmid 
  // notez qu'elle peut donc la perdre car c'est comme si gagnait
  // un plasmid non resistant
  bool isEchange (bernoulli (getConfig()["resistance"]
							["probabilite echange"].toDouble()));
  if (isEchange)
  {
    m_PlasmidR = plasmidR;
  }
  
  //on se rapelle de la derniere bacterie pour ne pas rester coller à elle
  m_LastBacterie = bacterie;
}

void PlasmidBacterium::deplacement (sf::Time dt)
{
  //on stock l'ancienne position pour le basculemnt avant de la changer
  Vec2d anciennePosition (m_position);
  double speedValeur ((getProperty ("speed")).get());
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
}

void PlasmidBacterium::updateProbabiliteBasculement (Vec2d const& anciennePosition)
{
  double ancienScore (getAppEnv().getPositionScore (anciennePosition));
  double nouveauScore (getAppEnv().getPositionScore (m_position));
  
  double lambda (0.0);
  if (nouveauScore >= ancienScore)
  {
    lambda = (getProperty ("tumble better")).get();
  }
  else
  {
	lambda = (getProperty ("tumble worse")).get();  
  }
  
  m_probabiliteBasculement = 1 - exp (-m_tempsBasculement.asSeconds() / lambda);
}

void PlasmidBacterium::basculement()
{
  //si je peux basculer
  bool isBascule (bernoulli (m_probabiliteBasculement));
  if (isBascule)
  {
	// On test les algo qu'on récupère mais on a pas fait changer dans
	//json
	std::string algorithme (getConfig()["tumble"]["algo"].toString());
	
	//BASCULE 
	if (algorithme == "single random vector")
	{
      m_direction = Vec2d::fromRandomAngle();
    }
	else if (algorithme == "best of N")
	{
	  setBestDirection();
	} 
	//remet le compteur a zero si elle a basculé
    m_tempsBasculement = sf::Time::Zero;
  }
}

//GETTEURS
Vec2d PlasmidBacterium::getSpeedVector (int const& valeur) const
{
  return valeur * m_direction;
}

Etat PlasmidBacterium::getState() const
{
  return m_state;
}

Resistance PlasmidBacterium::getResistance() const
{
  return {m_PlasmidR, m_PlasmidRE};
}

PlasmidBacterium* PlasmidBacterium::getLastBacterie () const
{
  return m_LastBacterie;
}

//METHODES OVERRIDE
void PlasmidBacterium::drawOn (sf::RenderTarget& target) const
{
  Bacterium::drawOn (target);
  auto set_of_points = sf::VertexArray (sf::LinesStrip); 
  set_of_points.append ({{0,0}, m_color.getColor()});

  //il faut donner la meme epaisseur que le rayon du pillus sexuel 
  // pour ne pas faire comme le grip
  const double epaisseurTrait (3); 
  
  Vec2d positionPillus (m_pilusSexuel.getPosition()); 
  auto line = buildLine (m_position, positionPillus, m_color.getColor()
						 , epaisseurTrait);
  target.draw (line);	
  
  double epaisseurMembrane (getProperty ("epaisseur membrane").get());
  if (epaisseurMembrane > 0)
  {
  auto const border = buildAnnulus (m_position, m_radius*1.005 
  , sf::Color::Magenta , epaisseurMembrane);
  target.draw (border);
  }
  
  if (m_PlasmidR)
  {
	//pour ne pas que ça soit sur l'ecriture
    const Vec2d positionPlasmid (7,-7);
    auto const border = buildAnnulus (m_position + positionPlasmid, m_radius/5 
									  , sf::Color::Yellow , 1.5);
    target.draw (border);
  }
  
  if (m_PlasmidRE)
  {
    //pour ne pas que ça soit sur l'ecriture
    const Vec2d positionPlasmid (-7,-7);
    auto const border = buildAnnulus (m_position + positionPlasmid, m_radius/5 
									  , sf::Color::White , 1.5);
    target.draw (border);
  }
}

j::Value& PlasmidBacterium::getConfig()const
{
  return getAppConfig()["plasmid bacterium"];
}

void PlasmidBacterium::update (sf::Time dt)
{
  Bacterium::update (dt);
  const double longueurPilus (1.5 * m_radius);
  Vec2d positionPilus (m_position + m_direction.normalised() * longueurPilus);
  m_pilusSexuel.setPosition (positionPilus);
  // si la membrane est assez epaisse alors la peniciline ne pourra
  //pas pénétrer et donc la bacterie est resistante
  const double epaisseurResistance (2.4);
  if (getProperty("epaisseur membrane").get() >= epaisseurResistance)
  {
    m_PlasmidRE = true;
  }
  else if (m_PlasmidRE == true)
  {
	m_PlasmidRE = false;
  }
  
}

//VIRTUAL PUR OVERRIDE
//la fonction prend 2 arguments meme si c'est des arguments de classe
//car elle doit overrider la fonction de  DiffEqFunction
Bacterium* PlasmidBacterium::copie () const
{
  // ne pas mettre new (SimpleBacterium (*this)) sion met pas
  //a jour le compteur
  Bacterium* plasmidBacterie (new PlasmidBacterium (*this, 1));
  return plasmidBacterie;
}

Quantity PlasmidBacterium::getConsoEnergy() const
{
  return getConfig()["energy"]["consumption factor"].toDouble(); 
}

//il faut quand meme donner des arguments juste pour pouvoir
//overrider la fonction
Vec2d PlasmidBacterium::f(Vec2d position, Vec2d speed) const
{
  Vec2d force (0,0);
  return force;
}

void PlasmidBacterium::move (sf::Time dt)
{
  PlasmidBacterium* bacterieProche (getAppEnv().getPlasmidProche (this));
  switch (m_state) 
  {
	//la bacterie cherche un "partenaire"
	case SEEK:
	{
	  if (bacterieProche != nullptr)
	  {
	    Vec2d direction (bacterieProche->getPosition() - m_position) ;
	    //il ne faut pas qu'elle puisse s'approcher plsu proche que leur
	    //pillus (de longeur 0.5* rayon , notez qu'on aurait du mettre 3
	    //si contacte entre pillus mais pour l'esthetique on a prefere
	    // que le pillus doivent touccher la bacterie 
	    const double distanceMin (2.5);
	    //la bacterie ne peut que conjuguer si l'autre le veut aussi
	    bool isConjugable (bacterieProche->getState() != LIVE );
	    if (direction.length() <= distanceMin * m_radius && isConjugable)
	    {
	      m_state = CONJUGATE;
		  //il faut echanger ici sinon ne fait que echanger (vu que reste 
		  //dans l'etat conjugable et donc peut conjuger avec une bacterie 
		  //qui mourrira => segFault
	      exchange (bacterieProche);
	    }
	    m_direction = direction.normalised();
      }
	  deplacement (dt);
	  break;
	}
	
	//l'etat de conjugaison => s'echange plasmid
	case CONJUGATE:
	{
	  m_tempsPrive += dt;
	  sf::Time tempsAttente = sf::seconds(2);
	  //reste pendant 2 secondes dans cet etatt pour faire genre
      if (m_tempsPrive >= tempsAttente)
      {
		m_tempsPrive = sf::Time::Zero;
	    m_state = LIVE;
	  }
	  break;
    }
    
    //vie normalement c'est a dire comme une bacterie simple en cherchant 
    //les nutriments pendant 10 secondes
	case LIVE:
	{
	  Vec2d anciennePosition (m_position);
	  m_tempsPrive += dt;
	  sf::Time tempsAttente = sf::seconds (10);
	  deplacement (dt); 
	  //BASCULEMENT
	  //on incremente a chaque fois le compteur pour le mettre à jour
	  m_tempsBasculement += dt;

	  //on met a jour la probabilite de basculement apres le move 
	  //car il faut connaitre la nouvelle et ancienne position
	  updateProbabiliteBasculement (anciennePosition);
	  basculement();
	  
      if (m_tempsPrive >= tempsAttente)
      { 
		m_tempsPrive = sf::Time::Zero;
	    m_state = SEEK;
	  }
	  break;
	}
  }
}

//SURCHARGE DE METHODE VIRTUELLE PURES OVERRIDE
Quantity PlasmidBacterium::eatableQuantity (NutrimentA& nutriment)
{
  return nutriment.eatableBy(*this); 
}

Quantity PlasmidBacterium::eatableQuantity (NutrimentB& nutriment)
{
  return nutriment.eatableBy(*this); 
}

//STATIC
unsigned int PlasmidBacterium::getPlasmidCompteur() 
{
	return plasmidBacteriumCompteur;
}

void PlasmidBacterium::resetPlasmidCompteur()
{
  plasmidBacteriumCompteur = 0; 
}

//DESTRUCTEUR
PlasmidBacterium::~PlasmidBacterium ()
{
  //décremente le compteur de 1 à chaque appel du constructeur
  --plasmidBacteriumCompteur;
  
  //se supprime du tableau de petridish
  getAppEnv().deletePlasmidBacterium (this); 
}
  



















  




















