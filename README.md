# Autocorrelation Binary Sequence Search using MPI

## Overview
The Autocorrelation Binary Sequence (LABS) problem poses the challenge of finding binary sequences with minimal autocorrelation, a critical pursuit in various fields like communications engineering, radar technology, statistical physics, and digital signature generation. These sequences play a pivotal role in reducing noise and enhancing the ability to detect desired targets in radar, sonar, and communication systems.

The search for Low Autocorrelation Binary Sequences (LABS) is computationally demanding and falls within the realm of NP-complete problems. The search space grows exponentially with the sequence length, making it an arduous task even for moderately sized sequences. For instance, a sequence length of 𝐿 = 50 results in a search space of approximately 2^50 sequences, illustrating the complexity and magnitude of the search problem.

## Objective
This project aims to employ parallel processing through MPI to tackle the LABS problem efficiently. By leveraging multiple virtual machines to distribute the search space, the objective is to expedite the exhaustive search process, especially for longer sequence lengths. 

## Purpose
- **Applications**: LABS finds applications in enhancing radar and communication systems by minimizing autocorrelation side lobe energies, contributing to noise reduction and improved target detection.
- **Challenges**: The vast search space for binary sequences presents computational challenges, necessitating optimized algorithms and parallel computing techniques for efficient exploration.

## Features
- **MPI Utilization**: Utilizes MPI for parallel processing, enabling the distribution of the search space across multiple computing nodes.
- **Energy Optimization**: Calculates autocorrelation energy to identify sequences with the lowest energy, optimizing for minimal autocorrelation.
- **Exhaustive Search**: Due to the complexity of the problem, an exhaustive search approach is employed to find optimal solutions.

## Getting Started
1. **Prerequisites**: Ensure MPI is installed on your system (e.g., OpenMPI or MPICH).
2. **Compilation**: Compile the program using an MPI compiler (`mpicc` or similar) with the provided command.
3. **Execution**: Run the compiled program using an MPI runtime (`mpirun` or similar), specifying the number of processes.

## Usage
- Modify the `length` variable in the code to adjust the length of binary sequences to be searched.
- Experiment with different search space divisions and parallel processes to optimize performance.

## License
This project is licensed under [License Name] - (Include the license file or link to the license).
