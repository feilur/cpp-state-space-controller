/**
 * @file stateSpaceController.cpp
 * @brief StateSpaceController class source file.
 * @author Alexis Proux
 * @date 1 July 2021
 ******/

#ifndef STATESPACECONTROLLER_CPP
#define STATESPACECONTROLLER_CPP

#include "stateSpaceController.h"

// Constructeur par defaut
/**
 * @brief Default constructor.
 * @details Construct a StateSpaceController object with basic matrices.
 ******/
template<typename T>
StateSpaceController<T>::StateSpaceController() : m_A(1,1,0), m_B(1,1,0), m_C(1,1,0), m_D(1,1,1), m_t_s(1),m_i(0),m_t(0),m_nx(m_A.get_rows()),m_ne(m_B.get_cols()),m_nu(m_C.get_rows()), m_x_i(m_nx,0), m_x_ib(m_nx,0),m_r_i(m_ne,0),m_y_i(m_ne,0),m_e_i(m_ne,0),m_u_i(m_nu,0)
{
}

// Constructeur
/**
 * @brief Constructor using user-generated QSMatrix matrices.
 * @param A Controller A matrix in state space representation.
 * @param B Controller B matrix in state space representation.
 * @param C Controller C matrix in state space representation.
 * @param D Controller D matrix in state space representation.
 * @param t_s Controller time step (seconds).
 ******/
template<typename T>
StateSpaceController<T>::StateSpaceController(QSMatrix<T> A, QSMatrix<T> B, QSMatrix<T> C, QSMatrix<T> D, const float t_s):
m_A(A), m_B(B), m_C(C), m_D(D), m_t_s(t_s), m_i(0),m_t(0),m_nx(m_A.get_rows()),m_ne(m_B.get_cols()),m_nu(m_C.get_rows()), m_x_i(m_nx,0), m_x_ib(m_nx,0),m_r_i(m_ne,0),m_y_i(m_ne,0),m_e_i(m_ne,0),m_u_i(m_nu,0)
{
    /*
     * Init a controller represented by a state-space model
     * 
     * A, B, C, D : matrices of the state-space representation of the controller
     * t_s : time step of the controller
     */
}

/**
 * @brief Constructor using generated controller data file.
 * @details The file has to be formatted as follows:
     * 
     * -------FILE_BEGIN-------
     * Time step value (seconds) (example: 0.1)
     * State vector dimension nx
     * Error vector dimension ne (example: 2)
     * Controller output vector dimension nu
     * A[0,0] value (example: 12.354)
     * ...
     * A[0,nx]
     * A[1,0]
     * ...
     * A[1,nx]
     * ...
     * A[nx,nx]
     * B[0,0] value (example: 12.354)
     * ...
     * B[0,ne]
     * B[1,0]
     * ...
     * B[1,ne]
     * ...
     * B[nx,ne]
     * C[0,0] value (example: 12.354)
     * ...
     * C[0,nx]
     * C[1,0]
     * ...
     * C[1,nx]
     * ...
     * C[nu,nx]
     * D[0,0] value (example: 12.354)
     * ...
     * D[0,ne]
     * D[1,0]
     * ...
     * D[1,ne]
     * ...
     * D[nu,ne]
     * -------FILE_END-------
 * @param formattedDataFilePath Path of the controller data file.
 ******/
template<typename T>
StateSpaceController<T>::StateSpaceController(std::string formattedDataFilePath):m_i(0),m_t(0)
{
    m_i = 0;
    m_t = 0;
    
    loadControllerData(formattedDataFilePath);
}

// Constructeur de copie
/**
 * @brief Copy constructor.
 * @param other Another StateSpaceController object.
 ******/
template<typename T>
StateSpaceController<T>::StateSpaceController(StateSpaceController<T> const& other):
m_A(other.m_A), m_B(other.m_B), m_C(other.m_C), m_D(other.m_D),m_i(other.m_i),m_t(other.m_t), m_t_s(other.m_t_s),m_nx(other.m_nx),m_ne(other.m_ne),m_nu(other.m_nu), m_x_i(other.m_x_i), m_x_ib(other.m_x_ib),m_r_i(other.m_r_i),m_y_i(other.m_y_i),m_e_i(other.m_e_i),m_u_i(other.m_u_i)
{
}

