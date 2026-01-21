import pandas as pd
import matplotlib.pyplot as plt

bench = pd.read_csv('results/bench.csv')

plt.figure()
plt.plot(bench['N'], bench["seconds"], marker='o')
plt.xlabel("N bodies")
plt.ylabel("seconds (log scale)")
plt.title("Runtime vs N")
plt.yscale("log")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.savefig("assets/bench.png", dpi=200, bbox_inches='tight')
print("Wrote bench.png to /assets")