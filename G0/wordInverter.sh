#!/bin/bash

x="$1"
text="$2"

read -ra array <<< "$text"

for i in "${!array[@]}"; do
    word="${array[$i]}"
    clean_word="${word//,/}"
    clean_word="${clean_word//!/}"
    count=${#clean_word}

    if ((count > x)); then
        upper="${clean_word^^}"
        rev=""
        for (( j=${#upper}-1; j>=0; j--)); do
            rev+="${upper:j:1}"
        done
        array[$i]="$rev"
    fi

done

echo "${array[@]}"