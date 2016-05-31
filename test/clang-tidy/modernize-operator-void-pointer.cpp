// RUN: %check_clang_tidy %s modernize-operator-void-pointer %t -- -- -std=c++11

// This should trigger the check:
class SomethingBad {
  operator const void *() const {
    // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: implicit operator void* declaration should probably be explicit operator bool [modernize-operator-void-pointer]
    return reinterpret_cast<void *>(something != 0);
  }

  int something = 0;
};

class SomethingGood {
  //Note: Use modernize-explicit-operator-bool to check for implicit operator bool.
  explicit operator bool() const {
    return something != 0;
  }

  int something = 0;
};

class SomethingGoodExplicitConstVoidPtr {
  explicit operator const void *() const {
    return &something;
  }

  const int something = 0;
};

class SomethingGoodExplicitNonConstVoidPtr {
  explicit operator void *() {
    return &something;
  }

  int something = 0;
};

class SomethingGoodNonConstVoidPtr {
  // A non-const void* is unlikely to to be meant as operator bool before C++11
  // let us use explicit.
  operator void *() {
    return &something;
  }

  int something = 0;
};
