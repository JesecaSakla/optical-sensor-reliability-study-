"""Reproduce the portfolio results figure from the preserved summary table.

This script uses condition-level summary values only. It does not reconstruct
missing raw Serial Monitor data.
"""
from pathlib import Path
import csv
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / "data" / "summary_results.csv"
OUT = ROOT / "figures" / "results_summary.png"

with DATA.open(newline="") as f:
    rows = list(csv.DictReader(f))

conditions = [r["condition"] for r in rows]
means = [float(r["mean_raw_ir_au"]) for r in rows]
unstable = [int(r["invalid_unstable_count"]) for r in rows]

x = list(range(len(conditions)))
fig, ax1 = plt.subplots(figsize=(10, 6))

bars = ax1.bar(x, means)
ax1.set_yscale("log")
ax1.set_ylabel("Mean raw IR reading (a.u., log scale)")
ax1.set_xlabel("Testing condition")
ax1.set_xticks(x, conditions)
ax1.set_title("Mean Raw IR and Unstable Reading Count by Condition")

for bar, value in zip(bars, means):
    ax1.text(
        bar.get_x() + bar.get_width()/2,
        value * 1.08,
        f"{value:,.2f}",
        ha="center",
        va="bottom",
        fontsize=8,
    )

ax2 = ax1.twinx()
ax2.plot(x, unstable, marker="o")
ax2.set_ylabel("Invalid/unstable readings (count)")
ax2.set_ylim(0, max(unstable) + 4)

for i, value in enumerate(unstable):
    ax2.text(i, value + 0.5, str(value), ha="center", va="bottom", fontsize=8)

fig.tight_layout()
fig.savefig(OUT, dpi=220, bbox_inches="tight")
print(f"Saved {OUT}")
