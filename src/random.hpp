#pragma once

#include <random>

/**
 * output range is [0, UINT64_MAX]
 * including both 0 and UINT64_MAX
 */
inline uint64_t randomBits() {
  thread_local std::random_device rd;
  thread_local std::mt19937_64 gen(rd());
  thread_local std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
  return dis(gen);
}

/**
 * output range is [0.0, 1.0)
 * including 0.0 but excluding 1.0
 */
inline double randomUniform() {
  thread_local std::random_device rd;
  thread_local std::mt19937_64 gen(rd());
  thread_local std::uniform_real_distribution<double> dis(0.0, 1.0);
  return dis(gen);
}

/**
 * output range is [0, length)
 * including 0 but excluding length
 * length must not be zero
 */
inline size_t randomIndex(size_t length) {
  return (size_t) floor(randomUniform() * length);
}

/**
 * output range is [-1.0, +1.0]
 * including both -1.0 and +1.0
 */
inline double randomBetweenMinusOneAndPlusOne() {
  thread_local std::random_device rd;
  thread_local std::mt19937_64 gen(rd());
  thread_local std::uniform_real_distribution<double> dis(-1.0, nextafter(1.0, 2.0));
  return dis(gen);
}