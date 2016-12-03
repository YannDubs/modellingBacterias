#include "Nutriment.hpp"
#include "Application.hpp"
#include <iostream>
//???? why not in hpp
#include "Bacterium.hpp"
#include "SimpleBacterium.hpp"
#include "TwitchingBacterium.hpp"
#include "SwarmBacterium.hpp"
#include "PlasmidBacterium.hpp"
//NUTRIMENT

//CONSTRUCTEUR
Nutriment::Nutriment (Quantity const& quantite, Vec2d const& position)
  : CircularBody (position, quantite)
  , m_quantite (quantite)
{}

//DESTRUCTEUR
Nutriment::~Nutriment()
{}

//METHODES
Quantity Nutriment::takeQuantity (Quantity quantite)
//pas de reference constante car on utilise quantite comme varaible pour ne pas créer une 3 variable
{
  if (quantite > m_quantite)
  {
    quantite = m_quantite;
    setQuantity (0);
    //si on veut en prélever trop, on retourn la quantite maximale qui a pu être prélevée
    return quantite;
  }
  setQuantity (m_quantite - quantite);
  //on utilise pas de else pour ne pas avoir de warning
  return quantite;
}

void Nutriment::setQuantity (Quantity const& quantite)
{
  double quantityMax (getConfig()["quantity"]["max"].toDouble());
    
    if (quantite < 1)
    {
      m_quantite = 0;
	}
    //cette ligne peut sembler inutile vu qu'on pourrait juste faire le test pour celle
    //d'après mais sans ça les nutriments peuvent grossir parfois a plus de 2*quantity si
    //la température est très élevée et le temps passe très vite donc onfait une sorte de second test
    
	//en laissant le <= nous etions dans le cas ou les
    // nutriments grandissent toujours au dessus de 2*quantitémax même 
    //lorsque celle ci valent 100 et donc les bacteries dans les test 
    //pouvaient manger indéfiniment ! 
	else if (m_quantite > 2 * quantityMax)
	{
      m_quantite = 2 * quantityMax;
	} 
	else
    {
      m_quantite = quantite;
	} 
  updateRadius();
}

void Nutriment::drawOn (sf::RenderTarget& target) const
{
  //on charge l'image de la source de nutriments qui est dans app.json
  auto const& texture = getAppTexture (getConfig()["texture"].toString());
  auto nutrimentSprite = buildSprite (m_position, 6, texture);
  // adapte la taille du Sprite au rayon du nutriment:
  nutrimentSprite.setScale (2 * m_radius / texture.getSize().x, 
							2 * m_radius / texture.getSize().y);
  //dessine notre nutriment
  target.draw (nutrimentSprite);
   
  if (isDebugOn())
  {
    int affiche (m_quantite);
    auto const text = buildText (std::to_string (affiche), m_position, 
								 getAppFont(), 15, sf::Color::Black);
    //dessine le texte voulu montrant la m_quantité de nutriment prélévé
    target.draw (text);
  }
}
 
//met à jour la rayon du nutriment donc la taille
void Nutriment::updateRadius()
{
  m_radius = m_quantite;
}

void Nutriment::update (sf::Time dt)
{
  double minTemperature (getConfig()["growth"]["min temperature"].toDouble());
  double dishTemperature (getAppConfig()["petri dish"]["temperature"].toDouble());
  double maxTemperature (getConfig()["growth"]["max temperature"].toDouble());
    
  if ((minTemperature < dishTemperature) && (maxTemperature > dishTemperature))
  {
    //on recupère la vitesse
    auto growth = (getConfig()["growth"]["speed"].toDouble()) * dt.asSeconds();
	setQuantity (growth + m_quantite);
  }
}

bool Nutriment::isEmpty() const
{
 return m_quantite < 1; 
}

double Nutriment::positionScore (const Vec2d& position) const
{
  double distanceP (distance (position, m_position));
  double puissance (getAppConfig()
				    ["petri dish"]["gradient"]["exponent"].toDouble());
  return (m_quantite) / pow (distanceP, puissance);
}

unsigned int Nutriment::getQuantity() const
{
  return m_quantite; 	
}

//NUTRIMENT A

NutrimentA::NutrimentA (Quantity const& quantite, Vec2d const& position)
  : Nutriment (quantite, position)
  {}

j::Value const& NutrimentA::getConfig() const
{
  return getAppConfig()["nutriments"]["A"];
}

Quantity NutrimentA::eatableBy (Bacterium& bacterium)
{
  return takeQuantity (bacterium.getMaxEatable());
}
//inutile mais on garde la logistique pour d'éventuels changements 
//ex: nutriments verts ne sont plus manger de la même façon par 
//les autres bacteries
Quantity NutrimentA::eatableBy (SimpleBacterium& bacterium)
{
  return takeQuantity(bacterium.getMaxEatable());
}

