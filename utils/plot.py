#!/usr/bin/env python3

import sys
import numpy as np
from matplotlib import pyplot as plt

data = np.memmap(sys.argv[1], dtype="float32", mode="r")
plt.rcParams["figure.figsize"] = (12, 7)
frame1 = plt.gca()
frame1.axes.xaxis.set_ticklabels([])
plt.plot(data)

plt.savefig(sys.argv[2])
