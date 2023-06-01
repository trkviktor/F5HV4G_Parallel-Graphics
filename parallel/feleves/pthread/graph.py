import csv
import matplotlib.pyplot as plt

with open('MyFile.csv', 'r') as file:
    reader = csv.reader(file)
    header = next(reader)  # Read the header row
    # Strip whitespace from column names
    header = [col.strip() for col in header]
    data = list(reader)

parallel_time_idx = header.index('Parallel Time')
sequential_time_idx = header.index('Sequential time')
sample_size_idx = header.index('Sample size')
thread_number_idx = header.index('Thread number')

thread_numbers = set(row[thread_number_idx].strip() for row in data)

plt.figure(figsize=(10, 6))
plt.xlabel('Sample Size')
plt.ylabel('Time')
plt.title('Parallel Time vs Sequential Time')

for thread_num in thread_numbers:
    thread_data = [
        row for row in data if row[thread_number_idx].strip() == thread_num]

    # Extract the data for plotting
    parallel_time = [float(row[parallel_time_idx].strip())
                     for row in thread_data]
    sequential_time = [float(row[sequential_time_idx].strip())
                       for row in thread_data]
    sample_size = [int(row[sample_size_idx].strip()) for row in thread_data]

    # Plot the data
    plt.plot(sample_size, parallel_time,
             label=f'Thread {thread_num} - Parallel Time')
    plt.plot(sample_size, sequential_time,
             label=f'Thread {thread_num} - Sequential Time')

plt.legend()
plt.grid(True)
plt.savefig('graph.png');
plt.show()