Quantity NutrimentA::eatableBy (TwitchingBacterium& bacterium)
{
  return takeQuantity (bacterium.getMaxEatable());
}

Quantity NutrimentA::eatableBy (SwarmBacterium& bacterium)
{
  return takeQuantity (bacterium.getMaxEatable());
}

Quantity NutrimentA::eatableBy (PlasmidBacterium& bacterium)
{
  return takeQuantity (bacterium.getMaxEatable()); 
}

//NUTRIMENTB

NutrimentB::NutrimentB (Quantity const& quantite, Vec2d const& position)
  : Nutriment (quantite, position)
  , m_isPerimer (false)
  {
    m_tempsPeremption = sf::Time::Zero;
  }

j::Value const& NutrimentB::getConfig() const
{
  return getAppConfig()["nutriments"]["B"];
}

//On appelle ici en fait les 3méthode suivantes (pour les simple, swarm,
//et Twitching, c'est justement le but de l'êxercice même si 
//appeler cette méthode pour une bacterie (tout court) peut 
//sembler inutile puisque elle c'est pas instanciable
Quantity NutrimentB::eatableBy (Bacterium& bacterium)
{
  return bacterium.eatableQuantity (*this); 
}

Quantity NutrimentB::eatableBy (SimpleBacterium& bacterium)
{
  if (m_isPerimer)
  {
	//on a choisi 100 pour dire que c'est une quantité suffisante pour 
	//tuer notre bactérie qui s'empoissonne
	return -100;
  }
  
  double resistance (getConfig()["resistance factor"].toDouble()); 
  return takeQuantity (bacterium.getMaxEatable() / resistance);
}

Quantity NutrimentB::eatableBy (TwitchingBacterium& bacterium)
{
   if (m_isPerimer)
  {
	return -100;
  }
  
  Quantity quantite (takeQuantity (bacterium.getMaxEatable())); 
  double factorNutritif (getConfig()["nutritive factor"].toDouble()); 
  return factorNutritif * quantite; 
}

Quantity NutrimentB::eatableBy (SwarmBacterium& bacterium)
{
  if (m_isPerimer)
  {
	return -100;
  }
  
  Quantity quantite (takeQuantity (bacterium.getMaxEatable())); 
  double factorPoison (getConfig()["poison factor"].toDouble());
  return (-(factorPoison) * quantite); 
}

Quantity NutrimentB::eatableBy (PlasmidBacterium& bacterium)
{
  bool plasmid (bacterium.getResistance()[0] || bacterium.getResistance()[1]);
  if (m_isPerimer && !plasmid)
  {
	return -100;
  }
  return takeQuantity (bacterium.getMaxEatable()); 
}

//METHODES
Quantity NutrimentB::takeQuantity (Quantity quantite)
//pas de reference constante car on utilise quantite comme varaible pour ne pas créer une 3 variable
{
  //il faut remettre a jour le compteur a chaque fois que nutriment est mangé
  if (!m_isPerimer)
  {
    m_tempsPeremption = sf::Time::Zero;
  }
  return Nutriment::takeQuantity (quantite);
  
}

void NutrimentB::drawOn (sf::RenderTarget& target) const
{	
  if (!m_isPerimer)
  {
    Nutriment::drawOn (target);
  }
  if (m_isPerimer)
  {
	//on charge l'image de la source de nutriments qui est dans app.json
	auto const& texture = getAppTexture (getConfig()["texture2"].toString());
	auto nutrimentSprite = buildSprite (m_position, 6, texture);
	// adapte la taille du Sprite au rayon du nutriment:
	nutrimentSprite.setScale (2 * m_radius / texture.getSize().x, 
							  2 * m_radius / texture.getSize().y);
	//dessine notre nutriment
	target.draw (nutrimentSprite);

	if (isDebugOn())
	{
	int affiche (m_quantite);
	auto const text = buildText (std::to_string(affiche), m_position,
								 getAppFont(), 15, sf::Color::Black);
	//dessine le texte voulu montrant la m_quantité de nutriment prélévé
	target.draw (text);
	}
  }
}

void NutrimentB::update (sf::Time dt)
{
  m_tempsPeremption += dt;
  //sf::Time maxPeremption = sf::seconds(1);
  if (!m_isPerimer)
  {
	if (m_tempsPeremption > sf::seconds (4))
    {
      m_isPerimer = true;
    }
    Nutriment::update (dt);
  }
  else if(m_tempsPeremption > sf::seconds (20))
  {
    m_quantite = 0;
  }
}

bool NutrimentB::isPerimer() const
{
  return m_isPerimer; 	
}
