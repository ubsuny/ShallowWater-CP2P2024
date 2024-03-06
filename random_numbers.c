#include <petscsys.h>

int main(int argc,char **argv)
{
  PetscInitialize(&argc,&argv,(char*)0,NULL);
  PetscRandom    rctx;
  PetscReal      rval;

  PetscRandomCreate(PETSC_COMM_WORLD,&rctx);
  PetscRandomSetFromOptions(rctx);
  
  // Generate a random number
  PetscRandomGetValue(rctx,&rval);
  PetscPrintf(PETSC_COMM_WORLD,"Random Value: %g\n",(double)rval);

  PetscRandomDestroy(&rctx);
  PetscFinalize();
  return 0;
}

