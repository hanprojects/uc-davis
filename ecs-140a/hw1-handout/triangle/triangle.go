package triangle

type triangleType uint8

const (
	UnknownTriangle = triangleType(iota)
	InvalidTriangle
	RightTriangle
	AcuteTriangle
	ObtuseTriangle
)

func isTriangle(a, b, c int) bool {
	return a+b > c && b+c > a && a+c > b
}

func getTriangleType(a, b, c int) triangleType {
	switch {
	case a > 1000:
		return UnknownTriangle
	case b > 2000:
		return UnknownTriangle
	case c > 3000:
		return UnknownTriangle
	case a <= 0:
		return UnknownTriangle
	case b <= 0:
		return UnknownTriangle
	case c <= 0:
		return UnknownTriangle
	}
	if !isTriangle(a, b, c) {
		return InvalidTriangle
	}
	if a*a+b*b == c*c {
		return RightTriangle
	} else if a*a+b*b > c*c {
		return AcuteTriangle
	} else {
		return ObtuseTriangle
	}
}
