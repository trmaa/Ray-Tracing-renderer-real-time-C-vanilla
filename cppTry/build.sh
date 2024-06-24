#!/bin/bash

if_file() {
	local file="$1"
	local callback=$2
	local error_callback=$3
	
	if [ -f "$file" ]; then 
		$callback
	elif [ -n "$error_callback" ]; then
		$error_callback
	fi
}

main() {
	clear
	if_file ./bin/run "rm ./bin/run"
	g++ -o ./bin/run ./src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system
	if_file ./bin/run "./bin/run" "echo srry..."
}

main
