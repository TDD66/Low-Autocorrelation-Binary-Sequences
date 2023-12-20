# Low Autocorrelation Binary Sequence Search using MPI

## Purpose
This project aims to efficiently search for low autocorrelation binary sequences using parallel computing techniques implemented with MPI (Message Passing Interface). Autocorrelation sequences find extensive application in various fields like communication systems, radar, cryptography, and signal processing. This program focuses on finding sequences with minimal autocorrelation energy, a crucial factor in optimizing these systems' performance.

## Features
- **Parallel Search**: Utilizes MPI to distribute the search space among multiple processes for parallel computation, reducing search time significantly.
- **Bitwise Operations**: Efficiently manipulates binary sequences using bitwise operations and bit counting techniques for autocorrelation calculation.
- **Energy Optimization**: Calculates autocorrelation energy for each sequence and identifies the sequence with the lowest energy, optimizing for minimal autocorrelation.

## Getting Started
1. **Prerequisites**: Ensure you have MPI installed on your system. You can use OpenMPI or MPICH.
2. **Compile**: Compile the program using an MPI compiler (`mpicc` or similar) using the provided compilation command.
3. **Run**: Execute the compiled program using an MPI runtime (`mpirun` or similar) and specify the number of processes.

## Usage
- Modify the `length` variable in the code to change the length of the binary sequences to be searched.
- Adjust parameters like `comms_rate` to control the frequency of communication among processes during the search.
- Experiment with different search space divisions (`N`) and parallel processes to optimize performance.

## Contributors
- [TDD66]

## License
This project is licensed under [License Name] - (Include the license file or link to the license).
