package main

import (
	_ "embed"
	"fmt"
	"strconv"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

type dirInfo struct {
	subDirs      []string
	curDirSize   int
	totalDirSize int
}

func removeLastDirFromPath(curPath string) string {
	pathElems := strings.Split(curPath, "/")

	ret := "/"
	for pathIdx := 0; pathIdx < (len(pathElems) - 1); pathIdx++ {
		if pathElems[pathIdx] != "" {
			if ret == "/" {
				ret += fmt.Sprintf("%s", pathElems[pathIdx])
			} else {
				ret += fmt.Sprintf("/%s", pathElems[pathIdx])
			}
		}
	}

	return ret
}

func addSubDir(info dirInfo, dir string) dirInfo {
	info.subDirs = append(info.subDirs, dir)
	return info
}

func setDirSize(info dirInfo, dirSize int) dirInfo {
	info.curDirSize = dirSize
	return info
}

func setTotalDirSize(info dirInfo, totalDirSize int) dirInfo {
	info.totalDirSize = totalDirSize
	return info
}

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	dirMap := map[string]dirInfo{}
	curDirTotalFileSize := 0
	curDirPath := "/"

	for _, line := range lines {
		if line == "$ cd /" || line == "$ ls" {
			continue
		}

		if strings.HasPrefix(line, "$ cd ") {
			cmdElems := strings.Split(line, " ")
			if len(cmdElems) != 3 {
				fmt.Printf("ERROR: unable to parse '%s' correctly\n", line)
				return -1
			}

			if curDirTotalFileSize > 0 {
				curEntry, _ := dirMap[curDirPath]
				dirMap[curDirPath] = setDirSize(curEntry, curDirTotalFileSize)
			}

			if cmdElems[2] == ".." {
				curDirPath = removeLastDirFromPath(curDirPath)
			} else {
				if curDirPath != "/" {
					curDirPath += fmt.Sprintf("/%s", cmdElems[2])
				} else {
					curDirPath += fmt.Sprintf("%s", cmdElems[2])
				}
				dirMap[curDirPath] = dirInfo{subDirs: []string{}, curDirSize: 0}
			}

			curDirTotalFileSize = 0
		} else if strings.HasPrefix(line, "dir") {
			dirElems := strings.Split(line, " ")
			if len(dirElems) != 2 {
				fmt.Printf("ERROR: unable to parse '%s' correctly\n", line)
				return -1
			}
			curEntry, _ := dirMap[curDirPath]
			if curDirPath == "/" {
				dirMap[curDirPath] = addSubDir(curEntry, fmt.Sprintf("%s%s", curDirPath, dirElems[1]))
			} else {
				dirMap[curDirPath] = addSubDir(curEntry, fmt.Sprintf("%s/%s", curDirPath, dirElems[1]))
			}
		} else {
			// must be a file listing
			fileListElems := strings.Split(line, " ")
			if len(fileListElems) != 2 {
				fmt.Printf("ERROR: unable to parse '%s' correctly\n", line)
				return -1
			}
			curFileSize, _ := strconv.Atoi(fileListElems[0])
			curDirTotalFileSize += curFileSize
		}
	}

	if curDirTotalFileSize > 0 {
		curEntry, _ := dirMap[curDirPath]
		dirMap[curDirPath] = setDirSize(curEntry, curDirTotalFileSize)
	}

	fmt.Printf("%v\n", dirMap)

	sumLargeDirSize := 0
	for dirName, dirElem := range dirMap {

		dirNamePrefix := "/"
		if dirName != "/" {
			dirNamePrefix = fmt.Sprintf("%s/", dirName)
		}

		sumRecursiveDirSize := dirElem.curDirSize
		for subDirName, subDirElem := range dirMap {
			if subDirName == "/" {
				continue
			}

			if strings.HasPrefix(subDirName, dirNamePrefix) {
				fmt.Printf("Directory '%s' contains '%s'\n", dirName, subDirName)
				sumRecursiveDirSize += subDirElem.curDirSize
			}
		}

		fmt.Printf("Dir '%s' has size: %d\n", dirName, sumRecursiveDirSize)

		if sumRecursiveDirSize < 100000 {
			sumLargeDirSize += sumRecursiveDirSize
		}
	}

	return sumLargeDirSize
}

