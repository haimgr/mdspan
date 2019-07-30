
#pragma once

#include <benchmark/benchmark.h>

#include "fill.hpp"

//==============================================================================
// <editor-fold desc="A helpful template for instantiating all 3D combinations"> {{{1

#define MDSPAN_BENCHMARK_ALL_3D(bench_template, prefix, md_template, X, Y, Z) \
BENCHMARK_CAPTURE( \
  bench_template, prefix##fixed_##X##_##Y##_##Z, md_template<int, X, Y, Z>{} \
); \
BENCHMARK_CAPTURE( \
  bench_template, prefix##dyn_##X##_##Y##_d##Z, md_template<int, X, Y, std::dynamic_extent>{}, Z \
); \
BENCHMARK_CAPTURE( \
  bench_template, prefix##dyn_##X##_d##Y##_##Z, md_template<int, X, std::dynamic_extent, Z>{}, Y \
); \
BENCHMARK_CAPTURE( \
  bench_template, prefix##dyn_d##X##_##Y##_##Z, md_template<int, std::dynamic_extent, Y, Z>{}, X \
); \
BENCHMARK_CAPTURE( \
  bench_template, prefix##dyn_##X##_d##Y##_d##Z, md_template<int, X, std::dynamic_extent, std::dynamic_extent>{}, Y, Z \
); \
BENCHMARK_CAPTURE( \
  bench_template, prefix##dyn_d##X##_##Y##_d##Z, md_template<int, std::dynamic_extent, Y, std::dynamic_extent>{}, X, Z \
); \
BENCHMARK_CAPTURE( \
  bench_template, prefix##dyn_d##X##_d##Y##_##Z, md_template<int, std::dynamic_extent, std::dynamic_extent, Z>{}, X, Y \
); \
BENCHMARK_CAPTURE( \
  bench_template, prefix##dyn_d##X##_d##Y##_d##Z, md_template<int, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent>{}, X, Y, Z \
)

// </editor-fold> end A helpful template for instantiating all 3D combinations }}}1
//==============================================================================

template <class T, class Size>
void BM_Raw_Sum_1D(benchmark::State& state, T, Size size) {
  auto buffer = std::make_unique<T[]>(size);
  {
    // just for setup...
    auto wrapped = std::mdspan<T, std::dynamic_extent>{buffer.get(), size};
    mdspan_benchmark::fill_random(wrapped);
  }
  T* data = buffer.get();
  for (auto _ : state) {
    T sum = 0;
    for(ptrdiff_t i = 0; i < size; ++i) {
      sum += data[i];
    }
    benchmark::DoNotOptimize(sum);
    benchmark::DoNotOptimize(data);
  }
  state.SetBytesProcessed(size * sizeof(T) * state.iterations());
}

//==============================================================================

template <class T, class SizeX, class SizeY, class SizeZ>
void BM_Raw_Sum_3D_right(benchmark::State& state, T, SizeX x, SizeY y, SizeZ z) {
  auto buffer = std::make_unique<T[]>(x * y * z);
  {
    // just for setup...
    auto wrapped = std::mdspan<T, std::dynamic_extent>{buffer.get(), x*y*z};
    mdspan_benchmark::fill_random(wrapped);
  }
  T* data = buffer.get();
  for (auto _ : state) {
    T sum = 0;
    for(ptrdiff_t i = 0; i < x; ++i) {
      for(ptrdiff_t j = 0; j < y; ++j) {
        for(ptrdiff_t k = 0; k < z; ++k) {
          sum += data[k + j*z + i*z*y];
        }
      }
    }
    benchmark::DoNotOptimize(sum);
    benchmark::DoNotOptimize(data);
  }
  state.SetBytesProcessed(x * y * z * sizeof(T) * state.iterations());
}

//================================================================================

template <class T, class SizeX, class SizeY, class SizeZ>
void BM_Raw_Sum_3D_left(benchmark::State& state, T, SizeX x, SizeY y, SizeZ z) {
  auto buffer = std::make_unique<T[]>(x * y * z);
  {
    // just for setup...
    auto wrapped = std::mdspan<T, std::dynamic_extent>{buffer.get(), x*y*z};
    mdspan_benchmark::fill_random(wrapped);
  }
  T* data = buffer.get();
  for (auto _ : state) {
    T sum = 0;
    for(ptrdiff_t k = 0; k < z; ++k) {
      for(ptrdiff_t j = 0; j < y; ++j) {
        for(ptrdiff_t i = 0; i < x; ++i) {
          sum += data[i + j*x + k*x*y];
        }
      }
    }
    benchmark::DoNotOptimize(sum);
    benchmark::DoNotOptimize(data);
  }
  state.SetBytesProcessed(x * y * z * sizeof(T) * state.iterations());
}

