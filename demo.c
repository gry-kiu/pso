/* An implementation of the Particle Swarm Optimization algorithm

   === DEMONSTRATION CODE ===

   Copyright 2010 Kyriakos Kentzoglanakis

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License version
   3 as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see
   <http://www.gnu.org/licenses/>.
*/

#include "pso.h"
#include <math.h>
#include <stdio.h> // for printf
#include <stdlib.h>
#include <string.h>

//==============================================================
//  BENCHMARK FUNCTIONS
//==============================================================

double pso_sphere(double *vec, int dim, void *params) {
  double sum = 0;
  int i;
  for (i = 0; i < dim; i++)
    sum += pow(vec[i], 2);
  return sum;
}

double pso_rosenbrock(double *vec, int dim, void *params) {
  double sum = 0;
  int i;
  for (i = 0; i < dim - 1; i++)
    sum += 100 * pow((vec[i + 1] - pow(vec[i], 2)), 2) + pow((1 - vec[i]), 2);
  return sum;
}

double pso_griewank(double *vec, int dim, void *params) {
  double sum = 0.;
  double prod = 1.;
  int i;
  for (i = 0; i < dim; i++) {
    sum += pow(vec[i], 2);
    prod *= cos(vec[i] / sqrt(i + 1));
  }
  return sum / 4000 - prod + 1;
}

//==============================================================

int main(int argc, char **argv) {
  pso_settings_t *settings = NULL;
  pso_obj_fun_t obj_fun = NULL;

  int dim = 30;
  double *range_lo = (double *)malloc(dim * sizeof(double));
  double *range_hi = (double *)malloc(dim * sizeof(double));

  // parse command line argument (function name)
  if (argc == 2) {
    if (strcmp(argv[1], "rosenbrock") == 0) {
      obj_fun = pso_rosenbrock;
      for (int i = 0; i < dim; i++) {
        range_lo[i] = -2.048;
        range_hi[i] = 2.048;
      }
      settings = pso_settings_new(dim, range_lo, range_hi);
      printf("Optimizing function: rosenbrock (dim=%d, swarm size=%d)\n", settings->dim, settings->size);
    } else if (strcmp(argv[1], "griewank") == 0) {
      obj_fun = pso_griewank;
      for (int i = 0; i < dim; i++) {
        range_lo[i] = -600;
        range_hi[i] = 600;
      }
      settings = pso_settings_new(dim, range_lo, range_hi);
      printf("Optimizing function: griewank (dim=%d, swarm size=%d)\n", settings->dim, settings->size);
    } else if (strcmp(argv[1], "sphere") == 0) {
      obj_fun = pso_sphere;
      for (int i = 0; i < dim; i++) {
        range_lo[i] = -100;
        range_hi[i] = 100;
      }
      settings = pso_settings_new(dim, range_lo, range_hi);
      printf("Optimizing function: sphere (dim=%d, swarm size=%d)\n", settings->dim, settings->size);
    } else {
      printf("Unsupported objective function: %s", argv[1]);
      return 1;
    }
  } else if (argc > 2) {
    printf("Usage: demo [PROBLEM], where problem is optional with values [sphere|rosenbrock|griewank]\n ");
    return 1;
  }

  // handle the default case (no argument given)
  if (obj_fun == NULL || settings == NULL) {
    obj_fun = pso_sphere;
    for (int i = 0; i < dim; i++) {
      range_lo[i] = -100;
      range_hi[i] = 100;
    }
    settings = pso_settings_new(dim, range_lo, range_hi);
    printf("Optimizing function: sphere (dim=%d, swarm size=%d)\n", settings->dim, settings->size);
  }

  // initialize GBEST solution
  pso_result_t solution;
  // allocate memory for the best position buffer
  solution.gbest = (double *)malloc(settings->dim * sizeof(double));

  // run optimization algorithm
  pso_solve(obj_fun, NULL, &solution, settings);

  // free the gbest buffer
  free(solution.gbest);

  // free the settings
  pso_settings_free(settings);

  free(range_lo);
  free(range_hi);

  return 0;
}
