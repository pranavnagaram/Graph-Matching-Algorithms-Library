#!/bin/bash

# Loop through test cases from 00 to 99
for i in {00..99}
do
    # Run the graphmatching algorithm with the specified input and output files
    ./graphmatching -q -i ../resources/input/input$i.txt -o ../resources/output/output$i.txt -L ../resources/log_files/log$i.txt
done

