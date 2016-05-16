// RUN: %check_clang_tidy %s modernize-explicit-operator-bool %t -- -- -std=c++11

// This should trigger the check:
class SomethingBad {
  operator bool() const {
    // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: operator bool declaration is not explicit [modernize-explicit-operator-bool]
    return something != 0;
  }

  int something = 0;
};

class SomethingGood {
  explicit operator bool() const {
    return something != 0;
  }

  int something = 0;
};
