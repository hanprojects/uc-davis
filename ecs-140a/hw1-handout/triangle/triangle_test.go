package triangle

import "testing"

func TestGetTriangleType(t *testing.T) {
	type Test struct {
		a, b, c  int
		expected triangleType
	}

	var tests = []Test{
		{1001, 5, 6, UnknownTriangle},   // a > 1000
		{6, 2002, 5, UnknownTriangle},   // b > 2000
		{6, 5, 3003, UnknownTriangle},   // c > 3000
		{-1, 5, 6, UnknownTriangle},     // a <= 0
		{10, -10, 10, UnknownTriangle},  // b <= 0
		{3, 4, -5, UnknownTriangle},     // c <= 0
		{1, 1, 2, InvalidTriangle},      // 1 + 1 = 2 --> invalid triangle
		{3, 4, 5, RightTriangle},        // basic Pythagorean right triangle
		{6, 6, 8, AcuteTriangle},        // Acute triangle
		{100, 150, 200, ObtuseTriangle}, // Obtuse triangle
	}

	for _, test := range tests {
		actual := getTriangleType(test.a, test.b, test.c)
		if actual != test.expected {
			t.Errorf("getTriangleType(%d, %d, %d)=%v; want %v", test.a, test.b, test.c, actual, test.expected)
		}
	}
}
