[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/hlMxVAew)
# 🎓 Lab 02: Cache and Tiling

The goal of this lab is to learn the memory architecture of
a multi-core processor and how to use the memory hierarchy to
speedup an application At the
end of the lab:
* you will know memory hierarchy of the processor that you are working.
* you will be able to use profiler to analyze the performance of your code.
* you will apply tiling to benefit from the fast memory.

**Note:** You must review Tutorial 01 before starting this lab.
***



## 🛠️ Logging in to the ECE Cluster
The ECE computer server is a local server used for this course. The ECE server
has 6 nodes, each with a 20-core Intel CPU and an Ada2000 GPU. The ECE server
uses a SLURM scheduler. SLURM (Simple Linux Utility for Resource Management)
is an open-source workload manager that allocates exclusive resources (computer nodes)
to users for a specific duration to run their tasks. It manages a queue of jobs,
ensuring that resources are used efficiently and jobs are run fairly. You will
need a username to log in to the server.  Your username is the one that you use
to login to PCs in labs. With that, you will need to SSH to the server using the
following command:

 ```
 ssh <username>@srv-cad.ece.mcmaster.ca
 ```

***

## ➕ Cloning the Repository

This step explains how to clone the repository. You will use **`git`**, a version
control system, to clone the repository and push your code to it. To clone
the repository, you will need to use the following command:


```
git clone https://github.com/4sp4-2025/<repo name>.git
```

Where `repo name` is your repository. The repository is private, so you may get an
error when you enter your username and password. To solve this, you will need to
use a **Personal Access Token (PAT)** instead of your password.


***

## 🚀 Building and running the starter code

This section explains how you can build and run the code on the
ECE cluster. First, go to where the project is cloned:

```
cd <where/the/repo/is/cloned>
```
All necessary instructions to build and run the code are
provided in the `build_run.sh` file.
You can open the file using a text editor like `nano` or `vim`,
e.g., `vim build_run.sh`.  Use the following command to build
and run the code:

```
sbatch build_run.sh
```

This command submits the `build_run.sh` script to the SLURM scheduler.
The instructions in the `build_run.sh` file are
unix commands that will be executed on a compute node. The output of
the job will be saved in a `*.out`file.

* **Never use `bash`** to run your code on the login node.
  Use `sbatch` instead. `bash` runs the code directly on
  the login node, while `sbatch` submits the job to the SLURM
  scheduler, which runs it on an available compute node.
* You should **never** run code on the login node unless it
  takes less than 30 seconds.
* You can check the status of your job using `squeue -u <username>`.


***


## ✅ Tasks

You will first design an experiment to determine different levels of 
caches and cache line size. Then, you will implement a numerical solver 
for the heat equation and apply loop tiling to it to benefit from the 
cache hierarchy of the processor.

*Note*: the number of iterations in the google benchmark should not be larger than 3.

### Task 1 : Cache Hierarchy Analysis
In this task, you will analyze cache hierarchy of the processor's memory
and implement a copy benchmark to measure the size of different levels of caches. 
You will need to do time measurement for this part. 
You will also need to do profiling with proper counters to back up your cache size claims.
You may need to work with compiler optimization flags to get the best results.
To ensure the copy function works as expected, you should pick a flag that does not change the 
code behaviour. 

### Task 2 : Heat Equation Solver
In this task you will first implement a part of the numerical solver 
for the heat equation. Then, you will apply loop tiling to the solver 
to benefit from the cache hierarchy of the processor.


### Overview

The heat equation is a partial differential equation (PDE) that 
describes the distribution of heat (or temperature) in a given 
region over time. It is widely used in physics and engineering
to model heat conduction. The equation in two dimensions is given as:
```angular2html
∂u/∂t = α(∂²u/∂x² + ∂²u/∂y²)
```
Where:
- `u(x, y, t)` is the temperature at position `(x, y)` and time `t`.
- `a` is the thermal diffusivity constant.
- `∂u/∂t` is the rate of change of temperature over time.
- `∂²u/∂x²` and `∂²u/∂y²` are the second spatial derivatives of temperature.

The equation is solved numerically using finite difference methods, 
where the spatial domain is discretized into a grid, and the time evolution 
is computed iteratively.

### Numerical Solver

#### Five-Point Stencil Formula

