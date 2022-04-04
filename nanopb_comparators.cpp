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

#include "nanopb_comparators.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CppUTest/UtestMacros.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
#include "pb_encode.h"
}

typedef struct {
  void *ptr;
  //! Size of memory pointed to by ptr
  size_t size;
} sFatPointer;

static sFatPointer prv_encode_pb_dynamic(const void *pb, const pb_msgdesc_t *fields) {
  size_t encoded_size = 0;
  bool ret = pb_get_encoded_size(&encoded_size, fields, pb);
  CHECK_TRUE_TEXT(ret, "Failed to calculate protobuf size");

  void *buf = malloc(encoded_size);
  CHECK_TRUE_TEXT(buf != NULL, "Out of memory during protobuf encoding");

  pb_ostream_t stream = pb_ostream_from_buffer((uint8_t*)buf, encoded_size);
  ret = pb_encode(&stream, fields, pb);
  CHECK_TRUE_TEXT(ret, "Failed to encode protobuf");

  sFatPointer ptr = {
    .ptr = buf,
    .size = encoded_size,
  };
  return ptr;
}

NanopbStructComparator::NanopbStructComparator(const pb_msgdesc_t *fields) {
  this->fields = fields;
}

bool NanopbStructComparator::isEqual(const void *object1, const void *object2) {
  sFatPointer expected_encoded = prv_encode_pb_dynamic(object1, this->fields);
  sFatPointer actual_encoded = prv_encode_pb_dynamic(object2, this->fields);

  bool is_equal = (expected_encoded.size == actual_encoded.size) &&
                  !memcmp(expected_encoded.ptr, actual_encoded.ptr, expected_encoded.size);

  free(expected_encoded.ptr);
  free(actual_encoded.ptr);
  return is_equal;
}

SimpleString NanopbStructComparator::valueToString(const void *object) {
    return StringFrom("Protobuf(opaque)");
}

