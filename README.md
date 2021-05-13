Useful tool for state-space controllers implementation (for robust control for example).

See the "example_StateSpaceController.cpp" to see the possibilies.



General State-Space Controller class.

 See "example_StateSpaceController.cpp.example" to have an implementation example.
 
                StateSpaceController class
            ______________/\______________
           /       ___        ________    \  ________
              r  +/   \  e   |        |  u  |        |   y
            ---->|     |---->|   K    |---->| Plant  |---+-->
                  \___/      |________|     |________|   |
                    ^ -                                  |
                  y |____________________________________|
 
       K:
           | x_{i+1} = A*x_i + B*e_i
           |     u_i = C*x_i + D*e_i
 
               with e_i = r_i - y_i
 


      Constructor from file.dat : useful to import data from controller synthesis
     
      formattedDataFilePath : Path to the file
     
      The file has to be formatted as follows:
     
      -------FILE_BEGIN-------
      Time step value (seconds) (example : 0.1)
      State vector dimension nx
      Error vector dimension ne (example : 2)
      Controller output vector dimension nu
      A[0,0] value (example : 12.354)
      ...
      A[0,nx]
      A[1,0]
      ...
      A[1,nx]
      ...
      A[nx,nx]
      B[0,0] value (example : 12.354)
      ...
      B[0,ne]
      B[1,0]
      ...
      B[1,ne]
      ...
      B[nx,ne]
      C[0,0] value (example : 12.354)
      ...
      C[0,nx]
      C[1,0]
      ...
      C[1,nx]
      ...
      C[nu,nx]
      D[0,0] value (example : 12.354)
      ...
      D[0,ne]
      D[1,0]
      ...
      D[1,ne]
      ...
      D[nu,ne]
      -------FILE_END-------
