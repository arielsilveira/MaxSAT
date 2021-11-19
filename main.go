package main

import (
	"MaxSAT/src"
	"fmt"
)

func main() {
	// uf20-01.cnf
	maxSAT, atoms := src.ReadFile("uf20-01.cnf")

	fmt.Println("MaxSAT: ", maxSAT)
	fmt.Println("Atoms: ", atoms)
}
