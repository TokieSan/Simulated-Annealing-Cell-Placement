import matplotlib.pyplot as plt
import os
design = input("Enter the design you want to do the graph for: ")
cooling_rate = 0.95
#cooling_rate = float(input("Enter the cooling rate you want for this file: "))
filename = "../"+design+"_logs/"+design+"_log_"+str(format(cooling_rate, ".2f"))
temp_values = []
TWL_values = []
with open(filename, "r") as file:
    for line in file:
        value1, value2 = line.strip().split()
        temp_values.append(float(value1))
        TWL_values.append(int(value2))

temp_values = temp_values[::-1]
TWL_values = TWL_values[::-1]

plt.figure(figsize=(10, 6))
plt.plot(temp_values, TWL_values, 'bo-', label='Data points')
plt.gca().invert_xaxis()

# Add title and labels
plt.title(design + "TWL_vs_Temperature")
plt.xlabel('Temperature')
plt.ylabel('TWL')
plt.ylim(min(TWL_values)-100, max(TWL_values)+100)  # Set the y-axis range from 0 to 10

# Add legend
plt.legend()

# Save the plot as a PNG image
plt.savefig(design + "_"+str(cooling_rate)+"_"+" TWL_vs_temperature.png")

# Optionally, display the plot
# plt.show()

print("Plot saved as 'plot.png'")