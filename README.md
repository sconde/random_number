# Random Number Generation Exercise

This project is a simple exercise in generating random numbers in C++. It's designed to help you understand how to use the standard library's random number generation facilities, as well as how to use them in conjunction with OpenMP for parallel computation.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- A modern C++ compiler with support for C++11.
- CMake version 3.10 or higher.
- OpenMP for parallel computation.

### Building

To build the project, follow these steps:

1. Clone the repository: `git clone https://github.com/sconde/random_number.git`
2. Navigate into the project directory: `cd random_number`
3. Create a build directory and navigate into it: `mkdir build && cd build`
4. Run CMake to generate the build files: `cmake ..`
5. Build the project: `make`

## Running the Tests

This project will use Google Test for unit testing. To run the tests, simply execute the test binary in the `build` directory after building the project.

## Usage

After building the project, you can run the `RNG` executable to generate random numbers. The usage is as follows:
