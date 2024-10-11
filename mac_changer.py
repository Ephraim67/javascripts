#!/usr/bin/env python3
import subprocess
import re
import sys

# Check the current IP address
def check_ip(interface):
    try:
        result = subprocess.run(f"ip addr show {interface}", shell=True, capture_output=True, text=True)
        if result.returncode == 0:
            # Regrex to extract IP address from the result

            ip_pattern = r"inet (\d+\.\d+\.\d+\.\d+)"
            ip_match = re.search(ip_pattern, result.stdout)
            if ip_match:
                print(f" [+] IP Address of {interface}: {ip_match.group(1)}")
            else:
                print(f" [-] No IP address found for {interface}.")
        else:
            print(f"Error getting IP address: {result.stderr}")
    
    except Exception as e:
        print(f"Error: {str(e)}")

# Function to change mac address
def change_mac(interface, new_mac):
    try:
        subprocess.run(f"sudo ifconfig {interface} down", shell=True)
        subprocess.run(f"sudo ifconfig {interface} hw ether {new_mac}", shell=True)
        subprocess.run(f"sudo ifconfig {interface} up", shell=True)
        print(f"MAC address for {interface} changed to {new_mac}")
    except Exception as e:
        print(f"Error changing MAC address: {str(e)}")

# function to change IP address

def change_ip(interface, new_ip, netmask):
    try:
        result = subprocess.run(f"sudo ifconfig {interface} {new_ip} netmask {netmask}", shell=True, capture_output=True, text=True)
        if result.returncode == 0:
            print(f"IP address of {interface} changed to {new_ip}/{netmask}")
        else:
            print(f"Error changing IP address: {result.stderr}")
    except Exception as e:
        print(f"Error: {str(e)}")

# function to display to current network configuration
def display_network_conf():
    try:
        result = subprocess.run(f"ifconfig", shell=True, capture_output=True, text=True)
        if result.returncode == 0:
            print(result.stdout)
        else:
            print(result.stderr)
    except Exception as e:
        print(f"Error: {str(e)}")

# Menu interfaces
def main():
    while True:
        print("\nNetwork Menu Commands")
        print("1. Display network configuration")
        print("2. Check IP address")
        print("3. Change MAC address")
        print("4. Change IP address")
        print("5. Exit")

        choice = input("Enter the command: ")

        if choice == '1':
            display_network_conf()

        elif choice == '2':
            interface = input("Enter the network interface you want to change (e.g., eth0, wlan0): ")
            check_ip(interface)

        elif choice == '3':
            interface = input("Enter the network interface you want to change (e.g., eth0, wlan0): ")
            new_mac = input("Enter the new MAC address (e.g., 00:11:22:33:44:55): ")
            change_mac(interface, new_mac)

        elif choice == '4':
            interface = input("Enter the network interface you want to change (e.g., eth0, wlan0): ")
            new_ip = input("Enter the new IP address (e.g., 192.168.1.100): ")
            netmask = input("Enter the subnet mask (e.g., 255.255.255.0): ")
            change_ip(interface, new_ip, netmask)

        elif choice == '5':
            print("Exiting program.")
            sys.exit()

        else:
            print("Invalid choice. Please try again.")

if __name__ == "__main__":
    main()
        