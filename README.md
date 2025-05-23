# EPL236 Project – Hitting Set Problem

This project explores multiple algorithmic strategies to solve the **Hitting Set Problem**, implemented in both **C** and **Java**. The emphasis is on evaluating performance under challenging input conditions.

---

## 🔧 Implementations

### 🧵 C Version (Abandoned)
Initial implementation in C was halted due to:
- Limited debugging tools
- Performance bottlenecks
- Optimization challenges

### ☕ Java Version
A full reimplementation in Java was carried out for better control over debugging and performance benchmarking.

---

## 📊 Experimental Evaluation

### 🧪 Experiment 1 – Challenging Inputs

#### 🔢 Input Conditions
- **N (value range):** Large – ensures low overlap among subsets
- **C (subset size):** Small – minimizes shared elements
- **M (number of subsets):** Small – increases uniqueness of elements across subsets

#### 📈 Results

**N = 5000, M = 10000, C = 5**
![Experiment 1 - Set A](https://github.com/AntoniosKalattas/epl236project/blob/main/images/image2.png)

**N = 1000, M = 5000, C = 3**
![Experiment 1 - Set B](https://github.com/AntoniosKalattas/epl236project/blob/main/images/image1.png)

#### 🧠 Observations
- All algorithms are fast for small `k`.
- **Algorithm 1** is consistently the slowest.
- **Algorithms 1 & 2** suffer from exponential time increase after `k > 8–9`.
- **Algorithm 3** is the most efficient, scaling linearly up to high `k`.
- **Algorithm 4** (hybrid strategy) balances performance but eventually degrades.

#### ✅ Summary
- Algorithm choice is irrelevant for small `k`.
- For large `k`, **Algorithm 3** is best.
- **Algorithm 2 & 4** are theoretically smarter, but practically slower due to costly decision-making.

---

### 🔮 Prediction vs. Reality

| Algorithm | Prediction | Result |
|----------|------------|--------|
| Algorithm 1 | Slowest | ✔ Confirmed |
| Algorithm 2 | Much faster than 1 | ❗ Difference was smaller than expected |
| Algorithm 3 | Best for large M | ✔ Proven highly efficient |
| Algorithm 4 | Fastest overall | ❗ Strategy costs slowed it down |

---

### 🧪 Experiment 2 – Hard-to-Find Solutions

#### 🔢 Input Conditions
- **N:** Large, but not extreme – balances uniqueness and complexity
- **M:** Large – increases search space
- **C:** Small – reduces overlap

#### 📈 Results

**N = 100, M = 500, C = 4, k = 50**
![Experiment 2 - Set A](https://github.com/AntoniosKalattas/epl236project/blob/main/images/image3.png)

**N = 400, M = 300, C = 10, k = 49**
![Experiment 2 - Set B](https://github.com/AntoniosKalattas/epl236project/blob/main/images/image4.png)

#### 🧠 Observations
- **Algorithm 1:** Unpredictable, high variance
- **Algorithm 2:** Fast and consistent; best pruning strategy
- **Algorithm 3:** Slow but stable
- **Algorithm 4:** Overall best; optimal balance of speed and stability

#### ✅ Summary
- **Algorithm 1:** Unstable – avoid
- **Algorithm 2:** Fast and good, but not the best
- **Algorithm 3:** Reliable but slow
- **Algorithm 4:** Top performer – handles large, complex cases well

---

### 🔮 Prediction vs. Reality

| Algorithm | Prediction | Result |
|----------|------------|--------|
| Algorithm 1 | High variance | ✔ Confirmed |
| Algorithm 2 | Effective pruning | ✔ Confirmed |
| Algorithm 3 | Ideal for big inputs | ❗ Overestimated |
| Algorithm 4 | Optimal hybrid | ✔ Confirmed |

---

## 📁 Repository Structure

```plaintext
├── java/
│   └── Main.java
├── c/
│   └── main.c (legacy)
├── images/
│   ├── image1.png
│   ├── image2.png
│   ├── image3.png
│   └── image4.png
└── README.md
