#include "TwitchingBacterium.hpp"
#include "Application.hpp"
#include "Nutriment.hpp"

#include "Random/Random.hpp"
#include "Utility/Vec2d.hpp"
#include "Utility/Constants.hpp"

//on initialise le compteur du nombre de bactérie à zero hors de 
// la classe car variable static ne peut pas etre initialiser dans
// classe
unsigned int TwitchingBacterium::twitchingBacteriumCompteur = 0;

//CONSTRUCTEUR

//CONSTRUCTEUR
TwitchingBacterium::TwitchingBacterium (Vec2d const& position)
  : Bacterium(uniform (getConfig()["energy"]["min"].toDouble(),
                         getConfig()["energy"]["max"].toDouble())
               , position
               , Vec2d::fromRandomAngle()
               , (uniform (getConfig()["radius"]["min"].toDouble(),
                           getConfig()["radius"]["max"].toDouble()))
               , getConfig()["color"]
               )
  , m_grip (m_position, m_radius / 4) 
  , m_state (IDLE)
{
  //ajout des parametre mutables
  //postive est une fonction static => résolution portée
  MutableNumber longueurMaxTentacle (MutableNumber::positive (getConfig()
						             ["tentacle"]["length"]));
  addProperty ("longueur max tentacle", longueurMaxTentacle);
  
  MutableNumber tentacleSpeed (MutableNumber::positive (getConfig()
						       ["tentacle"]["speed"]));
  addProperty ("tentacle speed", tentacleSpeed);
  
  //incrément le compteur de 1 à chaque appel du constructeur
  ++twitchingBacteriumCompteur;
}

//CONSTRUCTEUR DE COPIE
// on fait une surcharge pour faciliter la maintenance pour ne
// par avoir à changer le constructuer de copie
TwitchingBacterium::TwitchingBacterium (TwitchingBacterium const& autre, 
									    int const& valeur) 
: TwitchingBacterium (autre)
{ 
  twitchingBacteriumCompteur += valeur; 
}
  
//METHODES
//notre fonction moveGrip est fondamentalement pensée différemment 
//afin d'éviter des duplications de code nous utilisions donc 
//pour passer les test une valeur par defaut de dt qui est 1 
//vu que pour nous le move grip a pas besoin de dt mais que
//vous l'appeler sans dt (il faudrais mieux theoriquement 
//changer votre code )
void TwitchingBacterium::moveGrip (Vec2d const& direction, sf::Time const& dt)
{
  double tentacleSpeed (getProperty ("tentacle speed").get()); 
  Vec2d newDirection (direction * tentacleSpeed * dt.asSeconds()); 
  m_grip.move (newDirection); 
  double energySpent (getConsommationTentacle() * 
					  tentacleSpeed * dt.asSeconds()); 
  consumeEnergy (energySpent);
}

//GETTEURS
Quantity  TwitchingBacterium::getConsommationMove() const
{
  return getConfig()["energy"]["consumption factor"]["move"].toDouble(); 
} 

Quantity  TwitchingBacterium::getConsommationTentacle() const
{
  return getConfig()["energy"]["consumption factor"]["tentacle"].toDouble(); 
}

//METHODES OVERRIDE
void  TwitchingBacterium::drawOn (sf::RenderTarget& target) const 
{
  m_grip.drawOn (target, m_color); 
  Bacterium::drawOn (target); 
  const double epaisseurTrait (1); 
  Vec2d positionGrip (m_grip.getPosition()); 
  auto line = buildLine (m_position, positionGrip, m_color.getColor()
						, epaisseurTrait);
  target.draw (line);	
}

//VIRTUAL PUR OVERRIDE
Bacterium* TwitchingBacterium::copie() const
{
  //On crée une instance de TwitchingBacterie celle ci est 
  //une copie de la TwitchingBacterie de l'instance courante 
  //il faut incrémenter de 2 vu que juste au dessus on crée une bactérie
  //pour lui faire un noLeader
  TwitchingBacterium twitchingBacterie (*this, 2);
  //grip prend la position de la copie de bacterie et le radius/4 (cf énonce) 
  //sinon les deux grips sont de même longueur et la position de la fin du
  //grip serait la même 
  Vec2d clonePosition (twitchingBacterie.getPosition());
  twitchingBacterie.m_grip.setPosition (clonePosition); 
  twitchingBacterie.m_grip.setRadius (m_radius / 4);  
  Bacterium* newTwitch (new TwitchingBacterium (twitchingBacterie));
  return newTwitch;
}

Quantity TwitchingBacterium::getConsoEnergy() const
{
  return getConfig()["energy"][ "consumption factor"].toDouble(); 
}

