
#ifndef MutableNumber_HPP
#define MutableNumber_HPP

#include "Random/Random.hpp"

class MutableNumber
{
public: 
//CONSTRUCTEURS
//permet de contourner le bugg du arry car on initialiser les MutableNumber de manière minimale et on peut créer le tableau
  MutableNumber() = default;
  MutableNumber (double const& valeur, 
			     double const& probaMutation, 
			     double const& ecartType,
			     bool const& borneInf = false,
			     double const& inf = 0, 
			     bool const& borneSup = false,
			     double const& sup = 0); 

  MutableNumber (j::Value const& config);
  
  //METHODES
  double get() const;
  void set (double const& valeur);
  void mutate();

  //METHODES STATIQUES 
  //(ne peuvent pas être suivies de const cf cppreference) 
  static MutableNumber probability (double const& valeur, 
								    double const& probaMutation, 
							        double const& ecartType);
  static MutableNumber probability (j::Value const& config);
  static MutableNumber positive (double const& valeur, 
							     double const& probaMutation,
							     double const& ecartType, 
							     bool const& borneSup = false, 
							     double const& sup = 0);
  static MutableNumber positive (j::Value const& config,
							     bool const& borneSup = false,
							     double const& sup = 0);

private: 
  double m_valeur;
  double m_probaMutation;
  bool m_borneSup; 
  bool m_borneInf;
  double m_sup;
  double m_inf;
  double m_ecartType;
 
}; 

#endif
