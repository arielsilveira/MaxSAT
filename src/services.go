package src

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

func ReadFile(fileName string) (maxSat [][]string, atoms []int) {
	// var p1, p2, p3 string

	file, err := os.Open("input/uf20-91/" + fileName)

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanLines)

	for scanner.Scan() {
		s := strings.Fields(scanner.Text())
		if s[0] == "p" {
			atoms = make([]int, stringToInt(s[2]))
			break
		}
	}

	for scanner.Scan() {
		s := strings.Fields(scanner.Text())
		if s[0] == "%" {
			break
		}
		p1, p2, p3 := s[0], s[1], s[2]
		maxSat = append(maxSat, []string{p1, p2, p3})
	}

	return maxSat, atoms
}

func AskOption(question string, options []interface{}) (answer int) {

	fmt.Println(":::::::::: " + strings.ToUpper(question) + " ::::::::::")
	fmt.Println()
	for optKey, optVal := range options {
		fmt.Printf("[%d] %s\n", optKey, optVal)
	}
	answer = -1
	for ok := true; ok; ok = (answer < 0 || answer > len(options)) {
		fmt.Printf("\n" + "Escolha: ")
		questionInputScanner := bufio.NewScanner(os.Stdin)
		for questionInputScanner.Scan() {
			if answerChosen, err := strconv.Atoi(questionInputScanner.Text()); err == nil {
				if answerChosen >= 0 && answerChosen < len(options) {
					answer = answerChosen
					break
				}
			}
			fmt.Println("Oops. Resposta fora do intervalo. Tente novamente.")
			break
		}
	}
	return answer
}

func stringToInt(s string) int {
	value, _ := strconv.Atoi(s)
	return value
}
