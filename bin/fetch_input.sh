#!/bin/bash

YEAR=2024
DAY=${PWD##*/}

curl -b session=$(cat ${HOME}/.aocrc) https://adventofcode.com/${YEAR}/day/${DAY}/input > input