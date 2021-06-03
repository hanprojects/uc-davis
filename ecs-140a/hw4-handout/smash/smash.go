package smash

import (
	"bufio"
	"io"
	"sync"
	// "bytes"
	// "strings"
)

type word string

func Smash(r io.Reader, smasher func(word) uint32) map[uint32]uint {
	m := make(map[uint32]uint)

	// Get input scan words
	// Source: https://stackoverflow.com/questions/9644139/from-io-reader-to-string-in-go
	// s := new(bytes.Buffer) // shouldn't use bytes packages (Piazza @361)
	// s.ReadFrom(r)
	// scan_words := strings.Fields(s.String()) // TODO: how to use strings.Fields if not use bytes packages?

	// Source: https://golang.org/src/bufio/example_test.go
	scan_words := bufio.NewScanner(r)
	scan_words.Split(bufio.ScanWords)

	var wg sync.WaitGroup
	var mutex sync.Mutex

	for scan_words.Scan() {
		i := smasher(word(scan_words.Text()))
		wg.Add(1)

		go func(i uint32, m map[uint32]uint) {
			// Since we increment items in map, should use mutex
			// (might not be necessary, but doesn't hurt)
			mutex.Lock()
			m[i] += 1
			mutex.Unlock()
			wg.Done()
		}(i, m)
	}

	wg.Wait()
	return m
}
