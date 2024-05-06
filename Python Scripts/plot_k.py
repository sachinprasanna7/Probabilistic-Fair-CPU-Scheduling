import matplotlib.pyplot as plt

# Load data from files
def load_times_data(folder, filename):
    with open(f'Generated Data/{folder}/{filename}', 'r') as file:
        times = [float(time.strip()) for time in file.read().split(',') if time.strip()]
    return times


response_times = load_times_data('Response Times', 'responseTimes_k.txt')
waiting_times = load_times_data('Waiting Times', 'waitingTimes_k.txt')
turnaround_times = load_times_data('Turnaround Times', 'turnaroundTimes_k.txt')


x_values = list(range(0, 101))

plt.figure(figsize=(10, 6))


plt.plot(x_values, response_times, label='Response Times')
plt.plot(x_values, waiting_times, label='Waiting Times')
plt.plot(x_values, turnaround_times, label='Turnaround Times')


plt.xlabel('k')
plt.ylabel('Time')
plt.title('Comparing Response, Waiting, and Turnaround Times for Different k values')
plt.legend()


plt.savefig('Generated Data/Plotted Graphs/plot_k.png')