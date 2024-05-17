import matplotlib.pyplot as plt
import os
design = input("Enter the design you want to do the graph for: ")
files = [design + "_log_"+ str(format(i/100,".2f")) for i in range(75, 100,5)]
Cooling_rate_vals = [i/100 for i in range(75, 100,5)]
TWL_vals = []
count = 0

for i in range(75,100,5):
    with open("../"+design+"_logs/" + files[count], "r") as file:
        twl = 0
        for line in file:
            val1, val2 = line.strip().split()
            twl = int(val2)
        TWL_vals.append(twl)
    count += 1

plt.figure(figsize=(10, 6))
plt.plot(Cooling_rate_vals, TWL_vals, 'bo-', label='Data points')

# Add title and labels
plt.title(design + "TWL_vs_Cooling_Rate")
plt.xlabel('Cooling Rate')
plt.ylabel('TWL')
plt.ylim(min(TWL_vals)-10, max(TWL_vals)+10)  # Set the y-axis range from 0 to 10

# Add legend
plt.legend()

# Save the plot as a PNG image
plt.savefig(design + " TWL_vs_Cooling_Rate.png")

# Optionally, display the plot
# plt.show()

print("Plot saved as 'plot.png'")