/**
 * @brief = operator.
 * @param controller Another StateSpaceController object.
 ******/
template<typename T>
StateSpaceController<T>& StateSpaceController<T>::operator=(const StateSpaceController<T>& controller)
{
    // Here there is no reset but it could be necessary sometimes
    
    m_i = controller.m_i;
    m_t = controller.m_t;
    
    m_t_s = controller.m_t_s;
    
    m_A = controller.m_A;
    m_B = controller.m_B;
    m_C = controller.m_C;
    m_D = controller.m_D;
    
    m_nx = controller.m_nx;
    m_ne = controller.m_ne;
    m_nu = controller.m_nu;
    
    m_x_i = controller.m_x_i;
    m_x_ib = controller.m_x_ib;

    m_r_i = controller.m_r_i;
    m_y_i = controller.m_y_i;
    m_e_i = controller.m_e_i;
        
    m_u_i = controller.m_u_i;
    
    return *this;
}

// (Virtual) Destructor
/**
 * @brief Destructor.
 ******/
template<typename T>
StateSpaceController<T>::~StateSpaceController() {}

/**
 * @brief Modifies the controller from a controller data file.
 * @details The file has to be formatted as follows:
     * 
     * -------FILE_BEGIN-------
     * Time step value (seconds) (example: 0.1)
     * State vector dimension nx
     * Error vector dimension ne (example: 2)
     * Controller output vector dimension nu
     * A[0,0] value (example: 12.354)
     * ...
     * A[0,nx]
     * A[1,0]
     * ...
     * A[1,nx]
     * ...
     * A[nx,nx]
     * B[0,0] value (example: 12.354)
     * ...
     * B[0,ne]
     * B[1,0]
     * ...
     * B[1,ne]
     * ...
     * B[nx,ne]
     * C[0,0] value (example: 12.354)
     * ...
     * C[0,nx]
     * C[1,0]
     * ...
     * C[1,nx]
     * ...
     * C[nu,nx]
     * D[0,0] value (example: 12.354)
     * ...
     * D[0,ne]
     * D[1,0]
     * ...
     * D[1,ne]
     * ...
     * D[nu,ne]
     * -------FILE_END-------
 * @param formattedDataFilePath Path of the controller data file.
 ******/
template<typename T>
void StateSpaceController<T>::loadControllerData(std::string formattedDataFilePath)
{    
    //m_i = 0;
    //m_t = 0;
    
    std::ifstream readStream;
    readStream.open(formattedDataFilePath.c_str());
    
    if (readStream)
    {
        std::string line;
        
        std::getline(readStream, line);
        m_t_s = std::stof(line);
        std::getline(readStream, line);
        m_nx = std::stoi(line);
        std::getline(readStream, line);
        m_ne = std::stoi(line);
        std::getline(readStream, line);
        m_nu = std::stoi(line);
        
        std::vector<T> x_i(m_nx,0);
        
        m_x_i = x_i;
        m_x_ib = x_i;
        
        std::vector<T> e_i(m_ne,0);
        
        m_r_i = e_i;
        m_y_i = e_i;
        m_e_i = e_i;
        
        std::vector<T> u_i(m_nu,0);
        
        m_u_i = u_i;


        QSMatrix<T> A(m_nx,m_nx,0);
        QSMatrix<T> B(m_nx,m_ne,0);
        QSMatrix<T> C(m_nu,m_nx,0);
        QSMatrix<T> D(m_nu,m_ne,0);
        m_A = A;
        m_B = B;
        m_C = C;
        m_D = D;
        
        
        for (int row=0;row<m_nx;row++)
        {
            for (int col=0;col<m_nx;col++)
            {
                std::getline(readStream, line);
                m_A(row,col) = std::stod(line);
            }
        }
        for (int row=0;row<m_nx;row++)
        {
            for (int col=0;col<m_ne;col++)
            {
                std::getline(readStream, line);
                m_B(row,col) = std::stod(line);
            }
        }
        for (int row=0;row<m_nu;row++)
        {
            for (int col=0;col<m_nx;col++)
            {
                std::getline(readStream, line);
                m_C(row,col) = std::stod(line);
            }
        }
        for (int row=0;row<m_nu;row++)
        {
            for (int col=0;col<m_ne;col++)
            {
                std::getline(readStream, line);
                m_D(row,col) = std::stod(line);
            }
        }
    }
    else
    {
        std::cout << "\033[1;31mERROR: Unable to open state space controller data file.\033[0m" << std::endl << std::endl;
    }
    
    readStream.close();
}

