import matplotlib.pyplot as plt

# Load data from files
def load_times_data(folder, filename):
    with open(f'Generated Data/{folder}/{filename}', 'r') as file:
        times = [float(time.strip()) for time in file.read().split(',') if time.strip()]
    return times


response_times = load_times_data('Response Times', 'responseTimes.txt')
waiting_times = load_times_data('Waiting Times', 'waitingTimes.txt')
turnaround_times = load_times_data('Turnaround Times', 'turnaroundTimes.txt')

x_values = list(range(1, 101))

plt.figure(figsize=(10, 6))


plt.plot(x_values, response_times, label='Response Times')
plt.plot(x_values, waiting_times, label='Waiting Times')
plt.plot(x_values, turnaround_times, label='Turnaround Times')

plt.xlabel('Time Quantum')
plt.ylabel('Time')
plt.title('Comparing Response, Waiting, and Turnaround Times for Different Time Quantums')
plt.legend()

plt.savefig('Generated Data/plot_timequantum.png')