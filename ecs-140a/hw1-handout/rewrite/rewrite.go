package rewrite

import (
	"bytes"
	"go/ast"
	"go/format"
	"go/parser"
	"go/token"
	"strconv" // This package may be helpful...

	"hw1/expr"
	"hw1/simplify"
)

// rewriteCalls should modify the passed AST
func rewriteCalls(node ast.Node) {
	// TODO Write the rewriteCalls function
	// panic("TODO: implement this!")

	// ------------------------- STUDENT'S ANSWER --------------------------- //
	ast.Inspect(node, func(node ast.Node) bool {
		switch node1 := node.(type) {
		// Source: https://golang.org/pkg/go/ast/#CallExpr
		case *ast.CallExpr:
			switch node2 := node1.Fun.(type) {
			case ast.Expr:
				// Source: https://golang.org/pkg/go/ast/#SelectorExpr
				fname, found := node2.(*ast.SelectorExpr)
				if found {
					if fname.Sel.Name == "ParseAndEval" && len(node1.Args) >= 2 {
						switch node3 := node1.Args[0].(type) {

						// Source: https://golang.org/pkg/go/ast/#BasicLit
						case *ast.BasicLit:
							// Remove quote before simplification - Piazza @95
							val, _ := strconv.Unquote(node3.Value)
							expression, err := expr.Parse(val)

							// If no errors, proceed simplification
							if err == nil {
								// Parse the expr to simplify - NOTE: empty expr.Env
								simplified_expr := simplify.Simplify(expression, expr.Env{})

								// Format simplified expr to string
								// NOTE: Remember to add quote back
								str := expr.Format(simplified_expr)
								str = strconv.Quote(str)
								node3.Value = str
							}

						default:
							// panic("No ast.BasicLit type found")
						}
					}
				}

			default:
				// panic("No ast.SelectorExpr type found")
			}

		default:
			// panic("No ast.CallExpr type found")
		}

		return true
	})
}

func SimplifyParseAndEval(src string) string {
	fset := token.NewFileSet()
	f, err := parser.ParseFile(fset, "src.go", src, 0)
	if err != nil {
		panic(err)
	}

	rewriteCalls(f)

	var buf bytes.Buffer
	format.Node(&buf, fset, f)
	return buf.String()
}