/**
 * @brief Display the StateSpaceController class help.
 ******/
template<typename T>
void StateSpaceController<T>::help() // ne pas remettre "static"
{
	std::cout << "Help of the StateSpaceController class." << std::endl << std::endl 
	<< "This class aims to init a discrete controller represented by its state-space representation then to compute the controller output at each iteration."
    << std::endl << std::endl;
    std::cout << "Constructor from file.dat : useful to import data from controller synthesis" << std::endl <<
     "formattedDataFilePath : Path to the file" << std::endl << std::endl <<
     
     "The file has to be formatted as follows:"<< std::endl << std::endl <<
     
     "-------FILE_BEGIN-------" << std::endl <<
     "Time step value (seconds) (example : 0.1)"<< std::endl <<
     "State vector dimension nx"<< std::endl <<
     "Error vector dimension ne (example : 2)"<< std::endl <<
     "Controller output vector dimension nu"<< std::endl <<
     "A[0,0] value (example : 12.354)"<< std::endl <<
     "..."<< std::endl <<
     "A[0,nx]"<< std::endl <<
     "A[1,0]"<< std::endl <<
     "..."<< std::endl <<
     "A[1,nx]"<< std::endl <<
     "..."<< std::endl <<
     "A[nx,nx]"<< std::endl <<
     "B[0,0] value (example : 12.354)"<< std::endl <<
     "..."<< std::endl <<
     "B[0,ne]"<< std::endl <<
     "B[1,0]"<< std::endl <<
     "..."<< std::endl <<
     "B[1,ne]"<< std::endl <<
     "..."<< std::endl <<
     "B[nx,ne]"<< std::endl <<
     "C[0,0] value (example : 12.354)"<< std::endl <<
     "..."<< std::endl <<
     "C[0,nx]"<< std::endl <<
     "C[1,0]"<< std::endl <<
     "..."<< std::endl <<
     "C[1,nx]"<< std::endl <<
     "..."<< std::endl <<
     "C[nu,nx]"<< std::endl <<
     "D[0,0] value (example : 12.354)"<< std::endl <<
     "..."<< std::endl <<
     "D[0,ne]"<< std::endl <<
     "D[1,0]"<< std::endl <<
     "..."<< std::endl <<
     "D[1,ne]"<< std::endl <<
     "..."<< std::endl <<
     "D[nu,ne]"<< std::endl <<
     "-------FILE_END-------"<< std::endl << std::endl;
}

/**
 * @return Controller A matrix in state space representation.
 ******/
template<typename T>
QSMatrix<T> StateSpaceController<T>::getA() const
{
	return m_A;
}

/**
 * @param A Controller A matrix in state space representation.
 ******/
template<typename T>
void StateSpaceController<T>::setA(QSMatrix<T> A)
{
	m_A = A;
}

/**
 * @return Controller B matrix in state space representation.
 ******/
template<typename T>
QSMatrix<T> StateSpaceController<T>::getB() const
{
	return m_B;
}

/**
 * @param B Controller B matrix in state space representation.
 ******/
template<typename T>
void StateSpaceController<T>::setB(QSMatrix<T> B)
{
	m_B = B;
}

/**
 * @return Controller C matrix in state space representation.
 ******/
template<typename T>
QSMatrix<T> StateSpaceController<T>::getC() const
{
	return m_C;
}

