package min

// Min returns the minimum value in the arr,
// and 0 if arr is nil.
func Min(arr []int) int {
	// empty string (nil) case --> return 0
	if len(arr) == 0 {
		return 0
	}

	// find the lowest number in arr
	min_num := arr[0]

	for i := 0; i < len(arr); i++ {
		if min_num > arr[i] {
			min_num = arr[i]
		}
	}

	return min_num
}
