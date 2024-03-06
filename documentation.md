# Shallow Water Equation Simulation with PETSc

This project leverages the power of PETSc to simulate the dynamics of shallow water bodies. Utilizing PETSc's robust parallel computing capabilities, we aim to provide accurate and efficient solutions to the SWE, fundamental in understanding fluid flows in environments where the horizontal length scale significantly exceeds the vertical depth.

## Features
- **Efficient Parallel Computing:** Harness PETSc for scalable simulations.
- **Random Initial Conditions:** Employ PETSc's random number generation for diverse scenarios.
- **Flexible:** Designed for adaptability to various shallow water scenarios.

Explore the intricacies of fluid dynamics with our PETSc-based simulation tool.


# PETSc Data Types Documentation

Welcome to the documentation for utilizing PETSc (Portable, Extensible Toolkit for Scientific Computation) with C and Python data types. PETSc is a suite of data structures and routines for the scalable solution of scientific applications modeled by partial differential equations. It supports parallel processing and can be used with both C and Python programming languages, offering a wide range of functionalities for numerical computation.

## Table of Contents

- [Introduction](#introduction)
- [Getting Started](#getting-started)
- [Data Types in PETSc](#data-types-in-petsc)
  - [C Data Types](#c-data-types)
  - [Python Data Types](#python-data-types)
- [Examples](#examples)
  - [C Example](#c-example)
  - [Python Example](#python-example)
- [Conclusion](#conclusion)

## Introduction

This documentation focuses on the fundamental data types used in PETSc when programming in C and Python, providing a foundational understanding necessary for scientific computing projects. PETSc abstracts many complex structures into simple, manageable data types, facilitating operations such as matrix manipulation, linear algebra, and system solving in a parallel computing environment.

## Getting Started

To use PETSc, you first need to install it on your system. PETSc can be installed from source or via package managers. For detailed installation instructions, refer to the [official PETSc installation guide](https://petsc.org/release/install/).

## Data Types in PETSc

### C Data Types

In C, PETSc introduces several data types that encapsulate underlying complex data structures. 
For more detailed information on these data types, please look at the [PETSc Data Types Documentation](https://petsc.org/release/docs/manualpages/Sys/PETScDataTypes.html).
Here are some of the core data types:

- **Vec**: Represents vectors (arrays) in PETSc. Used for storing and manipulating vectors in parallel.
- **Mat**: Represents matrices in PETSc. Supports various matrix operations and storage formats.
- **KSP**: The Krylov Subspace solver context. Used for solving linear systems.
- **SNES**: Represents the nonlinear solver context in PETSc, which is used for solving nonlinear systems.

### Python Data Types

PETSc can be used in Python via the `petsc4py` package, which provides Python bindings for PETSc. The core data types in C are mirrored in Python with similar functionality:

- **Vec**: Accessed in Python via `petsc4py.PETSc.Vec()`. Represents distributed arrays.
- **Mat**: Accessed via `petsc4py.PETSc.Mat()`. Used for creating and manipulating matrices.
- **KSP**: The linear solver context in Python, accessible via `petsc4py.PETSc.KSP()`.
- **SNES**: Represents the nonlinear solvers in Python, accessible through `petsc4py.PETSc.SNES()`.

## Examples

### C Example

A simple example to create a vector, set values, and display it:

```c
#include <petscvec.h>
int main(int argc,char **args)
{
    Vec            x;
    PetscInitialize(&argc,&args,(char*)0,NULL);
    VecCreate(PETSC_COMM_WORLD,&x);
    VecSetSizes(x,PETSC_DECIDE,100); // Vector of global size 100
    VecSetFromOptions(x);
    VecSet(x,1.0); // Set all values to 1.0
    VecView(x,PETSC_VIEWER_STDOUT_WORLD);
    VecDestroy(&x);
    PetscFinalize();
    return 0;
}
```


from petsc4py import PETSc

x = PETSc.Vec().create(PETSc.COMM_WORLD)
x.setSizes(100) # Vector of size 100
x.setFromOptions()
x.set(1.0) # Set all elements to 1.0

x.view()


### Python Example
Creating and manipulating a vector in Python using `petsc4py`:

```python
from petsc4py import PETSc

x = PETSc.Vec().create(PETSc.COMM_WORLD)
x.setSizes(100)  # Vector of size 100
x.setFromOptions()
x.set(1.0)  # Set all elements to 1.0

x.view()
