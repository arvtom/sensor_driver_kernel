import statistics
import matplotlib.pyplot as plt

value = []
value_str = []
timestamp = []
period = []
start = []
execution_time = []

with open('pcf8591.txt') as f:
    lines = f.readlines()

value_entries = [i for i in lines if "pcf8591_value=" in i]
start_entries = [i for i in lines if "pcf8591_thread" in i]

for i in range(0, len(value_entries)):
    temp_arr_value = value_entries[i].split()
    temp_value = int(temp_arr_value[-1], 16)
    value.append(temp_value)
    value_str.append(format(temp_value, '#x'))

    temp_timestamp = str(temp_arr_value[-3])
    temp_timestamp = temp_timestamp.replace("[", "")
    temp_timestamp = temp_timestamp.replace("]", "")
    temp_timestamp = float(temp_timestamp)
    timestamp.append(temp_timestamp)

    temp_arr_start = start_entries[i].split()
    temp_start = str(temp_arr_start[-2])
    temp_start = temp_start.replace("[", "")
    temp_start = temp_start.replace("]", "")
    temp_start = float(temp_start)
    start.append(temp_start)

for i in range(1, len(timestamp)):
    period.append(round(timestamp[i] - timestamp[i - 1], 6))
    
for i in range(0, len(start)):
    execution_time.append(round(timestamp[i] - start[i], 6))

mean_period = statistics.mean(period)
mean_period *= 1000
mean_period = round(mean_period, 6)

stdev_period = statistics.stdev(period)
stdev_period *= 1000
stdev_period = round(stdev_period, 6)

mean_execution_time = statistics.mean(execution_time)
mean_execution_time *= 1000
mean_execution_time = round(mean_execution_time, 6)

stdev_execution_time = statistics.stdev(execution_time)
stdev_execution_time *= 1000
stdev_execution_time = round(stdev_execution_time, 6)

sample_period_ms = [i for i in lines if "pcf8591 sample_period_ms" in i]
sample_period_ms = sample_period_ms[0].split()
sample_period_ms = sample_period_ms[-1]
sample_period_ms = float(sample_period_ms)

error_requested_sample_period = mean_period - sample_period_ms
error_requested_sample_period = round(error_requested_sample_period, 6)

# print(timestamp)
# print(value)
# print(value_str)

print("mean_execution_time= " + str(mean_execution_time) + " ms")
print("stdev_execution_time= " + str(stdev_execution_time) + " ms")
print("mean_period= " + str(mean_period) + " ms")
print("stdev_period= " + str(stdev_period) + " ms")
print("error_requested_sample_period= " + str(error_requested_sample_period) + " ms")

plt.plot(value)
plt.xlabel('sample_number')
plt.ylabel('adc_value')
plt.grid()
plt.savefig("analysis.png")
# plt.show()