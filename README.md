# epl236project
Both C and Java implementations


## C Implementation
Due to a lack of debugging tools, problems encountered during benchmarking, and difficulties in performance optimization, I decided to drop it and start over in Java.


## Experimental Evaluation

### Experiment 1

#### Input Conditions

To make the algorithm struggle in finding a hitting set and evaluate its performance with difficult inputs, the input must meet the following:

- **Large Value Range (N):** Subsets should have minimal overlap. A large N ensures many unique elements across subsets.
- **Small Subset Size (C):** Keeps overlap low; elements are less likely to appear in multiple subsets.
- **Small Number of Subsets (M):** Fewer subsets reduce overlap and increase challenge.

#### Experiment 1 Results
N = 5000	M = 10000	C = 5
![Alt text](https://github.com/AntoniosKalattas/epl236project/blob/main/images/image2.png)

N = 1000	M = 5000	c = 3
![Alt text](https://github.com/AntoniosKalattas/epl236project/blob/main/images/image1.png)

#### Observations and Conclusions

- For small `k`, all algorithms terminate immediately.
- **Algorithm 1** performs the worst overall.
- For `k > 8–9`, **Algorithms 1 and 2** experience exponential growth in runtime.
- **Algorithm 4**, which combines 2 and 3, performs better than 1 and 2 but eventually also fails to terminate.
- **Algorithm 3** is the best, maintaining linear time until very large `k`, where others fail.

##### Summary:
- For small `k`, the algorithm choice is insignificant.
- For large `k`, **Algorithm 3** is the fastest.
- **Algorithms 2 and 4** have smart strategies but high costs due to critical element selection.

---

### Prediction Comparison (a)

- **Algorithm 1:** As predicted, the slowest and least strategic.
- **Algorithm 2:** Expected to be much faster than 1, but not as large a difference as expected.
- **Algorithm 3:** Proven highly efficient for large M — performs excellently until mid `k`.
- **Algorithm 4:** Expected to be the fastest due to its combined strategy, but practical costs reduced its efficiency.

---




### Experiment 2

#### Input Conditions

The input must have a hitting set that’s hard to detect:

- **Large Value Range (N):** Reduces likelihood of elements repeating across subsets but shouldn't be too large.
- **Large Number of Subsets (M):** Increases search space complexity.
- **Small Subset Size (C):** Decreases element overlap, increases possible hitting set combinations.

#### Experiment 2 Results
N =100		M = 500	C = 4		k=50
![Alt text](https://github.com/AntoniosKalattas/epl236project/blob/main/images/image3.png)
N =400		M = 300	C = 10		K=49
![Alt text](https://github.com/AntoniosKalattas/epl236project/blob/main/images/image4.png)

#### Observations and Conclusions

- **Algorithm 1:** Very high variability, unreliable; random selection is ineffective.
- **Algorithm 2:** Stable, fast; frequency-based selection is highly effective.
- **Algorithm 3:** Slower despite strategy; selecting the smallest subset is computationally cheap but hampers solution discovery.
- **Algorithm 4:** Best overall; fastest and most consistent.

##### Summary:
- **Algorithm 1**: Unreliable, could take seconds or hours.
- **Algorithm 2**: Fast and stable, slightly slower than 4.
- **Algorithm 3**: Consistent but slow.
- **Algorithm 4**: Best in speed and reliability — recommended for large, complex instances.

---

### Prediction Comparison (a)

- **Algorithm 1:** As predicted, highly variable and unreliable.
- **Algorithm 2:** Confirmed to effectively reduce search space with frequency-based selection.
- **Algorithm 3:** Overestimated; selecting smallest subset doesn't ensure fast solution.
- **Algorithm 4:** As expected, combines strengths of 2 and 3 for optimal performance regardless of input size or complexity.





