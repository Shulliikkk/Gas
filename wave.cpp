#include <iostream>
#include <fstream>
#include <cmath>

const int step = 10;

const int width = 30;
const int center = 100;
const int p_0 = 4;

const int T = 2000, K = 300, P = 11, P_0 = 5;
const int SIZE = P * K;

int index(int k, int p) {
  return (p + P_0) * K + k;
}

double get_gamma(int p) {
  return p * 1.0 / P;
}

double initial(int k) {
  return std::exp(-1.0 * ((k - center) * (k - center) / width / width));
}

void set_initial_values(double* data) {
  for (int k = 0; k < K; k++) {
    data[index(k, p_0)] = initial(k);
  }
}

void dump_concentration(double* data, int i) {
  std::ofstream out;
  char filename[100];
  sprintf(filename, "data/data_%d.txt", i);
  out.open(filename);
  for (int k = 0; k < K; k++) {
    double total = 0;
    for (int p = -P_0; p <= P_0; p++) {
      total += data[index(k, p)];
    }
    out << total << std::endl;
  }
  out.close();
}

void make_iteration(double* next, double* data) {
  double gamma;
  for (int p = -P_0; p <= P_0; p++) {
    gamma = get_gamma(p);
    if (gamma > 0) {
      for (int k = 1; k < K; k++) {
        next[index(k, p)] = data[index(k, p)] - gamma * (data[index(k, p)] - data[index(k - 1, p)]);
      }
    }
    else {
      for (int k = 0; k < K - 1; k++) {
        next[index(k, p)] = data[index(k, p)] - gamma * (data[index(k + 1, p)] - data[index(k, p)]);
      }
    }
  }
  double nom_left = 0, nom_right, denom = 0;
  for (int p = -P_0; p <= P_0; p++) {
    gamma = get_gamma(p);
    if (gamma > 0) {
      nom_right += next[index(K - 1, p)] * gamma;
    }
    else {
      nom_left += next[index(0, p)] * gamma;
    }
    if (gamma > 0) {
      denom += gamma * std::exp(-gamma * gamma / 2.0);
    }
  }
  for (int p = -P_0; p <= P_0; p++) {
    gamma = get_gamma(p);
    if (gamma > 0) {
      next[index(0, p)] = -nom_left / denom * std::exp(-gamma * gamma / 2.0);
    }
    else {
      next[index(K - 1, p)] = -nom_right / (-denom) * std::exp(-gamma * gamma / 2.0);
    }
  }
}

int main() {
  double data[SIZE], next[SIZE];
  set_initial_values(data);
  for (int i = 0; i < T; i++) {
    make_iteration(next, data);
    std::swap(data, next);
    if (i % step == 0) {
      dump_concentration(data, i);
    }
  }
}
