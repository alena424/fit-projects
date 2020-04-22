#!/bin/sh

# Output settings
TEXT_BOLD=`tput bold`
TEXT_RED=`tput setaf 1`
TEXT_GREEN=`tput setaf 2`
TEXT_RESET=`tput sgr0`

path="../"
exec="rka-2-dka"

green() {
  printf "%s\t" "${TEXT_GREEN}$1${TEXT_RESET}"
}

red() {
  printf "%s\t" "${TEXT_RED}$1${TEXT_RESET}"
}

bold_green() {
  printf "%s\t" "${TEXT_GREEN}${TEXT_BOLD}$1${TEXT_RESET}"
}

bold_red() {
  printf "%s\t" "${TEXT_RED}${TEXT_BOLD}$1${TEXT_RESET}"
}

count=1
run_test() {
	"${current}/${path}/${exec}" ${1} ${2} 1>out/"$(dirname $test)"/"$(dirname $test)".out 2>out/"$(dirname $test)"/"$(dirname $test)".err &
	wait $!

  diff -u "$(dirname $test)"/"$(dirname $test)".err out/"$(dirname $test)"/"$(dirname $test)".err 2>&1 1>out/"$(dirname $test)"/"$(dirname $test)".err.diff
	errdiff=$?

	diff -u "$(dirname $test)"/"$(dirname $test)".out out/"$(dirname $test)"/"$(dirname $test)".out 2>&1 1>out/"$(dirname $test)"/"$(dirname $test)".out.diff
  outdiff=$?

	if [ $outdiff -eq 0 ] && [ $errdiff -eq 0 ];
		then
			printf "  Test %3d   " $count
		  bold_green "PASS"
			rm -rf out/"$(dirname $test)"
		else
			printf "  Test %3d   " $count
			bold_red "FAIL"
	fi
    
    printf "STD OUT: "
    if [ $outdiff -eq 0 ];
		then
			green "OK"
		else
			red "DIFF"
	fi

    printf "ERR OUT: "
    if [ $errdiff -eq 0 ];
		then
			green "OK"
		else
			red "DIFF"
	fi

	echo "Params: ${1} ${2}"

	export count=$((count+1))

}


current=$PWD

rm -rf $current/out


echo 
echo "##########################"
echo "#    RKA-2-DKA TESTS    #"
echo "##########################"

echo
echo "------------------------------"
echo "Compiling..."
echo "------------------------------"

make -C $current/$path 1> /dev/null
if [ $? -ne 0 ];
then
  printf "%s\n" "${TEXT_RED}Compilation failed.${TEXT_RESET}"
  exit 1
fi

echo
echo "------------------------------"
echo "Test dir: ${current}"
echo "------------------------------"

mkdir -p out

echo
echo "Running tests"
echo "------------------------------"

for test in test*/test*.in; do
  mkdir -p out/"$(dirname $test)"
  run_test "-t" $test
done

