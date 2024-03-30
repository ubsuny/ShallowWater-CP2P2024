# PETSc Structured Grid Poisson Solver

This repository contains a Poisson solver using the Portable, Extensible Toolkit for Scientific Computation (PETSc). The solver utilizes a structured grid and the Krylov Subspace Methods (KSP) for solving the discretized Poisson equation in two dimensions.[^1]

## Introduction

The Poisson equation is a fundamental partial differential equation that appears in many fields of science and engineering, such as electrostatics, mechanical engineering, and thermal dynamics. This solver employs PETSc's distributed array (DMDA) for grid management and its linear system solvers for finding the solution.[^2]

## Prerequisites

- PETSc installed on your system
- A C compiler, such as GCC
- An MPI implementation for parallel execution

## Compilation

To compile the Poisson solver, run the following command in the terminal:

```bash
make poisson.c
```
## Bibliography

[^1]: [PETSc Documentation](https://petsc.org/release/)
[^2]: [PETSc KSP Documentation](https://petsc.org/main/manualpages/KSP/KSP/)
