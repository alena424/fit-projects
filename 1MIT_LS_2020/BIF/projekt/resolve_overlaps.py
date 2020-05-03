#!/usr/bin/env python3.6
# BIF Project 2020
# Author: Alena Tesařová (xtesar36@stud.fit.vutbr.cz)
# Date: 28.4.2020
# Name: resolve_overlaps.py

import sys
import datetime
from operator import itemgetter

# Generic Feature Format Version 3 (GFF3)
SEQID = 0
SOURCE = 1
TYPE = 2
START = 3
END = 4
SCORE = 5
STRAND = 6
PHASE = 7

"""
process_one_group
Function picks non overlapping sequences with highest score of one group using Weighted Interval Scheduling algorithm
@param sortedSequence sorted sequence by end time of one group
@return array with final indexes to sortedSequence
"""
def process_one_group(sortedSequences):
    weightArray = [0]  # initial weight is zero
    indexEndBeforeStartArray = [0]  # initial index is zero
    arrayM = [0]
    i = 0

    for infoSequence in sortedSequences:
        weightArray.append(infoSequence["score"])
        j = i - 1
        foundAppend = 0
        while j >= 0:
            if ( infoSequence['start'] > sortedSequences[j]['end'] ):
                foundAppend = j+1
                break
            j = j - 1
        indexEndBeforeStartArray.append(foundAppend)
        if (i >= 0):
            newValue = float(weightArray[i +1]) + arrayM[indexEndBeforeStartArray[i+1]]

            if (arrayM[i] > newValue):
                arrayM.append(arrayM[i])
            else:
                arrayM.append(newValue)
        i = i + 1

    arrayS = []

    j = len(arrayM) - 1
    # instead of backtracking, we will use this solution:
    while j > 0:
        if (float(weightArray[j]) + arrayM[indexEndBeforeStartArray[j]]) >= arrayM[j - 1]:
            arrayS.append(j - 1)  # appending the list of best sequences
            j = indexEndBeforeStartArray[j]
        else:
            j -= 1

    return arrayS


"""
process_file
@description Function process input file and saves information to array
@return sequence_array
"""
def process_file(file_name):
    sequence_array = {}  # array with all sequences

    with open(file_name, 'r') as inputFile:
        for line in inputFile:
            if line[0:2] == "##":  # We dont need comment in the array
                continue
            splitLine = [splits for splits in line.split("\t") if
                         splits is not ""]  # making an array to work better with the structure
            index = splitLine[SEQID] + splitLine[TYPE] + splitLine[STRAND]
            if sequence_array.get(index) is None:  # there are no data yet
                sequence_array[index] = {}
                sequence_array[index]['seqId'] = splitLine[SEQID]
                sequence_array[index]['type'] = splitLine[TYPE]
                sequence_array[index]['strand'] = splitLine[STRAND]
                sequence_array[index]['data'] = []
            sequence_array[index]['data'].append({
                "source": splitLine[SOURCE],
                "start": int(splitLine[START]),
                "end": int(splitLine[END]),
                "score": splitLine[SCORE],
                "phase": splitLine[PHASE].replace("\n", "")
            })
        return sequence_array


"""
print_group_sequences
@description Function prints picked non-overlapping sequences with highest score from one group on output
@param arrayS containes indexes to sortedSequences array with picked non-overlapping sequences
@param sequence_group array with information about seqId, type and strand
@param sortedSequences all sequences
"""


def print_group_sequences(arrayS, sequence_group, sortedSequences):
    for chosenIndex in reversed(arrayS):
        line = sortedSequences[chosenIndex]
        print(
            sequence_group['seqId'] + "\t" +
            line["source"] + "\t" +
            sequence_group["type"] + "\t" +
            str(line["start"]) + "\t" +
            str(line["end"]) + "\t" +
            str(line["score"]) + "\t" +
            sequence_group["strand"] + "\t" +
            line["phase"])


"""
Weighted Interval Scheduling algorithm
@description Reads file row by row and saves them to structure sequence_array, then iterates/process each group and
 prints the final output
"""


def run_algorithm(file_name):
    print("##gff-version 3")  # printing header
    print("##date " + datetime.datetime.now().strftime("%Y-%m-%d"))
    sequence_array = process_file(
        file_name)  # sequence array has important structure where index is (seqId + type + strand)

    for index, sequence_group in sequence_array.items():
        sortedSequences = sorted(sequence_group["data"], key=itemgetter(
            "end"))  # sorting by end time, end time is in data attribute of the structure
        arrayS = process_one_group(
            sortedSequences)  # processing each group and picking non-overlapping sequences with highest score
        print_group_sequences(arrayS, sequence_group, sortedSequences)  # printing results


# -------------- Main function ------------------
if __name__ == '__main__':
    if (len(sys.argv) != 2):  # checking arguments
        print("ERROR: program expects one argument with path of a input file\n", file=sys.stderr)
        exit(False)

    file_name = sys.argv[1]  # name of process file is expected on input
    try:
        run_algorithm(file_name)
    except FileNotFoundError:
        print("File was not found", file=sys.stderr)
        exit(False)
