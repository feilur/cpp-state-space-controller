#include <iostream>
#include <vector>

#include "ssController.h"
#include "QSMatrix.h"

using namespace std;

// ADD DISPLAY OF IN/OUT/STATE/TIME

int main() 
{
    ssController::help();   // Disp the help of the class
    
    double t_s = 1;   // Time step (seconds)
    
    int ne(3); // Number of controller inputs (errors)
    int nu(2); // Number of controller outputs (plant command)
    int nx(5); // Number of controller states
    
    
    // Generation of state matrices (here all matrices are null)
    QSMatrix<double> A(nx,nx,0);
    QSMatrix<double> B(nx,ne,0);
    QSMatrix<double> C(nu,nx,0);
    QSMatrix<double> D(nu,ne,1);
    
    ssController hinf(A,B,C,D,t_s);    // Init of mixed-sensitivity H infinity ssController
    
    hinf.printSS(); // Disp the state-space representation of the controller

    
    QSMatrix<double> ref(ne,1,1); // Constant reference signals
    
    QSMatrix<double> y_sys(ne,1,0); // Global system outputs (measurement)
    
    QSMatrix<double> u(nu,1,0); // Output of the controller
    
    cout << "Computation of controller outputs for 5 iterations..." << endl;
    for (int i=0;i<5;i++)
    {
        y_sys(0,0) = i; // Simulation of global system output rising on 1 channel
        
        u = hinf.currentOutput(ref, y_sys);   // Compute the output of the controller (after comparing measure and reference)
                                              // Also updates the state x of the next iteration
                                              // Possible to avoid comparing by using : sscontroller.currentOutput(error_value);
        u.print();
    }
    
    
    cout << "Reset of states vector..." << endl;
    hinf.reset();   // Reset states vector x_i to zero
    hinf.getX_i().print(); // Reset check
    
    return 0;
}

