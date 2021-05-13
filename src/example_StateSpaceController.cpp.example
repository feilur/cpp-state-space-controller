#include "stateSpaceController.h"
#include "QSMatrix.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() 
{
    StateSpaceController::help();   // Disp the help of the class
    
    // Use QSMatrix class only if you don't want to create a controller from .dat file
    
    const double t_s = 5;   // Time step (seconds)
    
    const int ne(2); // Number of controller inputs (errors)
    const int nu(3); // Number of controller outputs (plant command)
    const int nx(9); // Number of controller states
    
    
    // Generation of state matrices (here all matrices are null except D)
    // Only if we don't want to create the controller from .dat file
    
    QSMatrix<double> A(nx,nx,0);
    QSMatrix<double> B(nx,ne,0);
    QSMatrix<double> C(nu,nx,0);
    QSMatrix<double> D(nu,ne,1);
    
    
    StateSpaceController K(A,B,C,D,t_s);    // Init of mixed-sensitivity H infinity StateSpaceController
    
    // OR controller generation from formatted data file (see StateSpaceController.cpp for more details):
    /*
    StateSpaceController K("resources/controller.dat");
    const unsigned int ne = K.getNe();  // Number of controller inputs
    const unsigned int nu = K.getNu();  // Number of controller outputs
    */
    K.printSS(); // Disp the state-space representation of the controller
    
    
    vector<double> ref(ne,1); // Constant reference signals
    
    vector<double> y_sys(ne,0); // Global system outputs (measurement)
    
    vector<double> u(nu,0); // Output of the controller

    
    cout << "Computation of controller outputs for 5 iterations..." << endl;
    for (int i=0;i<5;i++)
    {
        y_sys[0] = i; // Simulation of global system output rising on 1 channel
        
        u = K.currentOutput(ref, y_sys);   // Compute the output of the controller (after comparing measure and reference)
                                              // Also updates the state x of the next iteration
                                              // Possible to avoid comparing by using : StateSpaceController.currentOutput(error_value);
        K.printState();
    }
    
    K.reset();   // Reset controller (states, time)
    
    return 0;
}