/**
 * @param C Controller C matrix in state space representation.
 ******/
template<typename T>
void StateSpaceController<T>::setC(QSMatrix<T> C)
{
	m_C = C;
}

/**
 * @return Controller D matrix in state space representation.
 ******/
template<typename T>
QSMatrix<T> StateSpaceController<T>::getD() const
{
	return m_D;
}

/**
 * @param D Controller D matrix in state space representation.
 ******/
template<typename T>
void StateSpaceController<T>::setD(QSMatrix<T> D)
{
	m_D = D;
}

/**
 * @return Current state vector.
 ******/
template<typename T>
std::vector<T> StateSpaceController<T>::getX_i() const
{
    return m_x_i;
}

/**
 * @return Controller time step (seconds).
 ******/
template<typename T>
float StateSpaceController<T>::getTimeStep() const
{
    return m_t_s;
}

/**
 * @param t_s Controller time step (seconds).
 ******/
template<typename T>
void StateSpaceController<T>::setTimeStep(const float t_s)
{
    m_t_s = t_s;
}

/**
 * @return Current time (seconds).
 ******/
template<typename T>
float StateSpaceController<T>::getTime() const
{
    return m_t;
}

/**
 * @return State vector dimension.
 ******/
template<typename T>
unsigned int StateSpaceController<T>::getNx() const
{
    return m_nx;
}

/**
 * @return Error vector dimension.
 ******/
template<typename T>
unsigned int StateSpaceController<T>::getNe() const
{
    return m_ne;
}

/**
 * @return Controller output vector dimension.
 ******/
template<typename T>
unsigned int StateSpaceController<T>::getNu() const
{
    return m_nu;
}

/**
 * @brief Prints the state space representation of the StateSpaceController object.
 * @details Prints the controller time step and all state matrices. 
 ******/
template<typename T>
void StateSpaceController<T>::printStateSpace() const
{
	// Print StateSpaceController matrices
    std::cout << "State-space representation of the controller (time step: " << m_t_s << " s)" 
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
    std::cout << std::endl;
}

/**
 * @brief Gives the state space representation of the StateSpaceController object.
 * @return String that contains the controller time step and all state matrices. 
 ******/
template<typename T>
std::string StateSpaceController<T>::getStateSpace() const
{
    std::stringstream buffer;
    
    buffer << "ACCURACY OF THESE VALUES IS NOT THE ACTUAL ONE." << std::endl << std::endl;
    
    buffer << "Time step = " << m_t_s << " s" << std::endl << std::endl;
    
    buffer << "A = " << std::endl;
    buffer << m_A.getRepresentation();
    buffer << std::endl;
    
    buffer << "B = " << std::endl;
    buffer << m_B.getRepresentation();
    buffer << std::endl;
    
    buffer << "C = " << std::endl;
    buffer << m_C.getRepresentation();
    buffer << std::endl;
    
    buffer << "D = " << std::endl;
    buffer << m_D.getRepresentation();
    buffer << std::endl;
    
    return buffer.str();
}

/**
 * @brief Prints the current state of the StateSpaceController object.
 * @details Prints the current time (seconds), the reference, plant output, error, controller output and state vector of the controller.
 * 
 * Prints a header if this function is called when the current indice is equal to 1.
 ******/
template<typename T>
void StateSpaceController<T>::printState() const
{
    // To do
    // t r1..rn y1..yn e1..en u1..un x1..xn
    // OR
    // t e1..en u1..un x1..xn (in case we have just the error)
    
    if(m_i == 1)
    {
        // Print the header
        std::cout << "t(s)";
        for(int k=0;k<m_ne;k++)
        {
            std::cout << " ; r" << k;
        }
        for(int k=0;k<m_ne;k++)
        {
            std::cout << " ; y" << k;
        }
        for(int k=0;k<m_ne;k++)
        {
            std::cout << " ; e" << k;
        }
        for(int k=0;k<m_nu;k++)
        {
            std::cout << " ; u" << k;
        }
        for(int k=0;k<m_nx;k++)
        {
            std::cout << " ; x" << k;
        }
        std::cout << std::endl;
    }
    // Print values
        std::cout << m_t;
        for(int k=0;k<m_ne;k++)
        {
            std::cout << " ; " << m_r_i[k];
        }
        for(int k=0;k<m_ne;k++)
        {
            std::cout << " ; " << m_y_i[k];
        }
        for(int k=0;k<m_ne;k++)
        {
            std::cout << " ; " << m_e_i[k];
        }
        for(int k=0;k<m_nu;k++)
        {
            std::cout << " ; " << m_u_i[k];
        }
        for(int k=0;k<m_nx;k++)
        {
            std::cout << " ; " << m_x_i[k];
        }
        std::cout << std::endl;
}

