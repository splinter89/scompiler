find . -name '*.cpp' -or -name '*.h' -or -name '*.ui' -or -name '*.php' -or -name '*.txt' -or -name '*.jff' | xargs -n1 -I_file -- sh -c 'echo "> _file"; perl -ane "{ if(m/[[:^ascii:]]/) { print \"$.\t@F\n\" } }" "_file"; printf "\n\n\n\n\n";'
