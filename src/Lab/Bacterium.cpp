#include "Bacterium.hpp"
#include "Application.hpp"
#include "Nutriment.hpp"

#include "Random/Random.hpp"
#include "Utility/MutableNumber.hpp"
#include "Utility/Utility.hpp"

//CONSTRUCTEUR
Bacterium::Bacterium (Quantity const& energy, Vec2d const& position,
				      Vec2d const& direction, double const& rayon,
				      MutableColor const& couleur,
				      Parametres const& parametres,
				      bool const& abstinance)
  : CircularBody(position, rayon)
  , m_energy(energy)
  , m_direction(direction)
  , m_color(couleur)
  , m_parametresMutable(parametres)
  , m_abstinance(abstinance)
  , m_angleRotation (m_direction.angle())
  , m_compteur (sf::Time::Zero)
{}

//METHODES
void Bacterium::mutate() 
{
  //for auto référence car doit faire muter les parametre 
  // et pas les parametre de la copie
  for (auto& element : m_parametresMutable) 
  {
    (element.second).mutate();
  }
  m_color.mutate(); 
}

bool Bacterium::dead() const
{
  //rappel: il vaut mieux faire un test <1 que <=0
  return (m_energy < 1);
}

bool Bacterium::canEat (sf::Time dt)
{
  //on incrémente a chaque appel
  m_compteur += dt;
  if (not (m_abstinance) && m_compteur > getTemps())
  {
    m_compteur = sf::Time::Zero;
    return true;
  }
  return false;
}

void Bacterium::consumeEnergy (Quantity const& qt)
{
  setEnergy (m_energy - qt);
}

void Bacterium::addProperty (const std::string& key, MutableNumber parametre)
{
  auto paire = m_parametresMutable.find (key);  
  // si la paire existe déjà alors on doit lancer une exception
  // car on veut seulement ajouter et pas modifié
  if (paire != m_parametresMutable.end())
  {
    throw std::invalid_argument ("La clef existe déjà");
  } 
  m_parametresMutable[key] = parametre; 
}

Bacterium* Bacterium::clone()
{
  if (m_energy > getEnergyMin())
  {
	// les 2 bactéries ont moitié l'énerge donc on fait perdre
	// la "parente" avant la copie
	setEnergy (m_energy / 2);
	//on doit appeler la nouvelle fonction copie qui est redéfinit dans
	// chaque sous classe (copie polymorphique) afin que le clone
	//renvois la bonne bactérie 
	Bacterium* bacterium (copie());
	Vec2d vecteur (Vec2d::fromRandomAngle());
    //on prend une direction au hasard (retourne un vecteur unitaire de
    //direction aléatoire puis on multiplie par la rayon pour faire comme 
	//sur video (1/4 des deux bacteries sont confondus 
	bacterium->CircularBody::move (vecteur * m_radius);
	//et on lui fait inverser sa direction
    m_direction = - m_direction;
	//on fait muter la nouvelle bactérie 
	bacterium->mutate();
	return bacterium;
  }
  //si elle ne peut pas se cloner il faut renvoyer un nullptr sinon segfault
  return nullptr; 
}

void Bacterium::eat (Nutriment& nutriment)
{
  Quantity eaten (nutriment.eatableBy (*this));
  setEnergy (m_energy + eaten);
}

//SETTEURS
void Bacterium::setEnergy (Quantity const& quantite)
{
  //borne supérieur subjective de l'énergie qu'une bactérie peut avoir 
  // pour eviter d'eventuel overflow
  const int maxEnergy (1000); 
  //même si jamais nul car déjà test dans focntion l'appelant
  if (quantite < 1)
  {
    m_energy = 0;
  }
  else if (quantite > maxEnergy) 
  {
    m_energy = maxEnergy;
  }
  else 
  {
    m_energy = quantite;
  }
}

void Bacterium::setBestDirection()
{
  //on stock une variable pour connaitre le score max
  double scoreMax (0.0);
  Vec2d directionScoreMax (m_direction);
  // N c'est le nombre de direction qu'on va proposer (par ex. 20)
  // !!!!!!!!!!! améliorer ici???
  int N (20);
  
  for (int i(0); i<N; ++i)
  {
	Vec2d directionAleatoire (Vec2d::fromRandomAngle());
	double scoreAleatoire (getAppEnv().getPositionScore 
								(m_position + directionAleatoire));
	
	if (scoreAleatoire > scoreMax)
	{
	  directionScoreMax = directionAleatoire;
	  scoreMax = scoreAleatoire;
	}
  }

  m_direction = directionScoreMax;
}
//GETTEURS

Quantity Bacterium::getEnergyMin() const
{
  return getConfig()["energy"]["division"].toDouble();
}

sf::Time Bacterium::getTemps() const
{
  return sf::seconds (getConfig()["meal"]["delay"].toDouble());
}

MutableNumber Bacterium::getProperty (const std::string& key) const
{
  auto paire = m_parametresMutable.find (key);
  if (paire == m_parametresMutable.end())
  {
    throw std::out_of_range ("pas de clef trouvé");
  }
  return paire->second;
}

Quantity Bacterium::getMaxEatable() const
{
  return getConfig()["meal"]["max"].toDouble();
}
 
//METHODES OVERRIDE
//dessine nutriment et taille sur la frame
void Bacterium::drawOn (sf::RenderTarget& target) const
{
  auto const circle = buildCircle (m_position, m_radius, m_color.getColor());
  target.draw (circle);
   
  //seulemet si la case debug est cocher
  if (isDebugOn())
  {
    int affiche (m_energy);
    auto const text = buildText (std::to_string(affiche), m_position, 
								getAppFont(), 15, sf::Color::White);
    //dessine le texte voulu (montrant la m_energy de la bacterie prélévé)
    target.draw (text);
  }
}

void Bacterium::update (sf::Time dt)
{  
  // on fait bouger de maniere polymorphique la bactérie
  move (dt);
  
  //ajoute une variable pour pas appeler 2 fois la fonction
  Nutriment* nutriment (getAppEnv().getNutrimentColliding (*this));
  bool isColliding (getAppEnv().doesCollideWithDish (*this));

  //si on collide avec le bord il faut faire - m_direction
  if (isColliding)
  {
    m_direction = - m_direction;
  }
    
  //ajout de la fonction canEat (retourne vrai lorsqu'on est pas en 
  //abstinance et que le temps etait assez long depuis le derniere repas
  if (nutriment != nullptr && canEat (dt))
  {
	//on met ici l'appel a la division car on trouve plus logique 
	//qu'elle se clone seulement si elle mange et non pas seulement
	// si elle collide (comme dit dans la partie 3.4
	//on ne stock pas le bool renvoyer par addBactrieum mais on 
	//pourrait faire une gestion erreure
	getAppEnv().addBacterium (clone());
	//consommeNutriment (nutriment);
	eat (*nutriment); 
  }
}
 
//DESTRUCTEUR
//ilfaut definir un destructeur virtuel pour bien supprimer mais
//on est toujours obliger de définir le destructeur donc {}
Bacterium::~Bacterium()
{}
