# Implementation Notes

## Sampling

Problem: We receive undirected edges as directed edges in both directions from the graph generator.
However, the sampling would be far easier if these edges were only present in one direction, to prevent accidentally sampling duplicates.

Solution: We went through multiple sampling strategies:

1. `std::mt19937`: Select an edge `e` if `e.tail < e.head` with probability p = n / √(n\*m) = √(n/m).
  This turned out to be very slow because branch miss-predictions on these conditions are very common and we have to iterate over the whole edge list.

2. `std::mt19937`: Select √(n\*m) edges `edge_list[x_i]` where `x_i` is sampled uniformly from `[0, edge_list.size())`. This way we may sample edges twice, but we found this to not diminish the quality of the sample.

3. `XORShift128`: Same as 2. but with a faster random number generator. We could not detect any compromise in quality.

4. Deterministic Sampling: Select every `edge_list.size() / √(n*m)`-th edge from the edge list.
This was more than an order of magnitude faster than relying on random number generators at no disadvantage to the quality of the sample. So we settled for this strategy in our final implementation.

## Graph Representation

We use single undirected edges where possible and only add reverse edges during the adjacency array buildup for the Jarnik-Prim subroutine.
The edges in the adjacency array are reduced to weight-head pairs instead of carrying the tail as well.

## Range Maximum Queries

We expand the levels to the next power of two allow for efficient shift operations instead of multiplications. This allows us to simplify the buildup. We also flattened the levels into one single array.

To speedup the filter loop condition for including edges between components, we set the weight of the root node in each component to ∞. This way, queries across boundaries return ∞ and the condition `e.weight < query(e)` evaluates to true.

## Jarnik-Prim

Our Jarnik-Prim implementation is very straight forward. We use a specialized version for computing the minimum spanning forest for our sampled edges and combine data that is accessed together to minimize cache misses. For edge priorities we use an indexed priority queue implementation which turned out to be faster than `std::priority_queue`, especially for dense graphs.

## Filter Loop

The filter loop turned out to be the main bottleneck because we need to check the filter condition for each edge. However, through the aforementioned techniques we reduced the computations as best as possible. Additionally, we split the split the query on the range maximum query data structure to allow for short-circuit evaluation because in this case cache misses are more expensive than branch miss-predictions.

## Exploit Simple Instances

Finally, our algorithm decides based on the expected runtime of Jarnik-Prim and IMaxFilter whether to run only a single Jarnik-Prim iteration or run the full IMaxFilter algorithm.

## Parallel IMaxFilter

We added a simple parallel implementation of our IMaxFilter that uses OpenMP to speed up the filter loop.
