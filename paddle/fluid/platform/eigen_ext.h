// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "paddle/fluid/platform/bfloat16.h"
#include "paddle/fluid/platform/complex128.h"
#include "paddle/fluid/platform/complex64.h"
#include "paddle/fluid/platform/hostdevice.h"

#include "unsupported/Eigen/CXX11/Tensor"

namespace Eigen {

using bfloat16 = paddle::platform::bfloat16;
using complex64 = paddle::platform::complex64;
using complex128 = paddle::platform::complex128;

template <typename T>
struct NumTraits;

template <>
struct NumTraits<bfloat16> : GenericNumTraits<bfloat16> {
  enum {
    IsSigned = true,
    IsInteger = false,
    IsComplex = false,
    RequireInitialization = false
  };

  HOSTDEVICE static inline bfloat16 epsilon() {
    return paddle::platform::raw_uint16_to_bfloat16(0x3400);
  }
  HOSTDEVICE static inline bfloat16 dummy_precision() {
    return bfloat16(1e-5f);
  }
  HOSTDEVICE static inline bfloat16 highest() {
    return paddle::platform::raw_uint16_to_bfloat16(0x7f7f);
  }
  HOSTDEVICE static inline bfloat16 lowest() {
    return paddle::platform::raw_uint16_to_bfloat16(0xff7f);
  }
  HOSTDEVICE static inline bfloat16 infinity() {
    return paddle::platform::raw_uint16_to_bfloat16(0x7f80);
  }
  HOSTDEVICE static inline bfloat16 quiet_NaN() {
    return paddle::platform::raw_uint16_to_bfloat16(0xffc1);
  }
};

template <>
struct NumTraits<complex64> : GenericNumTraits<std::complex<float>> {
  typedef float Real;
  typedef typename NumTraits<float>::Literal Literal;
  enum {
    IsComplex = 1,
    RequireInitialization = NumTraits<float>::RequireInitialization,
    ReadCost = 2 * NumTraits<float>::ReadCost,
    AddCost = 2 * NumTraits<Real>::AddCost,
    MulCost = 4 * NumTraits<Real>::MulCost + 2 * NumTraits<Real>::AddCost
  };

  EIGEN_DEVICE_FUNC
  static inline Real epsilon() { return NumTraits<Real>::epsilon(); }
  EIGEN_DEVICE_FUNC
  static inline Real dummy_precision() {
    return NumTraits<Real>::dummy_precision();
  }
  EIGEN_DEVICE_FUNC
  static inline int digits10() { return NumTraits<Real>::digits10(); }
};

template <>
struct NumTraits<complex128> : GenericNumTraits<std::complex<double>> {
  typedef double Real;
  typedef typename NumTraits<double>::Literal Literal;
  enum {
    IsComplex = 1,
    RequireInitialization = NumTraits<double>::RequireInitialization,
    ReadCost = 2 * NumTraits<double>::ReadCost,
    AddCost = 2 * NumTraits<Real>::AddCost,
    MulCost = 4 * NumTraits<Real>::MulCost + 2 * NumTraits<Real>::AddCost
  };

