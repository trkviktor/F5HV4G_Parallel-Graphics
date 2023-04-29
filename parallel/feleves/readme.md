## OPENMP | *Implementation of the Floyd-Warshall algorithm in C.*

This program generates a distance matrix from user input (N * N).
Each element represents the distance and weight between two nodes and both of them are randomly generated.
Then using the algorithm it iterates over all nodes until it finds the shortest path between the nodes.
The calculating happens X times determined by the user on different threads,measuring the speed of the threads.

## PTHREAD | *Implementation of the Maximum Likelihood estimation*

This program takes two command-line arguments, the number of threads to use and the number of samples to use from the user. 
Then it calculates the maximum likelihood function in two ways, sequentially and in parallel, then measures the time it takes for each method.


## PYTHON | *Speed difference between sequential and parallel interpolations*

This program generates random points between [-5;5] then using sequential and parallel interpolations to interpolate it,then it splits it into equal amounts depending on the available threads of the user's cpu.
Finally,it measures the time it took to interpolate the points between the to methods and the user can compare it after.