/**
 * @brief Computes the current controller output with the error vector and increments time.
 * @param e_i Current error vector.
 * @return Controller output vector.
 ******/
template<typename T>
std::vector<T> StateSpaceController<T>::currentOutput(const std::vector<T>& e_i)
{
    /*
     * Computes the current controller output (plant input) u_i 
     * 
     * e_i : system error between global system output y_i and reference r_i
     */
    m_e_i = e_i;    // Update error signal
    
    // Update controller output
    // m_u_i = m_C * m_x_i + m_D * m_e_i;
    m_u_i = QSMatrix<T>::vectorAdd(m_C * m_x_i, m_D * m_e_i);
    
    nextState();  // Update next iteration state signal
    
    return m_u_i;
}

/**
 * @brief Computes the current controller output with the error vector, applies a saturation and increments time.
 * @details Saturation vectors are used to tune each controller output. 
 * @param e_i Current error vector.
 * @param u_min Bottom saturation vector.
 * @param u_max Top saturation vector.
 * @return Controller output vector.
 ******/
template<typename T>
std::vector<T> StateSpaceController<T>::currentOutput(const std::vector<T>& e_i, const std::vector<T>& u_min, const std::vector<T>& u_max)
{
    /*
     * Computes the current controller output (plant input) u_i 
     * 
     * e_i : system error between global system output y_i and reference r_i
     */
    currentOutput(e_i);
    
    saturation(u_min, u_max); // Limit the controller output
    
    return m_u_i;
}

/**
 * @brief Computes the current controller output with the error vector, applies a saturation and increments time.
 * @details Unique saturation values are used to tune all controller outputs at once.
 * @param e_i Current error vector.
 * @param u_min Bottom saturation value.
 * @param u_max Top saturation value.
 * @return Controller output vector.
 ******/
template<typename T>
std::vector<T> StateSpaceController<T>::currentOutput(const std::vector<T>& e_i, const T& u_min, const T& u_max)
{
    /*
     * Computes the current controller output (plant input) u_i 
     * 
     * e_i : system error between global system output y_i and reference r_i
     */
    currentOutput(e_i);
    
    saturation(u_min, u_max); // Limit the controller output
    
    return m_u_i;
}

/**
 * @brief Computes the current controller output with the reference vector and the plant output vector  and increments time.
 * @param r_i Current reference vector.
 * @param y_i Current plant output vector.
 * @return Controller output vector.
 ******/
template<typename T>
std::vector<T> StateSpaceController<T>::currentOutput(const std::vector<T>& r_i, const std::vector<T>& y_i)
{
    /*
     * Computes the current controller output (plant input) u_i
     * 
     * r_i : reference signal
     * y_i : global system output
     */
    
    m_r_i = r_i;    // Update reference signal
    m_y_i = y_i;    // Update output signal (measured)
    
    currentError();   // Update error signal
    
    currentOutput(m_e_i);
    
    return m_u_i;
}

/**
 * @brief Computes the current controller output with the reference vector and the plant output vector, applies a saturation and increments time.
 * @details Saturation vectors are used to tune each controller output. 
 * @param r_i Current reference vector.
 * @param y_i Current plant output vector.
 * @param u_min Bottom saturation vector.
 * @param u_max Top saturation vector.
 * @return Controller output vector.
 ******/
