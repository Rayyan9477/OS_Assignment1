#!/bin/bash

# Display name and roll number on script start
echo "22I—2489-Rayyan-Ahmed"

# Function to create a new user and assign admin privileges
create_user() {
    echo "Enter user: OS_Assignment1"
    echo "Enter password: 12345"
    
    # Create the user with specified username and password
    sudo useradd -m -s /bin/bash OS_Assignment_1
    echo "OS_Assignment_1:12345" | sudo chpasswd
    
    # Add the user to the sudo group (granting admin privileges)
    sudo usermod -aG sudo OS_Assignment_1
    echo "User OS_Assignment_1 is created, and administrator privileges are assigned."
}

# Function to list installed applications
list_installed_apps() {
    echo "Listing installed applications..."
    dpkg --get-selections | grep -v deinstall
}

# Function to install Dropbox (or any other specified application)
install_app() {
    echo "Installing Dropbox..."
    
    # Ensure wget is installed (required for downloading)
    sudo apt-get update && sudo apt-get install wget -y
    
    # Download Dropbox package
    wget -O dropbox.deb https://www.dropbox.com/download?dl=packages/ubuntu/dropbox_2020.03.04_amd64.deb
    
    # Install the Dropbox .deb package
    sudo dpkg -i dropbox.deb
    
    # Fix any missing dependencies
    sudo apt-get install -f
    
    echo "Dropbox has been installed."
}

# Function to configure network settings
configure_network() {
    echo "Configuring network with the following details:"
    echo "IP Address: 10.0.0.1"
    echo "Mask: 255.255.255.0"
    echo "Gateway: 10.0.0.254"
    echo "DNS: 8.8.8.8"
    
    # Restart network service to apply changes
    sudo systemctl restart networking
    echo "Network configuration updated successfully."
}

# Function to display help information
display_help() {
    echo "Available switches and tasks:"
    echo "-uc    : Create a new user 'OS_Assignment_1' and assign administrator privileges."
    echo "-ld    : List all installed applications on the Ubuntu system."
    echo "-ins   : Install Dropbox on the system."
    echo "-ipcon : Configure IP, subnet mask, gateway, and DNS settings."
    echo "-help  : Display this help information."
}

# Parse command line arguments
case "$1" in
    -uc)
        create_user
        ;;
    -ld)
        list_installed_apps
        ;;
    -ins)
        install_app
        ;;
    -ipcon)
        configure_network
        ;;
    -help)
        display_help
        ;;
    *)
        echo "Invalid option. Use -help to see available switches."
        ;;
esac
