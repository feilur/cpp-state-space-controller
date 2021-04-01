#include "ssController.h"

// Constructeur par defaut
ssController::ssController() : m_A(1,1,0), m_B(1,1,0), m_C(1,1,0), m_D(1,1,1), m_t_s(1), m_x_i(m_A.get_rows(),1,0)
{
}

// Constructeur
ssController::ssController(QSMatrix<double> A, QSMatrix<double> B, QSMatrix<double> C, QSMatrix<double> D, double t_s):
m_A(A), m_B(B), m_C(C), m_D(D), m_t_s(t_s), m_x_i(m_A.get_rows(),1,0)
{
    /*
     * Init a controller represented by a state-space model
     * 
     * A, B, C, D : matrices of the state-space representation of the controller
     * t_s : time step of the controller
     */
}

// Constructeur de copie
ssController::ssController(ssController const& autre): 
m_A(autre.m_A), m_B(autre.m_B), m_C(autre.m_C), m_D(autre.m_D), m_t_s(autre.m_t_s), m_x_i(m_A.get_rows(),1,0)
{
}
// Exemple de methode statique
void ssController::help() // ne pas remettre "static"
{
	std::cout << "Help of the ssController class." << std::endl << std::endl 
	<< "This class aims to init a discrete controller represented by its state-space representation then to compute the controller output at each iteration."
    << std::endl << std::endl;
}

// Exemple d'accesseurs :
QSMatrix<double> ssController::getA() const
{
	return m_A;
}
void ssController::setA(QSMatrix<double> A)
{
	m_A = A;
}

QSMatrix<double> ssController::getB() const
{
	return m_B;
}
void ssController::setB(QSMatrix<double> B)
{
	m_B = B;
}

QSMatrix<double> ssController::getC() const
{
	return m_C;
}
void ssController::setC(QSMatrix<double> C)
{
	m_C = C;
}

QSMatrix<double> ssController::getD() const
{
	return m_D;
}
void ssController::setD(QSMatrix<double> D)
{
	m_D = D;
}

QSMatrix<double> ssController::getX_i() const
{
    return m_x_i;
}

double ssController::getT_s() const
{
    return m_t_s;
}

void ssController::setT_s(double t_s)
{
    m_t_s = t_s;
}

// Exemple de methode constante :
void ssController::printSS() const
{
	// Print ssController matrices
    std::cout << "State-space representation of the controller (time step: " << m_t_s << ")" 
    << std::endl;
    
    std::cout << "A = " << std::endl;
    m_A.print();
    std::cout << std::endl;
    
    std::cout << "B = " << std::endl;
    m_B.print();
    std::cout << std::endl;
    
    std::cout << "C = " << std::endl;
    m_C.print();
    std::cout << std::endl;
    
    std::cout << "D = " << std::endl;
    m_D.print();
    std::cout << std::endl << std::endl;
}

void ssController::printState() const
{
    // To do
}

QSMatrix<double> ssController::currentOutput(const QSMatrix<double>& e_i)
{
    /*
     * Computes the current controller output (plant input) u_i 
     * 
     * e_i : system error between global system output y_i and reference r_i
     */
    QSMatrix<double> u_i = m_C * m_x_i + m_D * e_i;
    
    ssController::nextState(e_i);
    
    return u_i;
}

QSMatrix<double> ssController::currentOutput(const QSMatrix<double>& r_i, const QSMatrix<double>& y_i)
{
    /*
     * Computes the current controller output (plant input) u_i
     * 
     * r_i : reference signal
     * y_i : global system output
     */
    
    QSMatrix<double> e_i = ssController::currentError(r_i,y_i);
    
    QSMatrix<double> u_i = m_C * m_x_i + m_D * e_i;
    
    ssController::nextState(e_i);
    
    return u_i;
}

void ssController::nextState(const QSMatrix<double>& e_i)
{
    /*
     * Computes the next controller state x_i
     */
    m_x_i = m_A * m_x_i + m_B * e_i;
}

void ssController::reset()
{
    /*
     * Reset states x_i (to zero)
     */
    QSMatrix<double> zero(m_A.get_rows(),1,0);
    m_x_i = zero;
}

QSMatrix<double> ssController::currentError(const QSMatrix<double>& r_i, const QSMatrix<double>& y_i) const
{
    /*
     * Computes the current error between global system output y_i
     * and reference signal r_i
     */
    QSMatrix<double> e_i = r_i - y_i;
    
    return e_i;
}
