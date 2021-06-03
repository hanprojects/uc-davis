package bug1

import (
	"sync"
)

// Counter stores a count.
type Counter struct {
	n int64
}

// Use mutex to avoid race conditions
var lock sync.Mutex

// Inc increments the count in the Counter.
func (c *Counter) Inc() {
	lock.Lock()
	c.n++
	lock.Unlock()
}
