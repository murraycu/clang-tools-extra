//===--- ExplicitOperatorBoolCheck.h - clang-tidy----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MODERNIZE_EXPLICIT_OPERATOR_BOOL_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MODERNIZE_EXPLICIT_OPERATOR_BOOL_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace modernize {

/// This check finds implicit operator bool overloads and inserts the explicit
/// keyword, which is available since C++11.
/// It also finds implicit operator const void* overloads, which should often be
/// explicit operator bool overloads.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/modernize-explicit-operator-bool.html
class ExplicitOperatorBoolCheck : public ClangTidyCheck {
public:
  ExplicitOperatorBoolCheck(StringRef Name, ClangTidyContext *Context);
  void storeOptions(ClangTidyOptions::OptionMap &Opt) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  const bool WarnOnOperatorVoidPointer;
};

} // namespace modernize
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MODERNIZE_EXPLICIT_OPERATOR_BOOL_H
