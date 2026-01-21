import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results/trajectories_verlet.csv')
t = df.iloc[:, 0]
E = df['energy'] if "energy" in df.columns else df.iloc[:, -1]

E0 = E.iloc[0]
drift = (E - E0) / abs(E0 + 1e-12)

plt.figure()
plt.plot(t, drift)
plt.xlabel("time")
plt.ylabel("relative energy drift")
plt.title("Energy drift over time")
plt.savefig("energy_drift.png", dpi=200, bbox_inches='tight')
print("Wrote energy_drift.png")