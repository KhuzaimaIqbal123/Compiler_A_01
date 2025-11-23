#ifndef TYPECHK_HPP
#define TYPECHK_HPP

#include "ast.hpp"
#include "scope.hpp"

enum class TypeChkError {
    NoError = 0,
    ErroneousVarDecl,
    FnCallParamCount,
    FnCallParamType,
    ErroneousReturnType,
    ExpressionTypeMismatch,
    ExpectedBooleanExpression,
    ErroneousBreak,
    NonBooleanCondStmt,
    EmptyExpression,
    AttemptedBoolOpOnNonBools,
    AttemptedBitOpOnNonNumeric,
    AttemptedShiftOnNonInt,
    AttemptedAddOpOnNonNumeric,
    AttemptedExponentiationOfNonNumeric,
    ReturnStmtNotFound
};

struct TypeChkReport {
    int error_count = 0;
};

// Run the type checker. Pass the global scope produced by analyze_scopes.
TypeChkReport type_check(Node* root, Scope* global);

#endif // TYPECHK_HPP
