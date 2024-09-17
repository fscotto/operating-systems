package main

import "os"

const (
	FREE      int = iota
	ALLOCATED int = iota
)

type Resource interface {
	Type() int
}

type ResourceManager interface {
	Available(r Resource) bool
	Assign(r Resource, p os.Process) Resource
	Release(r Resource, p os.Process) (bool, error)
}

type bankerResourceManager struct {
	available [][]int
	total     [][]int
	allocated [][]int
}

func main() {
}
