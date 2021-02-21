#!/usr/bin/env python3

# ------------------------------------------------------------- #
# ------------------ PROGRAM INSTRUCTIONS --------------------- #
# ------------------------------------------------------------- #

# In Terminal/Command Prompt, locate to the directory that contains
# the source code and the text files of DB and IP.

# Run the command: python ip2as.py ip2as DB_091803.txt IPlist.txt 

# ------------------------------------------------------------- #
# ------------------- IMPORTED LIBRARIES ---------------------- #
# ------------------------------------------------------------- #

from ipaddress import ip_network, ip_address
import os
import sys

# ------------------------------------------------------------- #
# --------------------- HELPER FUNCTION ----------------------- #
# ------------------------------------------------------------- #

# FUNCTION TO CHECK IF AN IP ADDRESS IS IN VALID FORMAT
# Source: https://stackoverflow.com/questions/3462784/check-if-a-string-matches-an-ip-address-pattern-in-python
def validate_ip(ip):
        split_ip = ip.split('.')
        
        if len(split_ip) != 4:
                return False

        for each in split_ip:
                if not each.isdigit():
                        return False
                
                num = int(each)
                if num < 0 or num > 255:
                        return False
        
        return True

# ------------------------------------------------------------- #
# ---------------------- MAIN FUNCTION ------------------------ #
# ------------------------------------------------------------- #

def main():
        # Open files
        DB_file = open(sys.argv[2])
        IP_file = open(sys.argv[3])

# GET INFORMATION ABOUT DB FILE
        DB_ip = []
        DB_prefix = []
        DB_as = []

        # Read each line in DB file
        DB_line = DB_file.readline()

        while DB_line != '':
                # Get DB info from a line
                DB_info = DB_line.replace("\n", "").split(" ")

                # Check invalid IPs here
                is_valid_db = validate_ip(DB_info[0])

                # Store IP/Prefix num/AS num in the appropriate list
                if is_valid_db == True:
                        DB_ip.append(DB_info[0])
                        DB_prefix.append(DB_info[1])
                        DB_as.append(DB_info[2])
        
                # Continue reading DB file
                DB_line = DB_file.readline()

# GET INFORMATION ABOUT IP LIST
        IP_list= []
        IP_line = IP_file.readline()

        while IP_line != '':
                # Get IP from file to list
                IP_line = IP_line.replace("\n", "")

                is_valid_ip = validate_ip(IP_line)

                if is_valid_ip == True:
                        IP_list.append(IP_line)

                # Continue reading IP file
                IP_line = IP_file.readline()

# FIND LONGEST PREFIX MATCH
        longest_prefix_match_index = []
        
        # Loop through IP list and DB list to find the longest prefix match
        for i in range(0, len(IP_list)):
                current_longest_match = ""
                current_best_index = 0

                for j in range(0, len(DB_ip)):
                        # Source: https://stackoverflow.com/questions/39358869/check-if-an-ip-is-within-a-range-of-cidr-in-python
                        ip_range = DB_ip[j] + "/" + DB_prefix[j]
                        net = ip_network(ip_range, False)
                        is_in_range = ip_address(IP_list[i]) in net

                        if (is_in_range == True):
                                # Get new found prefix match
                                match = os.path.commonprefix([IP_list[i], DB_ip[j]])

                                # Update current longest prefix match
                                # if the new found match is longer
                                if len(match) > len(current_longest_match):
                                        current_longest_match = match
                                        current_best_index = j

                                # If len(match) == len(current_longest_match)
                                # we will check if the IP is in range of DB
                                # If yes, then we check the DB prefix, if larger
                                # then it is a closer match
                                elif len(match) == len(current_longest_match):
                                        if (DB_prefix[j] >= DB_prefix[current_best_index]):
                                                current_longest_match = match
                                                current_best_index = j

                # Add index of current DB longest prefix match to list
                longest_prefix_match_index.append(current_best_index)

# WRITE TO OUTPUT.TXT FILE
        output_file = open("output_student.txt", "w")

        for k in range(0, len(longest_prefix_match_index)):
                index = longest_prefix_match_index[k]

                output_line = DB_ip[index] + "/" + DB_prefix[index] + " " + DB_as[index] + " " + IP_list[k] + "\n"

                output_file.write(output_line)

        output_file.close()

# ------------------------------------------------------------- #
# ---------------------- FORMAT CHECK ------------------------- #
# ------------------------------------------------------------- #

if (sys.argv[1] == "ip2as"):
        main()

else:
        print("Wrong format, please try again!\n")
        print("Run the following format in the command prompt:")
        print("\tpython main.py ip2as [file_DB] [file_IP]\n")
        print("Please make sure that your text files are in the same folder with main.py\n")
