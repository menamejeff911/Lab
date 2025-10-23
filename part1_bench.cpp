// Created by SwiftWare Lab on 9/25.
// CE 4SP4 - High Performance Programming
// Copyright (c) 2025 SwiftWare Lab
// Distribution of the code is not
// allowed in any form without permission
// from SwiftWare Lab.


#include "benchmark/benchmark.h"
#include "cache_size.h"
#include <iostream>



static void BM_COPY(benchmark::State &state)
{
    const int m = static_cast<int>(state.range(0));
    const int REP = 100;

    double *A = static_cast<double*>(std::malloc(m * sizeof(double)));
    if (!A) {
        state.SkipWithError("Memory allocation failed");
        return;
    }

    for (int i = 0; i < m; ++i)
        A[i] = static_cast<double>(i);

    for (auto _ : state) {
        swiftware::hpp::copy_func_2(A, m, REP);
        benchmark::ClobberMemory();
    }

    std::free(A);
}


#define MAXSIZE 131072

BENCHMARK(BM_COPY)->ArgsProduct({benchmark::CreateRange(1024, MAXSIZE, /*multi=*/2), {1}})
    ->Unit(benchmark::kMicrosecond)->Iterations(1)->Repetitions(3);



BENCHMARK_MAIN();
