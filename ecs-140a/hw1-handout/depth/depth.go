package depth

import (
	"hw1/expr"
)

// Depth returns the maximum number of AST nodes between the
// root of the tree and any leaf (literal or variable) in the tree.
func Depth(e expr.Expr) uint {
	// Depth is at least 1 with root
	var depth uint = 1

	// Check expresion cases
	// Source: https://tour.golang.org/methods/16 + Piazza @41
	switch e := e.(type) {

	// Literal type
	case expr.Literal:
		return depth

	// Var type
	case expr.Var:
		return depth

	// Unary type
	case expr.Unary:
		// Source: https://stackoverflow.com/questions/1876255/how-to-calculate-the-depth-of-a-binary-search-tree
		// NOTE: For unary type, there are no right nodes (no Y)
		depth += Depth(e.X)
		return depth

	// Binary type
	case expr.Binary:
		// Source: https://stackoverflow.com/questions/1876255/how-to-calculate-the-depth-of-a-binary-search-tree
		ldepth := Depth(e.X)
		rdepth := Depth(e.Y)

		if ldepth >= rdepth {
			depth += ldepth
		} else {
			depth += rdepth
		}

		return depth

	default:
		panic("e does not belong to any required types")
	}
}
