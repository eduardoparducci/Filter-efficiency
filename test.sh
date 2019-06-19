# Automatic test script
clear
echo "Testing $1."

# Get program name and data folder from argument
BIN=$1
DATA_FOLDER=$2

# Number of arguments
N_ARG=8

# Get files list ignoring *filtered.jpg
FILES=`find $DATA_FOLDER -not -name '*filtered.jpg' -name '*.jpg'`

# Run program with every file in DATA_FOLDER 5 times in range (1,N_ARGS) for multi* implementations
if [ $BIN != "./inline" ]; then
    for value in $(eval echo "{1..$N_ARG}")
    do
        for file in $FILES
        do
            for i in {1..5}
            do
                     $BIN $file $value
            done
        done
    done
else
    for file in $FILES
    do
        for i in {1..5}
        do
            $BIN $file
        done
    done
fi;
