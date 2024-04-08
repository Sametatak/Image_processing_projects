import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

left_uwp_map = pd.read_csv('C:/Users/samet/Desktop/opencvtest/task3/right_uwp_map.csv', header=None)
right_uwp_map = pd.read_csv('C:/Users/samet/Desktop/opencvtest/task3/right_uwp_map.csv', header=None)
filtered_uwp_map = pd.read_csv('C:/Users/samet/Desktop/opencvtest/task3/disparity_map.csv', header=None)

# Plot both phase maps for visual inspection
fig, axes = plt.subplots(1, 3, figsize=(20, 10))
sns.heatmap(left_uwp_map, ax=axes[0], cmap='viridis')
axes[0].set_title('Left Camera Phase Map')
sns.heatmap(right_uwp_map, ax=axes[2], cmap='viridis')
axes[2].set_title('right phase map')
sns.heatmap(filtered_uwp_map, ax=axes[1], cmap='viridis')
axes[1].set_title('filtered phase map')

plt.show()