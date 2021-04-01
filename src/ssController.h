#ifndef DEF_ssController
#define DEF_ssController

#include <iostream>
#include "QSMatrix.h"

class ssController
{
	public: // Certaines methodes pourraient etre privees
	// il y a aussi "protected" : pas accessible en dehors de la classe SAUF aux classes filles
	
	ssController(); // Constructeur par defaut
	ssController(QSMatrix<double> A, QSMatrix<double> B, QSMatrix<double> C, QSMatrix<double> D, double t_s);
	ssController(ssController const& autre); // Constructeur de copie (utilisation : ssController nouveaussssController(existant);)
    
    QSMatrix<double> currentOutput(const QSMatrix<double>& e_i);
    QSMatrix<double> currentOutput(const QSMatrix<double>& r_i, const QSMatrix<double>& y_i);
	
	// Exemple de methode statique (n'agissant pas sur un objet) :
	static void help(); // Dans le main.cpp : "ssController::help();"  --> pas besoin d'objet
	
	// Exemple d'accesseurs :
	QSMatrix<double> getA() const;
	void setA(QSMatrix<double> A);
    QSMatrix<double> getB() const;
	void setB(QSMatrix<double> B);
    QSMatrix<double> getC() const;
	void setC(QSMatrix<double> C);
    QSMatrix<double> getD() const;
	void setD(QSMatrix<double> D);
    
    double getT_s() const;
	void setT_s(double t_s);
    
    QSMatrix<double> getX_i() const;
	
	
	// Exemple de methode constante :
	void printSS() const;
    void printState() const;
    
    void reset(); // reset states (to zero)
	
	private: // Les attributs sont toujours prives (ou "protected" si il y a des classes filles)
        
    void nextState(const QSMatrix<double>& e_i);  // gives x_ip1 (x i+1)
    QSMatrix<double> currentError(const QSMatrix<double>& r_i, const QSMatrix<double>& y_i) const;
        
	QSMatrix<double> m_A;
	QSMatrix<double> m_B;
    QSMatrix<double> m_C;
	QSMatrix<double> m_D;
    double m_t_s;    // Time step of the controller
	
    QSMatrix<double> m_x_i;    // Current state
    
	//static double fps; // Attribut statique (appartenant a la classe mais non a un objet) ; accessible depuis l'exterieur meme en private : "ssController::fps".
};

#endif
