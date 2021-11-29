package main

import (
	"MaxSAT/src"
	"fmt"
	"os"
)

func main() {
	// file := "uf20-01.cnf"

	args, err := src.CheckArgs(os.Args)

	if err != nil {
		fmt.Println(err)
		os.Exit(0)
	}

	file := args[1]

	maxSAT, atoms := src.ReadFile(file)

	fmt.Println("MaxSAT: ", maxSAT)
	fmt.Println("Atoms: ", atoms)
}