void  TwitchingBacterium::move (sf::Time dt)
{
  switch (m_state) 
  {
	//Tentacule au repos 
	case IDLE: 
	{
	  m_state = WAIT_TO_DEPLOY; 
	  break; 
    }
    
	//Tentacule se préparant au deploiement 
	case WAIT_TO_DEPLOY:
	{
	  setBestDirection();
	  m_state = DEPLOY; 
	  break; 
	}
	
	//Tencule se deploit
	case DEPLOY: 
	{
	  moveGrip(m_direction, dt); 
	  
	  //Trouver si il y a une source de nutriment en collision 
	  //avec mon grapin 
	  Nutriment* nutriment (getAppEnv().getNutrimentColliding (m_grip));
	  if (nutriment != nullptr)
	  {
	    m_state = ATTRACT; 
	    break; 
	  } 
	  //Trouver la longueur entre la grip et si on collide avec le bord 
	  double tentacleMaxLength (getProperty ("longueur max tentacle").get()); 
	  double longueurTentacle (distance (m_position, m_grip.getPosition()));
	  bool isColliding (getAppEnv().doesCollideWithDish (m_grip)); 
	  if ((tentacleMaxLength <= longueurTentacle) || (isColliding) )
	  {
	    m_state = RETRACT; 
	  }
	  break;
	}
	
	//tentacule qui attracte la bectrie vers la source de nutriment 
	case ATTRACT: 
	{
	  Nutriment* nutriment (getAppEnv().getNutrimentColliding (m_grip));
	  if (nutriment != nullptr) 
	  {  
	    Nutriment* nutriment (getAppEnv().getNutrimentColliding (*this));
	    if (nutriment != nullptr) 
	    {
	      m_state = EAT; 
	      break; 
	    }
	    else
	    {
		  Vec2d directionTentacule (m_grip.getPosition() - m_position);
		  double tentacleSpeed (getProperty ("tentacle speed").get()); 
		  //donne la direction de déploiment du grapin/tentacule
		  double speedFactor (getConfig()["speed factor"].toDouble()); 
		  Vec2d deplacement (directionTentacule.normalised()
							 * tentacleSpeed * dt.asSeconds()* speedFactor);
		  double longueurDeplacement (deplacement.length());   				   
		  CircularBody::move (deplacement); 
		  consumeEnergy (longueurDeplacement * getConsommationMove()); 
		  break; 
	      //dans notre cas on pense que les grips se retractent au
	      //moment ou un a plus de nutriment (soit nullpptr), mais 
	      //pourquoi ce grip ne poursuit pas sa croissance ??? 
	    }
	  }
	  else 
	  {
	    m_state = RETRACT; 
	    break; 
	  }
	}
	
	//Tentacule se retractant 
	case RETRACT:  
	{
	  double distanceGrapinBacterie (distance (m_position, m_grip.getPosition()));
	  if (distanceGrapinBacterie <= (m_radius - m_grip.getRadius()))
	  {
	    m_state = IDLE; 
	    break;
	  }
	  else 
	  {
		//donne la direction opposée à la direction de déploiment 
	    Vec2d directionTentacule (m_position - m_grip.getPosition());
	    //on calcule le déplacement de la tentacule et on soustrait
	    //l'énergie que coute ce déplacement
	    moveGrip (directionTentacule.normalised(), dt);
	    break; 
	  }
	}
	
	//Bactérie en train de consommer de la nourriture 
	case EAT:
	{
	  //si le corpsCiruclaire du grapin et la bactérie ne sont plus en collision 
	  Nutriment* nutriment (getAppEnv().getNutrimentColliding (*this));
	  if (nutriment == nullptr) 
	  {
	    m_state = IDLE; 
	    break; 
	  }
    }
  }
}

j::Value& TwitchingBacterium::getConfig() const 
{
  return getAppConfig()["twitching bacterium"];
}

//SURCHARGE DE METHODE VIRTUELLE PURES OVERRIDE
Quantity TwitchingBacterium::eatableQuantity (NutrimentA& nutriment)
{
  return nutriment.eatableBy (*this); 
}

Quantity TwitchingBacterium::eatableQuantity (NutrimentB& nutriment) 
{
  return nutriment.eatableBy (*this); 
}

//STATIC
unsigned int TwitchingBacterium::getTwitchingCompteur() 
{
  return twitchingBacteriumCompteur;
}

void TwitchingBacterium::resetTwitchingCompteur()
{
 twitchingBacteriumCompteur = 0; 
}
  




//DESTRUCTEUR
TwitchingBacterium::~TwitchingBacterium ()
{
  //décremente le compteur de 1 à chaque appel du constructeur
  --twitchingBacteriumCompteur;
}

