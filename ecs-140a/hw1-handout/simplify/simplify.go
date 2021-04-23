package simplify

import (
	"hw1/expr"
)

// Simplify should return the simplified expresion
func Simplify(e expr.Expr, env expr.Env) expr.Expr {
	// TODO implement the simplify
	// panic("TODO: implement this!")

	// ------------------------- STUDENT'S ANSWER --------------------------- //

	// Source: https://tour.golang.org/methods/16
	switch e_copy := e.(type) {

	// Literal type
	case expr.Literal:
		return e

	// Var type
	case expr.Var:
		found := 0

		for i := range env {
			if i == e_copy {
				found = 1
				break
			}
		}

		if found == 1 {
			return expr.Literal(e.Eval(env))
		}

		return e

	// Unary type
	// NOTE: Node need to use expr.Unary to convert +X to X, simply (+X)
	case expr.Unary:
		e_copy.X = Simplify(e_copy.X, env)

		_, found := e_copy.X.(expr.Literal)

		if found {
			return expr.Literal(e_copy.Eval(env))
		}

		return e_copy

	// Binary type
	case expr.Binary:
		e_copy.X = Simplify(e_copy.X, env)
		e_copy.Y = Simplify(e_copy.Y, env)

		// Case 1: Both operands are literal --> evaluate and return literal()
		_, foundX := e_copy.X.(expr.Literal)
		_, foundY := e_copy.Y.(expr.Literal)

		if foundX && foundY {
			return expr.Literal(e_copy.Eval(env))
		}

		// Case 2: Both operands are not literal --> return e type
		if !foundX && !foundY {
			return e_copy
		}

		// Case 3: One of the operand is 0 (for add, sub, mult)
		// or 1 (for mult, div)

		// Case 3a: X is not literal, but Y is
		if !foundX {
			if e_copy.Y.Eval(env) == 0 {
				if e_copy.Op == '+' || e_copy.Op == '-' {
					return e_copy.X
				} else if e_copy.Op == '*' {
					return expr.Literal(0)
				}
			} else if e_copy.Y.Eval(env) == 1 {
				if e_copy.Op == '*' || e_copy.Op == '/' {
					return e_copy.X
				}
			}
		}

		// Case 3b: Y is not literal, but X is
		if !foundY {
			if e_copy.X.Eval(env) == 0 {
				if e_copy.Op == '+' {
					return e_copy.Y
				} else if e_copy.Op == '-' {
					return expr.Unary{Op: e_copy.Op, X: e_copy.Y}
				} else if e_copy.Op == '*' || e_copy.Op == '/' {
					return expr.Literal(0)
				}
			} else if e_copy.X.Eval(env) == 1 {
				if e_copy.Op == '*' {
					return e_copy.Y
				}
			}
		}

		// Case 4: Any other non-special cases for binary expression
		return expr.Binary{Op: e_copy.Op, X: e_copy.X, Y: e_copy.Y}

	default:
		panic("e does not belong to any required types")
	}
}
