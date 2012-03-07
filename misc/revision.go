package main
import (
	"fmt"
	"os"
	"os/exec"
	"io/ioutil"
	"strings"
)
func main(){
	
	file, err := os.OpenFile("version.hpp", os.O_RDWR, 0)
	if err != nil{
		fmt.Println(err)
		os.Exit(-1)
	}
	defer file.Close()
	bytes, err := ioutil.ReadAll(file)
	if err != nil{
		fmt.Println(err)
		os.Exit(-1)
	}
	
	content := string(bytes)
	lines := strings.Split(content, "\n")
	var linesToWrite string
	var needUpdate bool
	for _, line := range lines{
		if strings.Index(line, "const VERSION") != -1{
			from := strings.Index(line, `"`)
			to   := strings.LastIndex(line, `"`)

			if from == -1 && to == -1{
				fmt.Println("Error format of version.hpp")
				os.Exit(-1)
			}
			currentVer := line[from +1 :to]
			cmd := exec.Command("hg", "-q", "id")
			bytes, err := cmd.Output()
			if err != nil{
				fmt.Println(err)
				os.Exit(-1)
			}
			hgVer := strings.Trim(string(bytes), "\n")

			if hgVer == currentVer{
				fmt.Println("up-to-date revison: ", hgVer)
				os.Exit(0)
			}
			fmt.Println(currentVer, " => ", hgVer)
			line = strings.Replace(line, currentVer, hgVer, 1)
			needUpdate = true
		}
		linesToWrite += line
		linesToWrite += "\n"
	}
	if needUpdate{
		file.Seek(0, 0)
		_, err := file.WriteString(linesToWrite)
		if err != nil{
			fmt.Println(err)
			os.Exit(-1)
		}
	}
	os.Exit(0)
}
		
	
	