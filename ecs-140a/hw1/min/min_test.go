package min

import "testing"

type Test struct {
	in  []int
	out int
}

var tests = []Test{
	{
		in:  []int{-1, 0, 1, 2, -4},
		out: -4,
	},
	{
		in:  []int{1},
		out: 1,
	},
	{
		in:  []int{},
		out: 0,
	},
	{
		in:  nil,
		out: 0,
	},
	{
		in:  []int{100, 1, -100, -219, 10, 86, -7},
		out: -219,
	},
	{
		in:  []int{1, 1, 1, 1, 1, 1, 1, 1},
		out: 1,
	},
	{
		in:  []int{9, 1, 7, 2, 7, 8, 7, 8, 9},
		out: 1,
	},
}

func TestMin(t *testing.T) {
	for i, test := range tests {
		m := Min(test.in)
		if m != test.out {
			t.Errorf("#%d: Min(%v)=%d; want %d", i, test.in, m, test.out)
		}
	}
}