  EIGEN_DEVICE_FUNC
  static inline Real epsilon() { return NumTraits<Real>::epsilon(); }
  EIGEN_DEVICE_FUNC
  static inline Real dummy_precision() {
    return NumTraits<Real>::dummy_precision();
  }
  EIGEN_DEVICE_FUNC
  static inline int digits10() { return NumTraits<Real>::digits10(); }
};

namespace numext {

//////////// bfloat methods /////////////

template <>
HOSTDEVICE inline bool(isnan)(const bfloat16& a) {
  return (paddle::platform::isnan)(a);
}

template <>
HOSTDEVICE inline bool(isinf)(const bfloat16& a) {
  return (paddle::platform::isinf)(a);
}

template <>
HOSTDEVICE inline bool(isfinite)(const bfloat16& a) {
  return (paddle::platform::isfinite)(a);
}

template <>
HOSTDEVICE inline bfloat16 exp(const bfloat16& a) {
  return bfloat16(::expf(static_cast<float>(a)));
}

template <>
HOSTDEVICE inline bfloat16 erf(const bfloat16& a) {
  return bfloat16(::erff(static_cast<float>(a)));
}

template <>
HOSTDEVICE inline bfloat16 log(const bfloat16& a) {
  return bfloat16(::logf(static_cast<float>(a)));
}

template <>
HOSTDEVICE inline bfloat16 tanh(const bfloat16& a) {
  return bfloat16(::tanhf(static_cast<float>(a)));
}

template <>
HOSTDEVICE inline bfloat16 sqrt(const bfloat16& a) {
  return bfloat16(::sqrtf(static_cast<float>(a)));
}

template <>
HOSTDEVICE inline bfloat16 ceil(const bfloat16& a) {
  return bfloat16(::ceilf(static_cast<float>(a)));
}

template <>
HOSTDEVICE inline bfloat16 floor(const bfloat16& a) {
  return bfloat16(::floorf(static_cast<float>(a)));
}

template <>
HOSTDEVICE inline bfloat16 round(const bfloat16& a) {
  return bfloat16(::roundf(static_cast<float>(a)));
}

template <>
HOSTDEVICE inline bfloat16 pow(const bfloat16& a, const bfloat16& b) {
  return bfloat16(::powf(static_cast<float>(a), static_cast<float>(b)));
}

template <>
HOSTDEVICE inline bfloat16 abs(const bfloat16& a) {
  return bfloat16(::fabs(static_cast<float>(a)));
}

//////////// complex64 methods /////////////

template <>
HOSTDEVICE inline bool(isnan)(const complex64& a) {
  return (paddle::platform::isnan)(a);
}

template <>
HOSTDEVICE inline bool(isinf)(const complex64& a) {
  return (paddle::platform::isinf)(a);
}

template <>
HOSTDEVICE inline bool(isfinite)(const complex64& a) {
  return (paddle::platform::isfinite)(a);
}

template <>
HOSTDEVICE inline complex64 exp(const complex64& a) {
  float com = ::expf(a.real);
  float res_real = com * ::cosf(a.imag);
  float res_imag = com * ::sinf(a.imag);
  return complex64(res_real, res_imag);
}

template <>
HOSTDEVICE inline complex64 log(const complex64& a) {
  return paddle::platform::log(a);
}

template <>
HOSTDEVICE inline complex64 tanh(const complex64& a) {
  return paddle::platform::tanh(a);
}

template <>
HOSTDEVICE inline complex64 sqrt(const complex64& a) {
  return paddle::platform::sqrt(a);
}

template <>
HOSTDEVICE inline complex64 ceil(const complex64& a) {
  return complex64(::ceilf(a.real), ::ceilf(a.imag));
}

template <>
HOSTDEVICE inline complex64 floor(const complex64& a) {
  return complex64(::floorf(a.real), ::floor(a.imag));
}

template <>
HOSTDEVICE inline complex64 round(const complex64& a) {
  return complex64(::roundf(a.real), ::roundf(a.imag));
}

template <>
HOSTDEVICE inline complex64 pow(const complex64& a, const complex64& b) {
  return paddle::platform::pow(a, b);
}

template <>
HOSTDEVICE inline float abs(const complex64& a) {
  return paddle::platform::abs(a);
}

//////////// complex128 methods /////////////

template <>
HOSTDEVICE inline bool(isnan)(const complex128& a) {
  return (paddle::platform::isnan)(a);
}

template <>
HOSTDEVICE inline bool(isinf)(const complex128& a) {
  return (paddle::platform::isinf)(a);
}

template <>
HOSTDEVICE inline bool(isfinite)(const complex128& a) {
  return (paddle::platform::isfinite)(a);
}

template <>
HOSTDEVICE inline complex128 exp(const complex128& a) {
  double com = ::expf(a.real);
  double res_real = com * ::cosf(a.imag);
  double res_imag = com * ::sinf(a.imag);
  return complex128(res_real, res_imag);
}

template <>
HOSTDEVICE inline complex128 log(const complex128& a) {
  return paddle::platform::log(a);
}

template <>
HOSTDEVICE inline complex128 tanh(const complex128& a) {
  return paddle::platform::tanh(a);
}

template <>
HOSTDEVICE inline complex128 sqrt(const complex128& a) {
  return paddle::platform::sqrt(a);
}

template <>
HOSTDEVICE inline complex128 ceil(const complex128& a) {
  return complex128(::ceilf(a.real), ::ceilf(a.imag));
}

template <>
HOSTDEVICE inline complex128 floor(const complex128& a) {
  return complex128(::floorf(a.real), ::floor(a.imag));
}

template <>
HOSTDEVICE inline complex128 round(const complex128& a) {
  return complex128(::roundf(a.real), ::roundf(a.imag));
}

template <>
HOSTDEVICE inline complex128 pow(const complex128& a, const complex128& b) {
  return paddle::platform::pow(a, b);
}

template <>
HOSTDEVICE inline double abs(const complex128& a) {
  return paddle::platform::abs(a);
}

}  // namespace numext
}  // namespace Eigen
