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
    # interpolate the points using a polynomial of degree 3
    p = np.polyfit(x, y, 3)
    # generate 50 points between the min and max x values
    xp = np.linspace(x.min(), x.max(), 50)
    # evaluate the polynomial at the generated points
    yp = np.polyval(p, xp)
    # return the generated points and values
    return xp, yp

async def parallel_interpolate(x, y, loop):
    with concurrent.futures.ThreadPoolExecutor(max_workers=mp.cpu_count()) as executor:
        futures = [
            loop.run_in_executor(
                executor,
                interpolate,
                x[i:i+int(len(x)/mp.cpu_count())],
                y[i:i+int(len(y)/mp.cpu_count())]
            )
            for i in range(0, len(x), int(len(x)/mp.cpu_count()))
        ]
        results = await asyncio.gather(*futures)
        return results

if __name__ == '__main__':
    n = 1000000
    x, y = generate_points(n)

    # plot the random points
    plt.scatter(x, y, label='Random Points')

    # interpolate the points sequentially
    start_time = time.time()
    sequential_interp = interpolate(x, y)
    sequential_time = time.time() - start_time
    print("Sequential time:", sequential_time)

    # interpolate the points in parallel
    loop = asyncio.get_event_loop()
    start_time = time.time()
    results = loop.run_until_complete(parallel_interpolate(x, y, loop))
    loop.close()
    parallel_time = time.time() - start_time
    print("Parallel time:", parallel_time)

    # plot the interpolated curves in parallel
    for xp, yp in results:
        plt.plot(xp, yp, label='Parallel Interpolation')

    # display the plot
    plt.legend()
    plt.show()