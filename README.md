# Brandes
Brandes algorithm (sequential and parallel).

brandes-seq usage : ./brandes-seq fileIn fileOut

fileIn contains edges of graph in format "a b" denoting that in a graph exists edge from a to b.

fileOut is a file where results (vertex and its betweenness centrality) are written.

brandes-par usage : ./brandes-par numberOfThreads fileIn fileOut

fileIn contains SORTED edges of graph in the same format as before (0 1 before 0 2 before 1 2)


