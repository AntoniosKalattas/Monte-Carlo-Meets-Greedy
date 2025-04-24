#!/usr/bin/env python3
import sys

def read_subsets(file_path):
    """
    Read subsets from a file.
    Each non-empty, non-comment line should contain integers separated by whitespace.
    Returns a list of sets of ints.
    """
    subsets = []
    with open(file_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            subset = set(map(int, line.split()))
            subsets.append(subset)
    return subsets

def is_hitting_set(candidate_set, subsets):
    """
    Check if `candidate_set` is a hitting set for the list of `subsets`.
    Returns True if every subset has at least one element in common with candidate_set.
    """
    candidate = set(candidate_set)
    for subset in subsets:
        if candidate.isdisjoint(subset):
            return False
    return True

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <subsets_file> <elem1> [elem2 ...]")
        sys.exit(1)

    file_path = sys.argv[1]
    try:
        subsets = read_subsets(file_path)
    except Exception as e:
        print(f"Error reading '{file_path}': {e}", file=sys.stderr)
        sys.exit(1)

    try:
        candidate = set(map(int, sys.argv[2:]))
    except ValueError:
        print("All elements of the candidate set must be integers.", file=sys.stderr)
        sys.exit(1)

    if is_hitting_set(candidate, subsets):
        print("This IS a hitting set.")
    else:
        print("This is NOT a hitting set.")

if __name__ == "__main__":
    main()
