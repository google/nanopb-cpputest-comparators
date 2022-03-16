// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once

#include "CppUTestExt/MockSupport.h"

extern "C" {
#include "pb_encode.h"
}

//! @class NanopbStructComparator
//! @brief Generic comparator for nanopb C structs that represent unserialized protobuf messages.
//! It compares the serialized binary data represented by these structs to determine equality.
//! @code{.cpp}
//! // Example usage
//! // my_fields are the pb_msgdesc_t array found in nanopb's auto-generated header file
//! static NanopbStructComparator my_comparator = NanopbStructComparator(my_fields);
//! @endcode
class NanopbStructComparator : public MockNamedValueComparator {
 public:
  NanopbStructComparator(const pb_msgdesc_t *fields);
  virtual bool isEqual(const void *object1, const void *object2);
  virtual SimpleString valueToString(const void *object);

 private:
  const pb_msgdesc_t *fields;
};
