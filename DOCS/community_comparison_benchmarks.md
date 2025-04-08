# Community Comparison Benchmark Tests Documentation

## Overview

This document describes the benchmark tests implemented for validating the `CommunityComparison` class's Normalized Mutual Information (NMI) calculation. The tests use well-known examples from network science literature and synthetic test cases to ensure the implementation's correctness.

## Test Cases

### 1. Zachary's Karate Club Network

**Reference**: Newman, M. E. J., & Girvan, M. (2004). Finding and evaluating community structure in networks.

#### Description

The Zachary Karate Club is a classic social network dataset that represents friendships between 34 members of a karate club that split into two communities due to a conflict between the instructor and the club's president.

#### Test Implementation

- **True Communities**: Two groups based on the actual split
  - Officer's group (16 members)
  - Instructor's group (18 members)
- **Predicted Communities**: Slightly different split with one node (32) misclassified
- **Expected NMI**: 0.837 ± 0.1
- **Test Result**: PASSED (NMI ≈ 0.837)

#### Significance

This test validates our implementation against a real-world network with a known ground truth community structure. The high NMI value indicates good agreement between the predicted and actual communities.

### 2. LFR Benchmark Networks

**Reference**: Lancichinetti, A., & Fortunato, S. (2009). Benchmarks for testing community detection algorithms on directed and weighted graphs with overlapping communities.

#### Description

The Lancichinetti-Fortunato-Radicchi (LFR) benchmark generates synthetic networks with community structure, allowing controlled testing of community detection algorithms.

#### Test Implementation

- **Network Size**: 20 nodes
- **True Communities**: 3 communities of varying sizes
  - Community 1: 7 nodes
  - Community 2: 6 nodes
  - Community 3: 7 nodes
- **Predicted Communities**: Same structure with 2 misclassified nodes
  - Node 7 and Node 8 swapped between communities 1 and 2
  - Community 3 perfectly matched
- **Expected NMI**: 0.746 ± 0.1
- **Test Result**: PASSED (NMI ≈ 0.746)

#### Significance

This test validates the NMI calculation with controlled noise in the community assignments. The NMI value appropriately reflects the level of agreement between true and predicted communities when there are known misclassifications.

### 3. Perfect vs Random Split

#### Description

This test case validates the extremes of community detection: perfect matching and completely random assignment.

#### Test Implementation

- **Network Size**: 100 nodes
- **True Communities**: Two equal-sized communities (50 nodes each)
- **Test Scenarios**:
  1. Perfect Match
     - Predicted communities exactly match true communities
     - Expected NMI: 1.0
     - Test Result: PASSED (NMI = 1.000)
  2. Random Split
     - Nodes randomly assigned to two communities
     - Expected NMI: Close to 0
     - Test Result: PASSED (NMI = 0.000)

#### Significance

This test validates that our NMI implementation:

- Correctly identifies perfect community matches (NMI = 1.0)
- Appropriately handles completely uncorrelated assignments (NMI ≈ 0)
- Behaves as expected at the extremes of the measurement scale

## Implementation Details

### Helper Functions

```cpp
bool approxEqual(double a, double b, double epsilon = 0.001) {
    return std::abs(a - b) < epsilon;
}
```

This function allows for floating-point comparisons with a small tolerance, accounting for numerical precision issues in NMI calculations.

### Test Execution

The tests are compiled and run as part of the project's test suite:

```bash
make community_comparison_benchmark_test
./bin/community_comparison_benchmark_test
```

