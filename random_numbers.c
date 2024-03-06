#include <petscsys.h>

/*
 * Main function to demonstrate the generation of random numbers using PETSc.
 */
int main(int argc,char **argv)
{
  PetscInitialize(&argc,&argv,(char*)0,NULL); // Initialize the PETSc library.
  PetscRandom    rctx;                        // Declare the PETSc random context variable.
  PetscReal      rval;                        // Variable to store the generated random value.

  PetscRandomCreate(PETSC_COMM_WORLD,&rctx);  // Create a random number generator context.
  PetscRandomSetFromOptions(rctx);            // Set options for the random number generator from command-line options.

  // Generate a single random number and store it in rval.
  PetscRandomGetValue(rctx,&rval);
  // Print the generated random number.
  PetscPrintf(PETSC_COMM_WORLD,"Random Value: %g\n",(double)rval);

  PetscRandomDestroy(&rctx); // Clean up the random number generator context.
  PetscFinalize();           // Finalize the PETSc library to free resources.
  return 0;
}
