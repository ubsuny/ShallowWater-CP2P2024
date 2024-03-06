# Random Number Generation in Shallow Water Equation Simulation

## Overview
This project simulates the shallow water equations using the Portable, Extensible Toolkit for Scientific Computation (PETSc). An integral part of this simulation involves generating random numbers for various purposes, including initializing simulation states and introducing stochastic elements to explore different scenarios.

## Usage of Random Numbers
Random numbers are utilized in the following areas of the project:

- **Initial Conditions**: To simulate diverse initial water surface levels and velocities, we generate random numbers to initialize the state vectors.
- **Boundary Conditions**: In some cases, random fluctuations are added to boundary conditions to simulate environmental variability.
- **Parameter Sensitivity Analysis**: Random numbers are used to vary parameters within a predefined range to study the system's sensitivity.

## Implementation
Random numbers are generated using PETSc's random number generation utilities. Here is a snippet that demonstrates how to create a random number generator and generate a single random value:

```c
#include <petscsys.h>
int main(int argc,char **argv)
{
  PetscInitialize(&argc,&argv,NULL,NULL);
  PetscRandom    rctx;
  PetscReal      rval;
  PetscRandomCreate(PETSC_COMM_WORLD,&rctx);
  PetscRandomSetFromOptions(rctx);
  PetscRandomGetValue(rctx,&rval);
  PetscPrintf(PETSC_COMM_WORLD,"Random Value: %g\n", (double)rval);
  PetscRandomDestroy(&rctx);
  PetscFinalize();
  return 0;
}
```
