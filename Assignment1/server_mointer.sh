#!/bin/bash

LOG_FILE="server_monitor.log"

# Function to check disk usage
check_disk_usage() {
    disk_usage=$(df / | grep / | awk '{ print $5 }' | sed 's/%//g')
    if [ "$disk_usage" -gt 50 ]; then
        echo "$(date): Disk usage is at $disk_usage%, which exceeds the threshold." >> $LOG_FILE
    else
        echo "$(date): Disk usage is at $disk_usage%, which is within the limit." >> $LOG_FILE
    fi
}

# Function to check CPU usage
check_cpu_usage() {
    cpu_usage=$(top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{print 100 - $1}')
    if (( $(echo "$cpu_usage > 50" | bc -l) )); then
        echo "$(date): CPU usage is at $cpu_usage%, which exceeds the threshold." >> $LOG_FILE
    else
        echo "$(date): CPU usage is at $cpu_usage%, which is within the limit." >> $LOG_FILE
    fi
}

# Function to check memory usage
check_memory_usage() {
    mem_usage=$(free | grep Mem | awk '{print $3/$2 * 100.0}')
    if (( $(echo "$mem_usage > 70" | bc -l) )); then
        echo "$(date): Memory usage is at $mem_usage%, which exceeds the threshold." >> $LOG_FILE
    else
        echo "$(date): Memory usage is at $mem_usage%, which is within the limit." >> $LOG_FILE
    fi
}

# Function to rotate logs
rotate_logs() {
    log_size=$(du -k "$LOG_FILE" | cut -f1)
    if [ "$log_size" -gt 25600 ]; then
        echo "$(date): Log file size exceeded 25MB. Rotating logs." >> $LOG_FILE
        mv "$LOG_FILE" "$LOG_FILE.old"
        touch "$LOG_FILE"
    else
        echo "$(date): Log file size is within the limit." >> $LOG_FILE
    fi
}

# Main script 
{
    echo "$(date): Server monitoring started."
    check_disk_usage || echo "$(date): Error checking disk usage." >> $LOG_FILE
    check_cpu_usage || echo "$(date): Error checking CPU usage." >> $LOG_FILE
    check_memory_usage || echo "$(date): Error checking memory usage." >> $LOG_FILE
    rotate_logs || echo "$(date): Error rotating logs." >> $LOG_FILE
    echo "$(date): Server monitoring completed."
} 2>> $LOG_FILE

# To Run => ./server_mointer.sh