//================================================================================

template <class T, class SizeX, class SizeY, class SizeZ>
void BM_Raw_Sum_3D_right_iter_left(benchmark::State& state, T, SizeX x, SizeY y, SizeZ z) {
  auto buffer = std::make_unique<T[]>(x * y * z);
  {
    // just for setup...
    auto wrapped = std::mdspan<T, std::dynamic_extent>{buffer.get(), x*y*z};
    mdspan_benchmark::fill_random(wrapped);
  }
  T* data = buffer.get();
  for (auto _ : state) {
    T sum = 0;
    for(ptrdiff_t k = 0; k < z; ++k) {
      for(ptrdiff_t j = 0; j < y; ++j) {
        for(ptrdiff_t i = 0; i < x; ++i) {
          sum += data[k + j*z + i*z*y];
        }
      }
    }
    benchmark::DoNotOptimize(sum);
    benchmark::DoNotOptimize(data);
  }
  state.SetBytesProcessed(x * y * z * sizeof(T) * state.iterations());
}

//================================================================================

//================================================================================

template <class T, ptrdiff_t x, ptrdiff_t y, ptrdiff_t z>
void BM_Raw_Static_Sum_3D_right(benchmark::State& state, T,
  std::integral_constant<ptrdiff_t, x>,
  std::integral_constant<ptrdiff_t, y>,
  std::integral_constant<ptrdiff_t, z>
) {
  auto buffer = std::make_unique<T[]>(x * y * z);
  {
    // just for setup...
    auto wrapped = std::mdspan<T, std::dynamic_extent>{buffer.get(), x*y*z};
    mdspan_benchmark::fill_random(wrapped);
  }
  T* data = buffer.get();
  for (auto _ : state) {
    T sum = 0;
    for(ptrdiff_t i = 0; i < x; ++i) {
      for(ptrdiff_t j = 0; j < y; ++j) {
        for(ptrdiff_t k = 0; k < z; ++k) {
          sum += data[k + j*z + i*z*y];
        }
      }
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetBytesProcessed(x * y * z * sizeof(T) * state.iterations());
}

//================================================================================

template <class T, ptrdiff_t x, ptrdiff_t y, ptrdiff_t z>
void BM_Raw_Static_Sum_3D_left(benchmark::State& state, T,
  std::integral_constant<ptrdiff_t, x>,
  std::integral_constant<ptrdiff_t, y>,
  std::integral_constant<ptrdiff_t, z>
) {
  auto buffer = std::make_unique<T[]>(x * y * z);
  {
    // just for setup...
    auto wrapped = std::mdspan<T, std::dynamic_extent>{buffer.get(), x*y*z};
    mdspan_benchmark::fill_random(wrapped);
  }
  T* data = buffer.get();
  for (auto _ : state) {
    T sum = 0;
    for(ptrdiff_t k = 0; k < z; ++k) {
      for(ptrdiff_t j = 0; j < y; ++j) {
        for(ptrdiff_t i = 0; i < x; ++i) {
          sum += data[i + j*x + k*x*y];
        }
      }
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetBytesProcessed(x * y * z * sizeof(T) * state.iterations());
}

//================================================================================

template <class T, ptrdiff_t x, ptrdiff_t y, ptrdiff_t z>
void BM_Raw_Static_Sum_3D_right_iter_left(benchmark::State& state, T,
  std::integral_constant<ptrdiff_t, x>,
  std::integral_constant<ptrdiff_t, y>,
  std::integral_constant<ptrdiff_t, z>
) {
  auto buffer = std::make_unique<T[]>(x * y * z);
  {
    // just for setup...
    auto wrapped = std::mdspan<T, std::dynamic_extent>{buffer.get(), x*y*z};
    mdspan_benchmark::fill_random(wrapped);
  }
  T* data = buffer.get();
  for (auto _ : state) {
    T sum = 0;
    for(ptrdiff_t k = 0; k < z; ++k) {
      for(ptrdiff_t j = 0; j < y; ++j) {
        for(ptrdiff_t i = 0; i < x; ++i) {
          sum += data[k + j*z + i*z*y];
        }
      }
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetBytesProcessed(x * y * z * sizeof(T) * state.iterations());
}
