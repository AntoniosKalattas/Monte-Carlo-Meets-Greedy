# epl236project

Both C and Java implementations


## C Implementation
Due to a lack of debugging tools, problems encountered during benchmarking, and difficulties in performance optimization, I decided to drop it and start over in Java.


# Experimental Evaluation
## Experiment 1
#### Input Conditions
Since our goal is to make it difficult for the algorithm to easily find a hitting set, so that we can evaluate how it responds to challenging inputs, the input for this test must satisfy the following conditions:
**Large Value Range (N)**: To make it harder to find a hitting set, the subsets must not overlap significantly. Therefore, N should be large so there are many different elements, and each subset will contain different elements.
**Small Subset Size (C)**: To reduce the likelihood of overlap between subsets — that is, when selecting an element, to avoid that same element appearing in other subsets — the size C should be small.
**Small Number of Subsets (M)**: To further reduce the possibility of overlap, we limit the number of subsets. Having few subsets that meet the above criteria reduces the chance of elements appearing in multiple subsets.




