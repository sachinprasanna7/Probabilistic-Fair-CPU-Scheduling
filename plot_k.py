import matplotlib.pyplot as plt

# Load data from files
def load_times_data(folder, filename):
    with open(f'Generated Data/{folder}/{filename}', 'r') as file:
        times = [float(time.strip()) for time in file.read().split(',') if time.strip()]
    return times

# Load data for each type of time
response_times = load_times_data('Response Times', 'responseTimes_k.txt')
waiting_times = load_times_data('Waiting Times', 'waitingTimes_k.txt')
turnaround_times = load_times_data('Turnaround Times', 'turnaroundTimes_k.txt')

# Create x-axis values (1 to 100)
x_values = list(range(0, 101))


# Plotting
plt.figure(figsize=(10, 6))

# Plot response times
plt.plot(x_values, response_times, label='Response Times')

# Plot waiting times
plt.plot(x_values, waiting_times, label='Waiting Times')

# Plot turnaround times
plt.plot(x_values, turnaround_times, label='Turnaround Times')

# Adding labels and title
plt.xlabel('k')
plt.ylabel('Time')
plt.title('Comparing Response, Waiting, and Turnaround Times for Different k values')
plt.legend()

# Save plot the plot in a folder called Generated Data
plt.savefig('Generated Data/plot_k.png')

# Show plot
# plt.grid(True)
# plt.show()