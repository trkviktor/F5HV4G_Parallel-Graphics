import numpy as np
import matplotlib.pyplot as plt
import asyncio
import concurrent.futures
import multiprocessing as mp
import time

def generate_points(n):
    # generate n random points in the range [-5, 5]
    x = np.random.uniform(-5, 5, n)
    y = np.random.uniform(-5, 5, n)
    return x, y

def interpolate(x, y):
    p = np.polyfit(x, y, 3)
    xp = np.linspace(x.min(), x.max(), 50)
    yp = np.polyval(p, xp)

    return xp, yp

async def parallel_interpolate(x, y, loop, num_threads):
    with concurrent.futures.ThreadPoolExecutor(max_workers=num_threads) as executor:
        futures = [
            loop.run_in_executor(
                executor,
                interpolate,
                x[i:i+int(len(x)/num_threads)],
                y[i:i+int(len(y)/num_threads)]
            )
            for i in range(0, len(x), int(len(x)/num_threads))
        ]
        results = await asyncio.gather(*futures)
        return results

if __name__ == '__main__':
    n = int(input("Enter the sample size: "))
    num_threads = int(input("Enter the number of threads: "))

    x, y = generate_points(n)

    # interpolate the points sequentially
    start_time = time.time()
    sequential_interp = interpolate(x, y)
    sequential_time = time.time() - start_time
    print("Sequential time:", sequential_time)

    # interpolate the points in parallel
    loop = asyncio.get_event_loop()
    start_time = time.time()
    results = loop.run_until_complete(parallel_interpolate(x, y, loop, num_threads))
    loop.close()
    parallel_time = time.time() - start_time
    print("Parallel time:", parallel_time)

    # plot the interpolated curves sequentially
    plt.plot(sequential_interp[0], sequential_interp[1], label='Sequential Interpolation')

    # plot the interpolated curves in parallel
    for xp, yp in results:
        plt.plot(xp, yp, label='Parallel Interpolation')

    # label the x and y axes
    plt.xlabel('X')
    plt.ylabel('Y')

    # display the plot
    plt.legend()
    plt.show()
