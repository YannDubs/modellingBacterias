#include "MutableNumber.hpp"

//CONSTRUCTEURS

MutableNumber::MutableNumber(double const& valeur, 
							 double const& probaMutation, 
						     double const& ecartType,
							 bool const& borneInf,
							 double const& inf, 
							 bool const& borneSup, double const& sup)
			   : m_probaMutation (probaMutation)
		       , m_borneSup (borneSup)
		       , m_borneInf (borneInf)
		       , m_sup (sup)
		       , m_inf (inf)
		       , m_ecartType (ecartType)
{	
  set(valeur);
}
	
MutableNumber::MutableNumber (j::Value const& config)		
		      : m_probaMutation(config["rate"].toDouble())
		      , m_borneSup(config["clamp max"].toBool())
		      , m_borneInf(config["clamp min"].toBool())
		      , m_sup(config["max"].toDouble())
		      , m_inf(config["min"].toDouble())
		      , m_ecartType(config["sigma"].toDouble())
{
  set(config["initial"].toDouble());
}
	
//METHODES
double MutableNumber::get() const
{
  return m_valeur;
}
	
void MutableNumber::set (double const& valeur)
{
  //on affecte que la valeur si elle est comprise entre les bornes, on ne teste seulement les valeur ou cela ne fonctionne pas 
  if(m_borneSup && (valeur >= m_sup))
  { 
    m_valeur = m_sup;
  }	
  else if (m_borneInf && (valeur <= m_inf))
  {
    m_valeur=m_inf;
  }
  //else est nécessaire car on est à la même hauteur que le if initial car ici pas de return
  else m_valeur = valeur; 
} 

void MutableNumber::mutate()
{
  //retourne vrai si avec une probabilité de m_probamutation
  if (bernoulli (m_probaMutation))
  { 
	set (m_valeur += normal (0, ((m_ecartType) * (m_ecartType))));
  }
}
	
//METHODES STATIQUES
//résolUtion de porté est nécessaire, mais les statiques ne sont pas a remettre dans le code
MutableNumber MutableNumber::probability (double const& valeur, 
										  double const& probaMutation, 
										  double const& ecartType)
{ 
		return MutableNumber (valeur, probaMutation, ecartType,true,0.0,
							  true,1.0);
}

MutableNumber probability (j::Value const& config)
{
  return MutableNumber(config["initial"].toDouble(), 
					   config["rate"].toDouble(), 
					   config["sigma"].toDouble(),
					   true, 0, true, 1);
}

MutableNumber MutableNumber::positive(double const& valeur, 
									  double const& probaMutation,
									  double const& ecartType, 
									  bool const& borneSup, 
									  double const& sup)
{
  return MutableNumber(valeur, probaMutation, ecartType,
							true, 0, borneSup, sup);
}

MutableNumber MutableNumber::positive (j::Value const& config,
									   bool const& borneSup,
									   double const& sup)
{
  return MutableNumber(config["initial"].toDouble(),
					   config["rate"].toDouble(),
					   config["sigma"].toDouble(),true,0,
					   borneSup,sup);
}
