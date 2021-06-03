package bug2

import (
	"sync"
)

func bug2(n int, foo func(int) int, out chan int) {
	var wg sync.WaitGroup
	var mux sync.Mutex
	ch := make(chan int, n)

	for i := 0; i < n; i++ {
		ch <- i
		wg.Add(1)

		// Since channel receiver is constantly updated in for loop, should have
		// a mutex to make sure it doesn't have race conditions
		mux.Lock()

		go func() {
			ch_recv := <-ch
			out <- foo(ch_recv)
			mux.Unlock()
			wg.Done()
		}()
	}

	wg.Wait()
	close(out)
}