func Part2(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	dirMap := map[string]dirInfo{}
	curDirTotalFileSize := 0
	curDirPath := "/"

	for _, line := range lines {
		if line == "$ cd /" || line == "$ ls" {
			continue
		}

		if strings.HasPrefix(line, "$ cd ") {
			cmdElems := strings.Split(line, " ")
			if len(cmdElems) != 3 {
				fmt.Printf("ERROR: unable to parse '%s' correctly\n", line)
				return -1
			}

			if curDirTotalFileSize > 0 {
				curEntry, _ := dirMap[curDirPath]
				dirMap[curDirPath] = setDirSize(curEntry, curDirTotalFileSize)
			}

			if cmdElems[2] == ".." {
				curDirPath = removeLastDirFromPath(curDirPath)
			} else {
				if curDirPath != "/" {
					curDirPath += fmt.Sprintf("/%s", cmdElems[2])
				} else {
					curDirPath += fmt.Sprintf("%s", cmdElems[2])
				}
				dirMap[curDirPath] = dirInfo{subDirs: []string{}, curDirSize: 0}
			}

			curDirTotalFileSize = 0
		} else if strings.HasPrefix(line, "dir") {
			dirElems := strings.Split(line, " ")
			if len(dirElems) != 2 {
				fmt.Printf("ERROR: unable to parse '%s' correctly\n", line)
				return -1
			}
			curEntry, _ := dirMap[curDirPath]
			if curDirPath == "/" {
				dirMap[curDirPath] = addSubDir(curEntry, fmt.Sprintf("%s%s", curDirPath, dirElems[1]))
			} else {
				dirMap[curDirPath] = addSubDir(curEntry, fmt.Sprintf("%s/%s", curDirPath, dirElems[1]))
			}
		} else {
			// must be a file listing
			fileListElems := strings.Split(line, " ")
			if len(fileListElems) != 2 {
				fmt.Printf("ERROR: unable to parse '%s' correctly\n", line)
				return -1
			}
			curFileSize, _ := strconv.Atoi(fileListElems[0])
			curDirTotalFileSize += curFileSize
		}
	}

	if curDirTotalFileSize > 0 {
		curEntry, _ := dirMap[curDirPath]
		dirMap[curDirPath] = setDirSize(curEntry, curDirTotalFileSize)
	}

	usedSpace := 0
	for dirName, dirElem := range dirMap {

		dirNamePrefix := "/"
		if dirName != "/" {
			dirNamePrefix = fmt.Sprintf("%s/", dirName)
		}

		sumRecursiveDirSize := dirElem.curDirSize
		for subDirName, subDirElem := range dirMap {

			if subDirName == "/" {
				continue
			}

			if strings.HasPrefix(subDirName, dirNamePrefix) {
				sumRecursiveDirSize += subDirElem.curDirSize
			}
		}

		dirMap[dirName] = setTotalDirSize(dirElem, sumRecursiveDirSize)
		curEntry, _ := dirMap[dirName]

		fmt.Printf("Dir '%s' has size: %d\n", dirName, curEntry.totalDirSize)

		if dirName == "/" {
			usedSpace = sumRecursiveDirSize
		}
	}

	maxAllowedUsedSpace := 70000000 - 30000000
	minSpaceToClear := usedSpace - maxAllowedUsedSpace
	fmt.Printf("Used Space: %d Min space to clear %d\n", usedSpace, minSpaceToClear)
	minDirSpace := 70000000
	for dirName, dirElem := range dirMap {
		if dirName == "/" {
			continue
		}
		if dirElem.totalDirSize > minSpaceToClear {
			if dirElem.totalDirSize < minDirSpace {
				minDirSpace = dirElem.totalDirSize
			}
		}
	}

	return minDirSpace
}

func main() {
	fmt.Println("--{2022} day {07} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
