# Shallow Water Equations (SWE) Solver

## Overview

This project implements a numerical solver for the shallow water equations (SWE) using the finite difference method (FDM). It's designed for simulating fluid dynamics in thin layers, applicable in studying phenomena like tsunamis, tidal waves, and atmospheric flows over large scales.

## Equations

The solver simulates the SWE set, focusing on momentum equations and mass conservation:

- **Momentum Equations:**

 $$
\frac{\partial u}{\partial t} + u\frac{\partial u}{\partial x} + v\frac{\partial u}{\partial y} + g\frac{\partial \eta}{\partial x} = 0
$$

$$
\frac{\partial v}{\partial t} + u\frac{\partial v}{\partial x} + v\frac{\partial v}{\partial y} + g\frac{\partial \eta}{\partial y} = 0
$$


- **Mass Conservation:**

$$
\frac{\partial \eta}{\partial t} + \frac{\partial[(\eta + h)u]}{\partial x} + \frac{\partial[(\eta + h)v]}{\partial y} = 0
$$


## Initial and Boundary Conditions

Initial conditions must be specified for fluid velocity and surface elevation at the start of the simulation. The solver supports various boundary conditions to model different physical scenarios accurately:

### Free Surface Condition
The pressure is zero along the free surface, and the vertical displacement of the free surface is given by the kinematic boundary condition:

$$
p = 0, \quad \frac{\partial \eta}{\partial t} + \mathbf{v} \cdot \nabla \eta = w, \quad \text{on} \quad z = \eta(x, y, t)
$$

### Momentum Equation
Under the assumption of hydrostatic balance, the momentum equation is simplified to:

$$
\frac{D\mathbf{u}}{Dt} + \frac{1}{\rho} \nabla p + \mathbf{g}\hat{\mathbf{z}} = 0
$$

### Continuity Equation
Fluid incompressibility is represented by the divergence-free condition for the velocity field:

$$
\nabla \cdot \mathbf{u} = 0
$$


### Bottom Boundary Condition
The impermeable bottom boundary is modeled by the no-flow-through condition:

$$
\mathbf{u} \cdot \nabla(z + h(x, y)) = 0, \quad \text{on} \quad z = -h(x, y)
$$


Here, `p` is the pressure, `η` the vertical displacement of free surface, `u = (u, v, w)` the three-dimensional velocity, `ρ` the density, `g` the acceleration due to gravity, and `h(x, y)` the bottom topography.

## Numerical Approach

The project uses spatial discretization via the finite difference method and explicit time-stepping. Stability is maintained by adhering to the Courant-Friedrichs-Lewy (CFL) condition.
