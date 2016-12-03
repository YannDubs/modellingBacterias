#include <string>

#include "SimpleBacterium.hpp"
#include "Application.hpp"
#include "Nutriment.hpp"

#include "Utility/Constants.hpp"
#include "Random/Random.hpp"

//on initialise le compteur du nombre de bactérie à zero hors de 
// la classe car variable static ne peut pas etre initialiser dans
// classe
unsigned int SimpleBacterium::simpleBacteriumCompteur = 0;

//CONSTRUCTEUR

SimpleBacterium::SimpleBacterium (Vec2d const& position)
  :  Bacterium (uniform (getConfig()["energy"]["min"].toDouble(),
                         getConfig()["energy"]["max"].toDouble())
               , position
               , Vec2d::fromRandomAngle()
               , (uniform (getConfig()["radius"]["min"].toDouble(),
                           getConfig()["radius"]["max"].toDouble()))
               , getConfig()["color"]
               )
   , m_probabiliteBasculement (0.0)
   , m_tempsBasculement (sf::Time::Zero)
{
  // le m_tFlagelle doit etre initialisé a une valeur uniforme entre 0 et PI
  m_tFlagelle = uniform (0.0, PI);

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
  
  //incrémente le compteur de 1 à chaque appel du constructeur
  ++simpleBacteriumCompteur;
}

//CONSTRUCTEUR DE COPIE 
SimpleBacterium::SimpleBacterium (SimpleBacterium const& autre, int const& valeur)
  //pour ne pas faire de duplication de code
  : SimpleBacterium (autre) 
{
  // il faut pouvoir incrémenter le compteur lorsqu'on fait une copie 
  // (clone)
  simpleBacteriumCompteur += valeur; 
} 

//METHODES
void SimpleBacterium::updateAngle(sf::Time dt) 
{
  // calcule la différence entre le nouvelle angle de direction et l'ancien
  auto const angleDiff = angleDelta (m_direction.angle(), m_angleRotation); 
  // calcule dα
  auto dalpha = PI * dt.asSeconds();    
  // on ne peut tourner plus que de angleDiff
  dalpha = std::min (dalpha, std::abs(angleDiff)); 
  // on tourne dans la direction indiquée par angleDiff
  dalpha = std::copysign (dalpha, angleDiff); 
  // angle de rotation mis à jour
  m_angleRotation += dalpha;  
}

void SimpleBacterium::updateProbabiliteBasculement (Vec2d const& anciennePosition)
{
  double ancienScore (getAppEnv().getPositionScore (anciennePosition));
  double nouveauScore (getAppEnv().getPositionScore (m_position));
  
  double lambda (0.0);
  if (nouveauScore >= ancienScore)
  {
	// le get permet de passer d'un mutable number à un double
    lambda = (getProperty ("tumble better")).get();
  }
  else
  {
	lambda = (getProperty ("tumble worse")).get();  
  }
  
  m_probabiliteBasculement = 1 - exp ( - m_tempsBasculement.asSeconds() / lambda);
}

void SimpleBacterium::basculement()
{
  //si je peux basculer
  bool isBascule (bernoulli (m_probabiliteBasculement));
  if (isBascule)
  {
	// On test les algo qu'on récupère mais pour l'instant que 
	// un algo  
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
Vec2d SimpleBacterium::getSpeedVector (int const& valeur) const
{
  return valeur * m_direction;
}

//METHODES OVERRIDE
void SimpleBacterium::drawOn (sf::RenderTarget& target) const
{
  // on fait comme une bacterie normal mais on ajoute le flagelle
  Bacterium::drawOn (target);
  auto set_of_points = sf::VertexArray (sf::LinesStrip); 
  set_of_points.append ({{0,0}, m_color.getColor()});
  
  for (int i(1); i <= 30; ++i)
  {
	float x (static_cast<float> (-i * m_radius / 10.0));
	float y (static_cast<float> (m_radius * sin (m_tFlagelle) * sin (2 * i / 10.0)));
    set_of_points.append ({{x,y}, m_color.getColor()});
  } 
  target.draw(set_of_points);
  
  // déclare une matrice de transformation
  auto transform = sf::Transform(); 
  transform.translate (m_position);
  transform.rotate (m_angleRotation / DEG_TO_RAD);
 
  // dessin de l'ensemble des points fait après leur transformation
  // selon la matrice transform
  target.draw (set_of_points, transform); 
}


//VIRTUAL PUR OVERRIDE
//la fonction prend 2 arguments meme si c'est des arguments de classe
//car elle doit overrider la fonction de  DiffEqFunction
Vec2d SimpleBacterium::f(Vec2d position, Vec2d speed) const
{
  Vec2d force (0,0);
  return force;
}

Bacterium* SimpleBacterium::copie () const
{
  // ne pas mettre new (SimpleBacterium (*this)) sinon n'increment pas
  // le compteur
  Bacterium* simpleBacterie (new SimpleBacterium (*this, 1));
  return simpleBacterie;
}

Quantity SimpleBacterium::getConsoEnergy() const
{
  return getConfig()["energy"][ "consumption factor"].toDouble(); 
}

void SimpleBacterium::move (sf::Time dt)
{
  //on stock l'ancienne position pour le basculemnt avant de la changer
  Vec2d anciennePosition (m_position);
  double speedValeur ((getProperty ("speed")).get());
  Vec2d speed (getSpeedVector (speedValeur));
  // la fonction f sera appelé par stepDiffEq c'est pour cela qu'on 
  // doit donner *this, notez qu'on utilise jamais la vitesse 
  // mais stepDiffeq oui
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
  
  // on doit ajouter le temps multiplié par un facteur de 3 a chaque
  // déplacement de la bactérie pour faire une belle flagelle
  m_tFlagelle += 3 * dt.asSeconds();
  
  //on met l'angle à jour à chaque mouvement pour que le flagelle soit 
  // a l'arriere
  updateAngle (dt);
  
  //BASCULEMENT
  //on incremente a chaque fois le compteur pour le mettre à jour
  m_tempsBasculement += dt;
  
  //on met a jour la probabilite de basculement apres le move 
  //car il faut connaitre la nouvelle et ancienne position
  updateProbabiliteBasculement (anciennePosition);
  basculement();
} 

j::Value& SimpleBacterium::getConfig()const
{
  return getAppConfig()["simple bacterium"];
}

//SURCHARGE DE METHODE VIRTUELLE PURES OVERRIDE
Quantity SimpleBacterium::eatableQuantity (NutrimentA& nutriment)
{
  return nutriment.eatableBy (*this); 
}

Quantity SimpleBacterium::eatableQuantity (NutrimentB& nutriment)
{
  return nutriment.eatableBy (*this); 
}

//STATIC
unsigned int SimpleBacterium::getSimpleCompteur() 
{
  return simpleBacteriumCompteur;
}

void SimpleBacterium::resetSimpleCompteur()
{
  simpleBacteriumCompteur = 0; 
}

//DESTRUCTEUR
SimpleBacterium::~SimpleBacterium ()
{
  //décremente le compteur de 1 à chaque appel du constructeur
  --simpleBacteriumCompteur;
}
  




























  



