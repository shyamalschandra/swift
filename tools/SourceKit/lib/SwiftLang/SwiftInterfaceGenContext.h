//===--- SwiftInterfaceGenContext.h - ----------------------------*- C++ -*-==//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SOURCEKIT_LIB_SWIFTLANG_SWIFTINTERFACEGENCONTEXT_H
#define LLVM_SOURCEKIT_LIB_SWIFTLANG_SWIFTINTERFACEGENCONTEXT_H

#include "SourceKit/Core/LLVM.h"
#include "swift/AST/Module.h"
#include "swift/Basic/ThreadSafeRefCounted.h"
#include <string>

namespace swift {
  class CompilerInvocation;
  class ValueDecl;
}

namespace SourceKit {
  class EditorConsumer;
  class SwiftInterfaceGenContext;
  typedef RefPtr<SwiftInterfaceGenContext> SwiftInterfaceGenContextRef;
  class ASTUnit;
  typedef IntrusiveRefCntPtr<ASTUnit> ASTUnitRef;

class SwiftInterfaceGenContext :
		public swift::ThreadSafeRefCountedBase<SwiftInterfaceGenContext> {
public:
  static SwiftInterfaceGenContextRef create(StringRef DocumentName,
                                            bool IsModule,
                                            StringRef ModuleOrHeaderName,
                                            swift::CompilerInvocation Invocation,
                                            std::string &ErrorMsg);

  static SwiftInterfaceGenContextRef createForSwiftSource(StringRef DocumentName,
                                                          StringRef SourceFileName,
                                                          ASTUnitRef AstUnit,
                                                          std::string &ErrMsg);

  ~SwiftInterfaceGenContext();

  StringRef getDocumentName() const;
  StringRef getModuleOrHeaderName() const;
  bool isModule() const;

  bool matches(StringRef ModuleName, const swift::CompilerInvocation &Invok);

  void reportEditorInfo(EditorConsumer &Consumer) const;

  struct ResolvedEntity {
    const swift::ValueDecl *Dcl = nullptr;
    swift::ModuleEntity Mod;
    bool IsRef = false;

    ResolvedEntity() = default;
    ResolvedEntity(const swift::ValueDecl *Dcl, bool IsRef)
      : Dcl(Dcl), IsRef(IsRef) {}
    ResolvedEntity(const swift::ModuleEntity Mod, bool IsRef)
      : Mod(Mod), IsRef(IsRef) {}

    bool isResolved() const { return Dcl || Mod; }
  };

  /// Returns the resolved entity along with a boolean indicating if it is a
  /// reference or not.
  ResolvedEntity resolveEntityForOffset(unsigned Offset) const;

  /// Searches for a declaration with the given USR and returns the
  /// (offset,length) pair into the interface source if it finds one.
  llvm::Optional<std::pair<unsigned, unsigned>> findUSRRange(StringRef USR) const;

  void applyTo(swift::CompilerInvocation &CompInvok) const;

  class Implementation;

private:
  Implementation &Impl;

  SwiftInterfaceGenContext();
};

} // namespace SourceKit.

#endif
