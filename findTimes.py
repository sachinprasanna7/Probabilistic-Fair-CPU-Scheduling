import os

# Function to extract waiting times from a file
def extract_waiting_times(filename):
    waiting_times = []
    try:
        # Open the file
        with open(filename, 'r') as file:
            # Loop through each line in the file
            for line in file:
                # Split the line by tabs
                data = line.split('\t')
                # Extract the waiting time (the last number)
                waiting_time = int(data[-1])
                # Append the waiting time to the waiting_times array
                waiting_times.append(waiting_time)
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
    return waiting_times

# Directory containing the files
directory = 'Generated Data/Gini Index'

# Extract waiting times for each scheduling algorithm
waitingTimeFCFS = extract_waiting_times(os.path.join(directory, 'fcfs.txt'))
waitingTimeSJF = extract_waiting_times(os.path.join(directory, 'sjf.txt'))
waitingTimeSRTF = extract_waiting_times(os.path.join(directory, 'srtf.txt'))
waitingTimeRR = extract_waiting_times(os.path.join(directory, 'rr.txt'))
waitingTimePP = extract_waiting_times(os.path.join(directory, 'pp.txt'))
waitingTimePNP = extract_waiting_times(os.path.join(directory, 'pnp.txt'))

def write_waiting_times(filename, waiting_times):
    try:
        # Open the file for writing
        with open(filename, 'w') as file:
            # Write waiting times as comma-separated values
            file.write(','.join(map(str, waiting_times)))
    except IOError:
        print(f"Error: Unable to write to file '{filename}'.")

# Write waiting times to files
write_waiting_times(os.path.join(directory, 'fcfs_waitingtimes.txt'), waitingTimeFCFS)
write_waiting_times(os.path.join(directory, 'sjf_waitingtimes.txt'), waitingTimeSJF)
write_waiting_times(os.path.join(directory, 'srtf_waitingtimes.txt'), waitingTimeSRTF)
write_waiting_times(os.path.join(directory, 'rr_waitingtimes.txt'), waitingTimeRR)
write_waiting_times(os.path.join(directory, 'pp_waitingtimes.txt'), waitingTimePP)
write_waiting_times(os.path.join(directory, 'pnp_waitingtimes.txt'), waitingTimePNP)

# Example usage, printing waiting times for each algorithm
print("FCFS waiting times:", waitingTimeFCFS)
print("\n\n\n")
print("SJF waiting times:", waitingTimeSJF)
print("\n\n\n")
print("SRTF waiting times:", waitingTimeSRTF)
print("\n\n\n")
print("RR waiting times:", waitingTimeRR)
print("\n\n\n")
print("PP waiting times:", waitingTimePP)
print("\n\n\n")
print("PNP waiting times:", waitingTimePNP)
print("\n\n\n")


