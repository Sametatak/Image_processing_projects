import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv("phaseRow.csv", header=None, names=['Phase'])


plt.figure(figsize=(10, 6))
plt.plot(df.index, df['Phase'], label='Unwrapped Phase')
plt.title('Unwrapped Phase for a Selected Row')
plt.xlabel('Pixel')
plt.ylabel('Phase (radians)')
plt.legend()
plt.grid(True)
plt.show()
