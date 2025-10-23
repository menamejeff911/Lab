// Created by SwiftWare Lab on 9/25.
// CE 4SP4 - High Performance Programming
// Copyright (c) 2025 SwiftWare Lab
// Distribution of the code is not
// allowed in any form without permission
// from SwiftWare Lab.



#include "gtest/gtest.h"
#include "heat.h"
#include <vector>



//TEST(HeatTest, BasicAssertions) {
//ASSERT_TRUE(false);
//}

// Small helper for indexing
static inline int idx(const field& f, int i, int j) {
  return i * (f.ny + 2) + j;
}

// Make a constant field with a specific value everywhere (including ghosts)
static void make_constant_field(field* f, int nx, int ny, double value) {
  field_set_size(f, nx, ny);
  field_allocate(f);
  for (int i = 0; i < f->nx + 2; ++i) {
    for (int j = 0; j < f->ny + 2; ++j) {
      f->data[idx(*f, i, j)] = value;
    }
  }
}

// 1) Constant field should remain constant after one evolve step
TEST(HeatTest, ConstantFieldNoChange) {
  field prev, curr;
  const int nx = 8, ny = 8;
  const double a = 0.5;
  // Stable time step using the same formula as the benchmark
  const double dx2 = DX * DX, dy2 = DY * DY;
  const double dt = dx2 * dy2 / (2.0 * a * (dx2 + dy2));

  // Set everything (including ghosts) to a constant value
  make_constant_field(&prev, nx, ny, 42.0);
  field_set_size(&curr, nx, ny);
  field_allocate(&curr);
  field_copy(&prev, &curr); // ensure boundaries in curr are identical to prev

  // Evolve once
  evolve(&curr, &prev, a, dt);

  // All interior points should remain exactly 42.0
  for (int i = 1; i <= nx; ++i) {
    for (int j = 1; j <= ny; ++j) {
      ASSERT_DOUBLE_EQ(curr.data[idx(curr, i, j)], 42.0);
    }
  }
}

// 2) Boundaries must remain unchanged by evolve
TEST(HeatTest, BoundariesUnchangedAfterEvolve) {
  field prev, curr;
  const int nx = 8, ny = 8;
  const double a = 0.5;
  const double dx2 = DX * DX, dy2 = DY * DY;
  const double dt = dx2 * dy2 / (2.0 * a * (dx2 + dy2));

  // Initialize prev with some known values: interior 10.0, ghosts different constants
  field_set_size(&prev, nx, ny);
  field_allocate(&prev);
  // interior
  for (int i = 1; i <= nx; ++i)
    for (int j = 1; j <= ny; ++j)
      prev.data[idx(prev, i, j)] = 10.0;
  // boundaries (ghost layers) - set rows first, then columns without overwriting corners
  for (int j = 0; j < ny + 2; ++j) {
    prev.data[idx(prev, 0, j)] = 77.0;         // top row
    prev.data[idx(prev, nx + 1, j)] = 55.0;    // bottom row
  }
  for (int i = 1; i <= nx; ++i) {               // only interior rows to avoid corner conflicts
    prev.data[idx(prev, i, 0)] = 11.0;         // left column
    prev.data[idx(prev, i, ny + 1)] = 99.0;    // right column
  }

  field_set_size(&curr, nx, ny);
  field_allocate(&curr);
  field_copy(&prev, &curr); // start curr as a copy, so boundaries are already correct

  evolve(&curr, &prev, a, dt);

  // Boundaries should be exactly preserved in curr
  for (int j = 0; j < ny + 2; ++j) {
    ASSERT_DOUBLE_EQ(curr.data[idx(curr, 0, j)], 77.0);
    ASSERT_DOUBLE_EQ(curr.data[idx(curr, nx + 1, j)], 55.0);
  }
  for (int i = 1; i <= nx; ++i) {  // only check interior rows for left/right columns
    ASSERT_DOUBLE_EQ(curr.data[idx(curr, i, 0)], 11.0);
    ASSERT_DOUBLE_EQ(curr.data[idx(curr, i, ny + 1)], 99.0);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}