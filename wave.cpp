
/* solving the transfer equation du / dt + gamma * du / dx = 0, where 'd' == '\partitial' */
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdio.h>

const double T = 100, H = 100; // calculation time, size of calculation area
const double tau = 0.01, h = 0.1; // step by time and grid.
const double v = 0.1;
const int N_grid_cells = static_cast<int>(H / h);
const int step = 100;

double u_init(int k) {
  const int a = 10;
  return std::exp(-(k - a) * (k - a));
}

int main() {
  double gamma = v * tau / h;
  double u_curr[N_grid_cells], u_next[N_grid_cells];

  std::ofstream out;
  char filename[100];

  for (int k = 0; k * h < H; k++) {
    u_curr[k] = u_init(k);
  }

  for(int i = 0; i * tau  < T; i++) {
    u_next[0] = u_curr[0];
    for(int k = 0; k * h < H; k++) {
      u_next[k] = u_curr[k] - gamma * (u_curr[k] - u_curr[k - 1]);
    }
    if (i % step == 0) {
      sprintf(filename, "data/data_%d.txt", i / step);
      out.open(filename);
      for (int k = 0; k * h < H; k++) {
        out << k * h << ' ' <<  u_curr[k] << std::endl;
      }
      out.close();
    }
    std::swap(u_curr, u_next);
  }
}
