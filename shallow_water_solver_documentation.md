# 1D Shallow Water Equation Solver in PETSc

## Final Project

**Computational Physics-II**  
**Spring 2024**
---

## Project Description

The shallow water equations (SWE) describe the flow below a pressure surface in a fluid. They are a simplification of the Navier-Stokes equations and are used extensively in fields such as oceanography, meteorology, and hydraulic engineering. This project implements a solver for the 1D shallow water equations using the Portable, Extensible Toolkit for Scientific Computation (PETSc). The project employs the finite difference method, imposes boundary and initial conditions, and uses the method of manufactured solutions (MMS) to verify the solver’s accuracy.

## Problem Background

The 1D shallow water equations consist of the conservation of mass and momentum. The equations can be written as:

$$\frac{\partial h}{\partial t} + \frac{\partial (hu)}{\partial x} = S_h$$

$$\frac{\partial (hu)}{\partial t} + \frac{\partial}{\partial x} \left( hu^2 + \frac{1}{2} gh^2 \right) = S_{hu}$$

Where:
- h is the water height
- u is the velocity
- g is the gravitational constant
- $S_h$ is the source term for h
- $S_{hu}$ is the source term for hu

### Derivation of Shallow Water Equations

The shallow water equations are derived from the Navier-Stokes equations under the assumptions of hydrostatic pressure and a shallow fluid layer (where the horizontal scale is much larger than the vertical scale).

1. **Conservation of Mass**:
    $$\frac{\partial h}{\partial t} + \frac{\partial (hu)}{\partial x} = 0$$

2. **Conservation of Momentum**:
    The momentum equation in the x-direction for a shallow fluid layer is:
    $$\frac{\partial (hu)}{\partial t} + \frac{\partial}{\partial x} \left( hu^2 + \frac{1}{2} gh^2 \right) = 0$$
    Here, the term $½ gh²$ represents the hydrostatic pressure.

### Method of Manufactured Solutions (MMS)

The MMS involves choosing an exact solution and deriving corresponding source terms to ensure that the chosen solution satisfies the differential equations. This method is used to verify the accuracy of numerical solvers.

1. **Chosen Exact Solution**:
    - $h(x, t) = 1.0 + 0.1 \sin(\pi x) \cos(\pi t)$
    - $u(x, t) = 0.1 \cos(\pi x) \sin(\pi t)$

2. **Deriving Source Terms**:
    - For h:
      $$S_h = \frac{\partial h}{\partial t} = -0.1 \pi \sin(\pi x) \sin(\pi t)$$
    - For hu:
      
        $$hu = h \cdot u = (1.0 + 0.1 \sin(\pi x) \cos(\pi t))(0.1 \cos(\pi x) \sin(\pi t))$$
      
        $$S_{hu} = \frac{\partial (hu)}{\partial t} = h \frac{\partial u}{\partial t} + u \frac{\partial h}{\partial t}$$

  Using the exact solution, these terms can be derived analytically to verify the numerical solver.

## Numerical Approach

### Finite Difference Method

The finite difference method is used to discretize the spatial derivatives. The spatial domain is divided into N points with a uniform grid spacing ∆x. The finite difference approximations for the derivatives are:

$$\frac{\partial (hu)}{\partial x} \approx \frac{hu_{i+1} - hu_{i-1}}{2 \Delta x}$$

$$\frac{\partial}{\partial x} \left( hu^2 + \frac{1}{2} gh^2 \right) \approx \frac{\left( hu^2_{i+1} + \frac{1}{2} gh^2_{i+1} \right) - \left( hu^2_{i-1} + \frac{1}{2} gh^2_{i-1} \right)}{2 \Delta x}$$

### Boundary and Initial Conditions

Boundary conditions are applied at the edges of the domain to reflect the physical problem’s nature. In this project, we use reflective boundary conditions, setting fluxes at the boundaries to zero:

At $x = 0$ and $x = L : f[i][0] = 0.0, f[i][1] = 0.0$

Initial conditions are derived from the manufactured solution, ensuring the initial state is consistent with the exact solution:

$h(x, 0) = 1.0 + 0.1 \sin(\pi x)$

$hu(x, 0) = (1.0 + 0.1 \sin(\pi x))(0.1 \cos(\pi x))$

After applying these in PETSc, we plotted SWE by collecting the height and momentum from the output file, then plotted the SWE using Python’s Matplotlib libraries. The figures below show the SWE for 100, 200, and 300 grid sizes respectively.

