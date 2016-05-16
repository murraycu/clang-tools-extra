//===--- ExplicitOperatorBoolCheck.cpp - clang-tidy------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "ExplicitOperatorBoolCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace modernize {

void ExplicitOperatorBoolCheck::registerMatchers(MatchFinder *Finder) {
  // Use of the explicit keyword with operator overloads requires C++11 or
  // later.
  if (!getLangOpts().CPlusPlus11)
    return;

  Finder->addMatcher(
      cxxConversionDecl(returns(booleanType()), unless(isExplicit()))
          .bind("operator-bool"),
      this);
}

void ExplicitOperatorBoolCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl =
      Result.Nodes.getNodeAs<CXXConversionDecl>("operator-bool");
  diag(MatchedDecl->getLocation(), "operator bool declaration is not explicit")
      << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "explicit ");
}

} // namespace modernize
} // namespace tidy
} // namespace clang