template<typename T>
std::vector<T> StateSpaceController<T>::currentOutput(const std::vector<T>& r_i, const std::vector<T>& y_i, const std::vector<T>& u_min, const std::vector<T>& u_max)
{
    /*
     * Computes the current controller output (plant input) u_i
     * 
     * r_i : reference signal
     * y_i : global system output
     */
    
    m_r_i = r_i;    // Update reference signal
    m_y_i = y_i;    // Update output signal (measured)
    
    currentError();   // Update error signal
    
    currentOutput(m_e_i);
    
    saturation(u_min, u_max); // Limit the controller output
    
    return m_u_i;
}

/**
 * @brief Computes the current controller output with the reference vector and the plant output vector, applies a saturation and increments time.
 * @details Unique saturation values are used to tune all controller outputs at once.
 * @param r_i Current reference vector.
 * @param y_i Current plant output vector.
 * @param u_min Bottom saturation value.
 * @param u_max Top saturation value.
 * @return Controller output vector.
 ******/
template<typename T>
std::vector<T> StateSpaceController<T>::currentOutput(const std::vector<T>& r_i, const std::vector<T>& y_i, const T& u_min, const T& u_max)
{
    /*
     * Computes the current controller output (plant input) u_i
     * 
     * r_i : reference signal
     * y_i : global system output
     */
    
    m_r_i = r_i;    // Update reference signal
    m_y_i = y_i;    // Update output signal (measured)
    
    currentError();   // Update error signal
    
    currentOutput(m_e_i);
    
    saturation(u_min, u_max); // Limit the controller output
    
    return m_u_i;
}

/**
 * @brief Applies a saturation on the controller output vector.
 * @details Saturation vectors are used to tune each controller output.
 * @param u_min Bottom saturation vector.
 * @param u_max Top saturation vector.
 ******/
template<typename T>
void StateSpaceController<T>::saturation(const std::vector<T>& u_min, const std::vector<T>& u_max)
{
    for (int k=0;k<m_nu;k++)
    {
        if (m_u_i[k] > u_max[k])
        {
            m_u_i[k] = u_max[k];
        }
        else if (m_u_i[k] < u_min[k])
        {
            m_u_i[k] = u_min[k];
        }
    }
}

/**
 * @brief Applies a saturation on the controller output vector.
 * @details Unique saturation values are used to tune all controller outputs at once.
 * @param u_min Bottom saturation value.
 * @param u_max Top saturation value.
 ******/
template<typename T>
void StateSpaceController<T>::saturation(const T& u_min, const T& u_max)
{
    for (int k=0;k<m_nu;k++)
    {
        if (m_u_i[k] > u_max)
        {
            m_u_i[k] = u_max;
        }
        else if (m_u_i[k] < u_min)
        {
            m_u_i[k] = u_min;
        }
    }
}

/**
 * @brief Computes the next state vector value and increments time.
 ******/
template<typename T>
void StateSpaceController<T>::nextState()
{
    /*
     * Computes the next controller state x_i
     */
    m_x_ib = m_x_i; // Backup of the last state for display
    
    //m_x_i = m_A * m_x_i + m_B * m_e_i;
    m_x_i = QSMatrix<T>::vectorAdd(m_A * m_x_i, m_B * m_e_i);
    
    m_t = m_i * m_t_s;
    m_i++;
}

/**
 * @brief Resets time and state vector.
 ******/
template<typename T>
void StateSpaceController<T>::reset()
{
    /*
     * Reset states x_i (to zero)
     */
    std::vector<T> zero(m_nx,0);
    m_x_i = zero;
    
    m_i = 0;
    m_t = 0;
}

/**
 * @brief Computes the current error vector from reference vector and plant output vector.
 ******/
template<typename T>
void StateSpaceController<T>::currentError()
{
    /*
     * Computes the current error between global system output y_i
     * and reference signal r_i
     */
    //m_e_i = m_r_i - m_y_i;
    m_e_i = QSMatrix<T>::vectorSubstract(m_r_i, m_y_i);
}

#endif
