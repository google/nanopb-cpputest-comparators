# nanopb-cpputest-comparators

The [CppUTest unit test and mocking framework](https://cpputest.github.io/mocking_manual.html) has
the concept of "comparators". [Comparators](https://cpputest.github.io/mocking_manual.html#objects_as_parameters)
are used to compare two different C/C++ objects.

This repository contains a CppUTest comparator that compares two different [nanopb](https://github.com/nanopb/nanopb)
C structs by serializing them and comparing the binary output. This can be used to help unit test
C/C++ code that serializes or deserializes protobufs using nanopb.

## Example usage

See the [CppUTest mocking framework docs](https://cpputest.github.io/mocking_manual.html) for more
details.

```cpp
// my_fields are the pb_msgdesc_t array found in nanopb's auto-generated header file
static NanopbStructComparator my_comparator = NanopbStructComparator(my_fields);

TEST_GROUP(MY_TEST_GROUP) {
  void setup(void) {
    mock().installComparator("my_protobuf_type", my_comparator);
  }

  void teardown(void) {
    mock().clear();
    mock().removeAllComparatorsAndCopiers();
  }
};

TEST(MY_TEST_GROUP, my_test) {
  const my_generated_nanopb_struct expected = {
    // some data
  }

  mock().
    .expectOneCall("some_function")
    .withParameterOfType("my_protobuf_type", "some_argument", &expected);
}
```
