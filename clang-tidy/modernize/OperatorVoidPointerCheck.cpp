//===--- OperatorVoidPointerCheck.cpp - clang-tidy------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "OperatorVoidPointerCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace modernize {

void OperatorVoidPointerCheck::registerMatchers(MatchFinder *Finder) {
  // Use of the explicit keyword with operator overloads requires C++11 or
  // later.
  if (!getLangOpts().CPlusPlus11)
    return;

  Finder->addMatcher(cxxConversionDecl(returns(pointerType(pointee(
                                           isConstQualified(), voidType()))),
                                       unless(isExplicit()))
                         .bind("operator-void-pointer"),
                     this);
}

void OperatorVoidPointerCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl =
      Result.Nodes.getNodeAs<CXXConversionDecl>("operator-void-pointer");

  diag(MatchedDecl->getLocation(), "implicit operator void* declaration should "
                                   "probably be explicit operator bool");

  // FIXME: This tries to change the type and add explicit, but
  // MatchedDecl->getTypeSpecStartLoc() gets the start of void, not the start
  // of const, in const void*. We would need something like
  // getConversionTypeStartLoc().
  // const auto Type = MatchedDecl->getConversionType().
  // const auto OldRange = CharSourceRange::getTokenRange(
  //  MatchedDecl->getTypeSpecStartLoc(),
  //  MatchedDecl->getTypeSpecStartLoc().getLocWithOffset(Type.getAsString().size()));
  // diag(MatchedDecl->getLocation(),
  //  "implicit operator void* declaration should probably be explicit operator
  //  bool")
  //  << FixItHint::CreateReplacement(OldRange, "bool")
  //  << FixItHint::CreateInsertion(MatchedDecl->getLocStart(), "explicit ");
}

} // namespace modernize
} // namespace tidy
} // namespace clang