The numerical solver uses the five-point stencil formula to compute the 
temperature at each grid point:
```angular2html
u(i, j, t+Δt) = u(i, j, t) + αΔt( (u(i+1, j, t) - 2u(i, j, t) + u(i-1, j, t))/Δx² + (u(i, j+1, t) - 2u(i, j, t) + u(i, j-1, t))/Δy² )
```
Where:
- `u(i, j, t)` is the temperature at grid point `(i, j)` at time `t`.
- `Δt` is the time step.
- `Δx` and `Δy` are the spatial step sizes in the x and y directions, respectively.
- `α` is the thermal diffusivity constant.
- `u(i+1, j, t)`, `u(i-1, j, t)`, `u(i, j+1, t)`, and `u(i, j-1, t)` are the 
 temperatures at the neighboring grid points. The formula updates the temperature 
 at each grid point based on its current value and the values of its four immediate neighbors.
 This method is efficient and provides a good approximation of the heat distribution over time.

#### What is Stencil computation?
Stencil computations are a class of numerical data processing solution which update array
elements according to some fixed pattern, called a stencil.

#### Boundary Conditions
The solver implements Dirichlet boundary conditions, where the temperature at the
boundaries of the grid is fixed.

#### Initial Conditions
The initial temperature distribution is set using a Gaussian function centered in
the grid, simulating a heat source.

### Code Description

#### Key Components

1. **Data Structures**:
  - The `field` structure represents the temperature grid, including its dimensions, 
   grid spacing, and temperature values.

2. **Numerical Solver**:
  - The `evolve` function updates the temperature grid using the five-point stencil method.
  - The `evolve_tiled` function applies loop tiling to improve cache performance.

3**Output**:
- The code writes the temperature field to a PNG file at regular intervals and 
  computes the average temperature for analysis.


For task 2, you will implement the evolve first and then you will tile the evolve function. 
You will need to do proper experimentation and profiling to select the tile size(s).

*Hint* an improvement of around 5% is acceptable. More than 10% is excellent and 
will be rewarded with bonus points.

### The expected output
Your submission must include the following:

* Necessary benchmarks to make an experiment for the cache and cache line size.
* Use above data to determine the size of different levels of caches. (with plots)
* Implement the stencil kernel for the heat equation and provide enough tests to verify its correctness.
We do not grade your tests but it is important to verify your code works as expected.
* Apply loop tiling to the stencil kernel and show the performance improvement versus the baseline code.
* Profile the stencil kernel for different sizes of data to find the best tile size and 
 the loops that are selected for tiling (with plots)
* The necessary Python scripts (and packages) to generate all plots. You won't be allowed to push plots to the repo.
  We should be able to generate all plots using your scripts and the logs generated from running your
  code on the ECE server.


### Evaluation
Your lab submission will be graded based on these criteria:

* Your code compiles and runs successfully on the ECE cluster using the provided build_run.sh script (Pass/Fail).
* All grading tests are passed as expected.
* The quality of your plots and its analysis, which must:
  * Convey a correct and clear argument.
  * Be visually understandable with proper labels, units, and a concise 1-2 sentence executive summary.
* All plots must be generated using your provided Python scripts and the logs from your code runs on the ECE server.
  Plots pushed directly to the repository will not be accepted.
  **Negative** points will be assigned for any plots/logs that are pushed to the repo.


### Submission
Follow these guidelines for a successful submission:
* First, please implement all TODOs in the code and remove all comments that
  start with `TODO`.
* Push all your code to the main branch of your repository before the deadline.
* Ensure your code compiles and runs on the ECE cluster. Submissions that fail to
  compile or run will receive a grade of zero. The TA will only use the `build_run.sh` file for
  building and running your code. You may want to make a copy of the script for your final testing.
* The build_run.sh script's execution should only output results from Google Test and
  Google Benchmark. Do not include any extra output from cout or printf. This can negatively
  impact your grade, and regrade requests for this reason will not be accepted.
* All logs should be redirected to `logs/` directory, and all plots should be saved in the
  `plots/` directory. These directories are not tracked by git to avoid pushing log files
  or plots to the repository.




## Descriptive Answers (TODO)
Typically there is no single correct answer/plot for the following questions. Rely on your thought process!

### Plot(s) 1: experiment for cache and cache line size

TODO: make sure to reference the correct plot below

![Figure 1: Standard Sort Algorithm Complexity](plots/plot1.png)

Description: TODO: please provide details for your plot(s) here.

### Plot(s) 2: Tiling 
TODO: follow like above example


# Licence
Created by SwiftWare Lab on 9/25.
CE 4SP4 - High Performance Programming
Copyright (c) 2025 SwiftWare Lab.
Distribution of the code is not
allowed in any form without permission
from SwiftWare Lab.




