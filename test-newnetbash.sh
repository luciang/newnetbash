#!/bin/bash

NET_TEST_SERIES="010 050 100 200"

# get more than one sample of the free system memory.
FREEMEM_SAMPLES=10

# interval between two samples of free system memory.
FREEMEM_SAMPLING_INTERVAL=1


run_shells () {
    local freemem_filename=freemem_proc_$1_$2


    # run shells
    echo Running $1 clients in $2 mode
    for ((i=0;i<$1;i++)); do
	echo "sleep 100" | sudo ./newnetbash $2 &> /dev/null & true  ;
    done


    # sample free memory
    rm -f $freemem_filename
    touch $freemem_filename

    for ((i=0;i<$FREEMEM_SAMPLES;i++)); do
	sleep $FREEMEM_SAMPLING_INTERVAL;
	cat /proc/meminfo  | grep MemFree | cut -d':' -f 2 | cut -dk -f 1 >> $freemem_filename;
    done

    
    # kill shells
    killall -9 sleep
}


test_batch () {
    for nr_nets in $NET_TEST_SERIES; do
	run_shells $nr_nets shared
	run_shells $nr_nets newnet
    done

}

compute_avg () {
    for nr_nets in $NET_TEST_SERIES; do
	sum_newnet=$( cat "freemem_proc_"$nr_nets"_newnet" | \
	    awk "BEGIN {sum=0;} { sum+=\$1 } END {print sum/$FREEMEM_SAMPLES}" )
	sum_shared=$( cat "freemem_proc_"$nr_nets"_shared" | \
	    awk "BEGIN {sum=0;} { sum+=\$1 } END {print sum/$FREEMEM_SAMPLES}" )

	echo "Amount used for 1 net (testing with $nr_nets)" $((($sum_shared-$sum_newnet)/$nr_nets)) "KB"
    done
}


if [ ! -f ./newnetbash ]; then
    echo $0: Error: no ./newnetbash file found! Aborting tests!
    exit 1
fi



test_batch
compute_avg
