import random

# Parameters: Adjust these as desired.
n = 50    # Universe elements 1 through 50
m = 900   # Number of subsets (rows)
c = 6     # Each subset will have exactly 6 elements
k = 14    # The hitting set size

output_filename = "large_test_input.txt"

# 1. Generate a random hitting set candidate of size k.
hitting_set_candidate = random.sample(range(0, n+1), k)

with open(output_filename, "w") as f:
    # Write the first line: n m c k
    f.write(f"{n} {m} {c} {k}\n")

    for _ in range(m):
        # 2. Force at least one element from the hitting set candidate in each subset.
        forced_element = random.choice(hitting_set_candidate)

        # 3. Choose the remaining (c-1) elements randomly from the universe.
        pool = list(range(0, n+1))
        pool.remove(forced_element)
        other_elements = random.sample(pool, c-1)

        subset = [forced_element] + other_elements

        # Shuffle the subset so the forced element is in a random position.
        random.shuffle(subset)

        # Ensure the first element is not 0
        if subset[0] == 0:
            for i in range(1, len(subset)):
                if subset[i] != 0:
                    subset[0], subset[i] = subset[i], subset[0]
                    break

        # Write the subset into the file.
        line = " ".join(map(str, subset))
        f.write(line + "\n")

print(f"Test input file '{output_filename}' generated with a guaranteed hitting set of size {k}.")
