// (c) 2023 ENCCS, CSC and the contributors

// Created by SwiftWare Lab on 9/25.
// CE 4SP4 - High Performance Programming
// Copyright (c) 2025 SwiftWare Lab
// Distribution of the code is not
// allowed in any form without permission
// from SwiftWare Lab.


#include "heat.h"

// Update the temperature values using five-point stencil
// Arguments:
//   curr: current temperature values
//   prev: temperature values from previous time step
//   a: diffusivity
//   dt: time step
void evolve(field *curr, field *prev, double a, double dt)
{
  //Get coefficients 
  double dx2 = prev->dx * prev->dx;
  double dy2 = prev->dy * prev->dy;

  for (int i = 1; i <= prev->nx; i++) {
    for (int j = 1; j <= prev->ny; j++) {

      // Compute the index in the 1D array (center point)
      int index = i * (prev->ny + 2) + j;

      // Get four neighbors 
      int up = (i - 1) * (prev->ny + 2) + j;
      int down = (i + 1) * (prev->ny + 2) + j;
      int left = i * (prev->ny + 2) + (j - 1);
      int right = i * (prev->ny + 2) + (j + 1);

      // Apply the five-point stencil formula
      curr->data[index] = prev->data[index] + a * dt * ((prev->data[up] - 2.0 * prev->data[index] + prev->data[down]) / dx2 + (prev->data[left] - 2.0 * prev->data[index] + prev->data[right]) / dy2);
    }
  }
}


void evolve_tiled(field *curr, field *prev, double a, double dt)
{
  //Get coefficients
  const double dx2 = prev->dx * prev->dx;
  const double dy2 = prev->dy * prev->dy;
  const int nx = prev->nx;
  const int ny = prev->ny;
  const int step = ny + 2;
  const int ti = 16; // Tile size in i direction
  const int tj = 16; // Tile size in j direction

  for (int x = 1; x <= nx; x += ti){
    const int iMax = (x + ti - 1 <= nx) ? (x + ti - 1) : nx;

    for (int y = 1; y <= ny; y += tj){
      const int jMax = (y + tj - 1 <= ny) ? (y + tj - 1) : ny;

      for (int i = x; i <= iMax; ++i){
        const int iUp = (i - 1) * step;
        const int iRow = i * step;
        const int iDown = (i + 1) * step;

        for (int j = y; j <= jMax; ++j){
          const int index = iRow + j;
          // Get four neighbors
          const int up = iUp + j;
          const int down = iDown + j;
          const int left = iRow + (j - 1);
          const int right = iRow + (j + 1);

          const double centerVal = prev->data[index];
          const double laplaceX = (prev->data[up] - 2.0 * centerVal + prev->data[down]) / dx2;
          const double laplaceY = (prev->data[left] - 2.0 * centerVal + prev->data[right]) / dy2;

          // Apply the five-point stencil formula
          curr->data[index] = centerVal + a * dt * (laplaceX + laplaceY);
        } 
      }
    }
  }
  
} 

