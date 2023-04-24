import statistics

value = []
value_str = []
timestamp = []
period = []
start = []
execution_time = []

with open('demo.txt') as f:
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

# print(timestamp)
# print(value)
# print(value_str)

print("mean_period= " + str(mean_period) + " ms, stdev_period= " + str(stdev_period) + " ms")
print("mean_execution_time= " + str(mean_execution_time) + " ms, stdev_execution_time= " + str(stdev_execution_time) + " ms")
