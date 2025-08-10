#!/bin/bash

# Author: Jacob Smith 
#
# Commentary: This script tests the identity server by sending it 
#      different requests. The server's response is written to
#      a file, and the diff command is used to check the output
#      against the expected 

##########################################
# Check for valid command line arguments #
##########################################
case $# in 
2) ;;
*) echo "Usage: " `basename $0` " <server-quantity> <rmi-port>"; exit 1;;
esac

########################
# Function definitions #
########################

# Kill the server and the RMI registry
function cleanup ( )
{
    
    # Log
    printf "${CYAN}╔═════════════════╗\n║ Stopping tester ║\n╚═════════════════╝${RESET}\n\n"

    # Kill the server
    kill $RMIPID &> /dev/null

    # Log 
    printf "${YELLOW}╭──────────────────────╮\n│ Servers are still up │\n╰──────────────────────╯${RESET}\n\n"
}

# Catch signals
trap cleanup SIGINT;

####################
# Initialized data #
####################
EXIT_SUCCESS=0
EXIT_FAILURE=1
SERVER_QUANTITY=$1
REDIS_PORT=5110
RMI_PORT=$2
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[0;93m'
RESET='\033[0m'
declare -a SERVER_ADDRESSES

######################
# Prepare the tester #
######################

# Log
printf "${CYAN}╔══════════════════════════════════╗\n║ Requesting superuser permissions ║\n╚══════════════════════════════════╝${RESET}\n"

# Ask for superuser up front
sudo ls > /dev/null

# Kill any docker containers
sudo docker kill $(sudo docker ps -q) > /dev/null

# Clear the old log files
cd logs; rm *; cd ..
rm servers.txt
touch servers.txt

# Log
printf "\n${CYAN}╔═════════════════╗\n║ Starting tester ║\n╚═════════════════╝${RESET}\n\n"

# Compile everything
make clean > /dev/null
make > logs/make_output.txt 2> logs/make_output.txt

# Catch compile errors
if [ $? != $EXIT_SUCCESS ]; then

    # Print an error to standard out
    printf "${RED}[Error: Failed to compile project!]\nMake says: ${RESET}\n\n"

    # Concatenate makefile output to standard out
    cat make_output.txt

    # Clean up
    cleanup

    # Error
    exit $EXIT_FAILURE
else

    # Print an error to standard out
    printf "${GREEN}╭───────────────────────────────╮\n│ Project compiled successfully │\n╰───────────────────────────────╯${RESET}\n\n"
fi

# Construct a list of IP addresses and port numbers foreach instance of the identity server
for (( i=0; i < $SERVER_QUANTITY; i++ ))
do
    SERVER_ADDRESSES[i]="172.17.0.$(($i+2))"
    echo "${SERVER_ADDRESSES[i]}:${RMI_PORT}" >> servers.txt
    echo "" > "logs/identity_server_${i}.txt" 
done

# Build the docker image
sudo docker build -t identity_server . > logs/docker_build_output.txt 2> logs/docker_build_output.txt

# Start identity servers
for (( i=0; i < $SERVER_QUANTITY; i++ ))
do
    printf "${GREEN}Starting identity server $(($i + 1))${RESET}\r"
    sudo docker logs --details -f $(sudo docker run -it -d identity_server) >& "logs/identity_server_${i}.txt" &
done

printf "\n${GREEN}All ${SERVER_QUANTITY} server are up and running${RESET}\n\n"

for (( i=0; i < $SERVER_QUANTITY; i++ ))
do
    echo "${SERVER_ADDRESSES[i]}:${RMI_PORT}"
done

# Clean up
cleanup

# Success
exit $EXIT_SUCCESS