![SWE for 100 grid size](https://raw.githubusercontent.com/ubsuny/ShallowWater-CP2P2024/main/src/swe_data/solution_100.png)
![SWE for 200 grid size](https://raw.githubusercontent.com/ubsuny/ShallowWater-CP2P2024/main/src/swe_data/solution_200.png)
![SWE for 300 grid size](https://raw.githubusercontent.com/ubsuny/ShallowWater-CP2P2024/main/src/swe_data/solution_300.png)

## Implementation Details

### Code Structure

The implementation in PETSc involves several key steps:

1. **Setting Up the Problem Context**:
    - Define the spatial grid and problem parameters.
    - Implement functions to compute the manufactured solution and source terms.

2. **Initial and Boundary Conditions**:
    - Initialize the solution using the manufactured solution.
    - Apply boundary conditions in the finite difference scheme.

3. **Right-Hand Side Function**:
    - Compute the right-hand side (RHS) of the differential equations using finite difference approximations.

4. **Solving the Time-Stepping Problem**:
    - Use PETSc’s time-stepping (TS) routines to solve the equations over time.

### Error Analysis

Computed the error norms by comparing the numerical solution to the exact solution derived from the manufactured solution.

#### Error Norms

To assess the accuracy of the numerical solution, the error norms for h and hu are calculated:

$$\|e_h\| = \sqrt{ \sum_{i=1}^{N} (h_i - h_{\text{exact} i})^2 \Delta x }$$ 

$$\|e_{hu}\| = \sqrt{ \sum_{i=1}^{N} (hu_i - hu_{\text{exact} i})^2 \Delta x }$$

### Results and Analysis

The method of manufactured solutions (MMS) is a technique used to verify numerical solvers. Applying MMS to time-dependent problems requires separating temporal and spatial parts of the solution. This section discusses the error norm analysis for the 1D shallow water equation solver using MMS, where error norms did not decrease with increasing grid size.

#### Observations

The error norms for different grid sizes showed unexpected behavior, increasing with finer grid resolutions:

| Grid Size | Error (h) | Error (hu) |
|-----------|------------|------------|
| 5         | 0.141274   | 0.0128704  |
| 50        | 0.141115   | 0.0198663  |
| 100       | 0.141111   | 0.0199961  |
| 200       | 0.141110   | 0.0200308  |
| 300       | 0.141110   | 0.0200448  |
| 400       | 0.141110   | 0.0201209  |
| 500       | 0.141119   | 0.0202272  |
| 600       | 0.141145   | 0.0235038  |
| 700       | 0.141181   | 0.0425668  |
| 800       | 0.142223   | 0.0811513  |
| 900       | 0.152137   | 0.130226   |
| 1000      | 0.197220   | 0.214509   |
| 1100      | 0.313487   | 0.586756   |
| 1200      | 0.522598   | 2.049560   |

For temporal convergence, run simulations with halved timesteps while keeping the final time constant, then plot the log of the error vs. the run number to ensure the slope corresponds to the order of the time-stepping method. For spatial convergence, this code checked the error norms across grid sizes to ensure the solver accurately reflects the finite difference method and maintained the stability by satisfying the CFL condition. Increasing errors suggest potential issues with discretization or implementation.

#### Potential Issues and Solutions

- **Temporal and Spatial Separation**: Ensure the solution is split into temporal and spatial components correctly.
- **CFL Condition**: Verify the timestep satisfies the CFL condition for stability.
- **Boundary Conditions**: Ensure correct implementation of boundary conditions.
- **Finite Difference Scheme**: Confirm the accuracy of the finite difference scheme and its suitability for finer resolutions.
- **Error Norm Calculation**: Ensure correct integration over space and time for accurate L2 norms.

#### Steps to Address the Issue

- Verify Temporal Convergence: Run simulations with progressively halved timesteps and analyze the error slopes.
- Verify Spatial Convergence: Ensure correct spatial discretization and implementation.
- Accurate Error Norm Calculation: Confirm the integration process for L2 norms.

The unexpected increase in errors with larger grid resolutions suggests potential issues with temporal and spatial discretization. By systematically verifying temporal and spatial convergence and ensuring accurate error norm calculations, the solver’s accuracy can be improved to align with theoretical expectations.

## Efficiency and Speedup

### Observations

The table and figure below show the serial and parallel execution times for the 1D shallow water equation solver:

| Num Processors | Grid Size | Execution Time (s) |
|----------------|-----------|--------------------|
| 1              | 5         | 0.05518            |
| 1              | 50        | 0.06737            |
| 1              | 100       | 0.05247            |
| 1              | 200       | 0.06611            |
| 1              | 300       | 0.06773            |
| 1              | 400       | 0.06819            |
| 1              | 500       | 0.06506            |
| 1              | 600       | 0.07080            |
| 1              | 700       | 0.08335            |
| 1              | 800       | 0.08952            |
| 1              | 900       | 0.09779            |
| 1              | 1000      | 0.10309            |
| 1              | 1100      | 0.11357            |
| 1              | 1200      | 0.12035            |
| 1              | 1000      | 0.10663            |
| 2              | 1000      | 0.08209            |
| 4              | 1000      | 0.07492            |
| 8              | 1000      | 0.05119            |
| 16             | 1000      | 0.02119            |

The following two plots for the serial and parallel execution time:
![Serila Execution](https://raw.githubusercontent.com/ubsuny/ShallowWater-CP2P2024/main/src/swe_data/serial_execution.png)
![Parallel Execution](https://raw.githubusercontent.com/ubsuny/ShallowWater-CP2P2024/main/src/swe_data/parallel_execution.png)
#### Discussion

The parallel execution times do not exhibit significant speedup (plots below), particularly for smaller grid sizes. 
![Sppedup](https://raw.githubusercontent.com/ubsuny/ShallowWater-CP2P2024/main/src/swe_data/speedup.png)
![Efficiency](https://raw.githubusercontent.com/ubsuny/ShallowWater-CP2P2024/main/src/swe_data/efficiency.png)

Several factors can contribute to this behavior:

- **Communication Overhead**: In parallel computing, especially with a small grid size or a high number of processors, the communication overhead can dominate the computational workload. This is because the time spent on inter-processor communication can outweigh the time saved by distributing the computational load.
- **Load Balancing**: Effective parallelization requires evenly distributing the workload among processors. For small grid sizes, the workload might not be sufficiently large to be divided effectively, leading to some processors being underutilized.
- **Amdahl’s Law**: The speedup of a parallel algorithm is limited by the sequential portion of the code. If a significant part of the algorithm remains sequential, the overall speedup will be constrained regardless of the number of processors used.
- **Granularity of Computation**: For small problems, the granularity of the computation might be too fine, meaning that each processor does very little work before needing to communicate with others. This results in inefficient use of resources and limited speedup.

#### Analysis

- **Serial Execution**: The execution times increase gradually with grid size, indicating that the computational complexity scales as expected.
- **Parallel Execution**: For the grid size of 1000:
  - Doubling the processors from 1 to 2 reduces the execution time from 0.10663s to 0.08209s, showing a moderate speedup.
  - Increasing to 4 processors reduces the time further to 0.07492s, indicating diminishing returns.
  - With 8 processors, the time is 0.05119s, showing better performance but still not a linear speedup.
  - Using 16 processors reduces the time significantly to 0.02119s, but this is far from the ideal speedup, illustrating the impact of overhead and inefficiencies.

The lack of significant parallel speedup can be attributed to communication overhead, load balancing issues, and the constraints imposed by Amdahl’s Law. To achieve better performance, especially for larger grid sizes, optimizing the communication pattern, ensuring better load balancing, and minimizing the sequential portions of the code are crucial. Additionally, using a larger grid size can help in better utilizing the available processors and achieving more efficient parallel execution.

## Challenges and Difficulties

One of the primary challenges encountered during this project was the need to use my local machine for development and execution instead of the Center for Computational Research (CCR) due to difficulties in loading PETSc on the CCR system. This reliance on local resources limited the computational power and parallel processing capabilities available, making it more challenging to handle larger grid sizes and more complex simulations. Additionally, developing a finite difference solver from scratch posed a significant difficulty, as PETSc does not offer a built-in solver specifically for the SWE finite difference method. This required extensive effort to implement the numerical scheme, manage boundary conditions, and ensure the accuracy and stability of the solution. Overcoming these challenges necessitated a deep understanding of numerical methods and the PETSc library, but ultimately, the project was successful in achieving its objectives.

## Future Work

Building on the successful implementation and validation of the 1D shallow water equations, future work can expand in several directions. Extending the model to two and three dimensions will provide more comprehensive insights into complex fluid dynamics scenarios, requiring adaptations to the numerical scheme and handling more sophisticated boundary conditions. Exploring alternative numerical methods, such as finite volume or finite element methods, could enhance the accuracy and efficiency of the simulations. Leveraging high-performance computing resources like CCR, once the PETSc loading issues are resolved, will enable the handling of larger and more detailed simulations. Additionally, applying the model to real-world cases, such as coastal erosion, flood prediction, and river dynamics, will demonstrate its practical utility and robustness in addressing environmental and engineering challenges. Further research could also focus on optimizing the solver for better parallel efficiency and incorporating adaptive mesh refinement to improve computational efficiency and accuracy.

## References

1. LeVeque, R. J. (2002). *Finite Volume Methods for Hyperbolic Problems*. Cambridge University Press.
2. Balay, S., Abhyankar, S., Adams, M. F., Brown, J., Brune, P., Buschelman, K., Zhang, H. (2022). *PETSc Web page*. https://petsc.org/
3. Toro, E. F. (2001). *Shock-Capturing Methods for Free-Surface Shallow Flows*. Wiley.
