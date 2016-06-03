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

ExplicitOperatorBoolCheck::ExplicitOperatorBoolCheck(StringRef Name,
                                                     ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      WarnOnOperatorVoidPointer(Options.get("WarnOnOperatorVoidPointer", 1) !=
                                0) {}

void ExplicitOperatorBoolCheck::storeOptions(
    ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "WarnOnOperatorVoidPointer", WarnOnOperatorVoidPointer);
}

void ExplicitOperatorBoolCheck::registerMatchers(MatchFinder *Finder) {
  // Use of the explicit keyword with operator overloads requires C++11 or
  // later.
  if (!getLangOpts().CPlusPlus11)
    return;

  Finder->addMatcher(
      cxxConversionDecl(returns(booleanType()), unless(isExplicit()))
          .bind("operator-bool"),
      this);

  if (WarnOnOperatorVoidPointer) {
    Finder->addMatcher(cxxConversionDecl(returns(pointerType(pointee(
                                             isConstQualified(), voidType()))),
                                         unless(isExplicit()))
                           .bind("operator-void-pointer"),
                       this);
  }
}

void ExplicitOperatorBoolCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *MatchedDecl =
          Result.Nodes.getNodeAs<CXXConversionDecl>("operator-bool")) {
    diag(MatchedDecl->getLocation(),
         "operator bool declaration is not explicit")
        << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "explicit ");
  } else if (const auto *MatchedDecl =
                 Result.Nodes.getNodeAs<CXXConversionDecl>(
                     "operator-void-pointer")) {
    diag(MatchedDecl->getLocation(),
         "implicit operator const void* declaration should "
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
    //  "implicit operator void* declaration should probably be explicit
    //  operator
    //  bool")
    //  << FixItHint::CreateReplacement(OldRange, "bool")
    //  << FixItHint::CreateInsertion(MatchedDecl->getLocStart(), "explicit ");
  }
}

} // namespace modernize
} // namespace tidy
} // namespace clang
