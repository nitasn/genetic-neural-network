#pragma once

#include <random>

/**
 * output range is [0, UINT64_MAX]
 */
inline uint64_t randomBits() {
  thread_local std::random_device rd;
  thread_local std::mt19937_64 gen(rd());
  thread_local std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
  return dis(gen);
}

/**
 * output range is [0.0, 1.0)
 */
inline double randomUniform() {
  thread_local std::random_device rd;
  thread_local std::mt19937_64 gen(rd());
  thread_local std::uniform_real_distribution<double> dis(0.0, 1.0);
  return dis(gen);
}

/**
 * output range is [0, length)
 * length must not be zero
 */
inline size_t randomIndex(size_t length) {
  return (size_t) floor(randomUniform() * length);
}

/**
 * output range is [-1.0, +1.0]
 */
inline double randomSigned() {
  thread_local std::random_device rd;
  thread_local std::mt19937_64 gen(rd());
  thread_local std::uniform_real_distribution<double> dis(-1.0, nextafter(1.0, 2.0));
  return dis(gen);
}

/**
 * normal distribution with mean = 0.0, stdev = 1.0.
 */
inline double randomNormal() {
  thread_local std::random_device rd;
  thread_local std::mt19937_64 gen(rd());
  thread_local std::normal_distribution<double> dis(0.0, 1.0);
  return dis(gen);
}