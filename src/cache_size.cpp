// Created by SwiftWare Lab on 9/25.
// CE 4SP4 - High Performance Programming
// Copyright (c) 2025 SwiftWare Lab
// Distribution of the code is not
// allowed in any form without permission
// from SwiftWare Lab.

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "cache_size.h"

namespace swiftware::hpp
{

  // ------------------------------------------------------------------
  // ADDED FUNCTION FOR TASK 1
  // Performs repeated copy operations to stress caches
  // A: pointer to an array of size m
  // m: array length
  // REP: # of repetitions
  // ------------------------------------------------------------------
  void copy_func_2(double *A, int m, int REP)
  {
    double *B = static_cast<double*>(std::malloc(m * sizeof(double)));
    if (!B) {
      std::cerr << "Allocation failed\n";
      return;
    }

    // repeat copy to get stable timing
    for (int r = 0; r < REP; ++r) {
      for (int i = 0; i < m; ++i) {
        B[i] = A[i];
      }
    }

    std::free(B);
  }

}