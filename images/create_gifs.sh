#!/bin/bash
files=`ls *.png`
for lhs in $files
do
    for rhs in $files
    do
        if [[ $lhs != $rhs ]]
        then
            out_file="${lhs%.*}_to_${rhs%.*}.gif"
            echo "Generating $out_file"
            convert $lhs $rhs -loop 1 -morph 8 $out_file
        fi
    done
done

