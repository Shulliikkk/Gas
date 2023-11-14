from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
T = 2000
step = 10
for i in range(0, T, step):
    arr = np.genfromtxt('data/data_{}.txt'.format(i))
    #x = arr[:, 0]
    #u = arr[:, 1]
    u = arr
    plt.plot(u)
    plt.ylim(0, 1)
    plt.savefig('img/plot_{}.png'.format(i))
    plt.close()
frames = []
for frame_number in range(0, T, step):
    frame = Image.open(f'img/plot_{frame_number}.png')
    frames.append(frame)
frames[0].save(
    'wave_1.gif',
    save_all = True,
    append_images = frames[1:],
    optimize = True,
    duration = 100,
    loop = 0
)
