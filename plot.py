import numpy as np
import matplotlib.pyplot as plt
import os
from matplotlib.animation import FuncAnimation

nz, nx = 648, 1150

model = np.fromfile("data/model_vp_2d_1150x648.bin", dtype=np.float32, count=nx * nz).reshape([nz, nx], order="C")
wavefield = np.fromfile("data/wavefield_result.bin", dtype=np.float32, count=nx * nz).reshape([nz, nx], order="C")

snapshots_files = np.sort(os.listdir("data/snapshots/"))

def ideal_parameters(dx, dz, vpMax):
    a0 = -2.8472222
    a1 = 1.6000000
    a2 = -0.2000000
    a3 = 0.0253968
    a4 = -0.0017857

    dh = np.sqrt(dx**2 + dz**2)
    A1 = abs(a0) + 2.0 * abs(a1) + 2.0 * abs(a2) + 2.0 * abs(a3) + 2.0 * abs(a4)
    dtmin = 0.35 * 2.0 * dh / np.sqrt(A1) / vpMax

    print(f"dh: {dh}, A1: {A1}, dtmin: {dtmin}")

def update(i):
    ax.clear()

    ax.imshow(model, cmap="jet", aspect="auto", alpha=0.3)
    
    snapshot = np.fromfile(f"data/snapshots/{snapshots_files[i]}", dtype=np.float32, \
                           count=nx * nz).reshape([nz, nx], order="C")

    scale = 0.9*np.std(snapshot)
    im = ax.imshow(snapshot, cmap="Greys", aspect="auto", alpha=0.5, vmin=-scale, vmax=scale)
    
    return im,

fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(10, 8))

anim = FuncAnimation(fig, update, frames=len(snapshots_files), interval=1500)

plt.show()

# ideal_parameters(dx=10, dz=10, vpMax=4500)



