import statistics

value = []
value_str = []
timestamp = []
period = []

with open('demo.txt') as f:
    lines = f.readlines()

value_entries = [s for s in lines if "pcf8591_value=" in s]

for i in range(0, len(value_entries)):
    temp_arr = value_entries[i].split()

    temp_value = int(temp_arr[3], 16)
    value.append(temp_value)
    value_str.append(format(temp_value, '#x'))

    temp_timestamp = temp_arr[1]
    temp_timestamp = temp_timestamp[:-1]
    temp_timestamp = float(temp_timestamp)

    timestamp.append(temp_timestamp)

for i in range(1, len(timestamp)):
    period.append(round(timestamp[i] - timestamp[i - 1], 6))

mean_period = round(statistics.mean(period), 6)
stdev_period = round(statistics.stdev(period), 6)

# print(timestamp)
# print(value)
# print(value_str)

print("mean_period= " + str(mean_period) + " s, stdev_period= " + str(stdev_period) + " s")
