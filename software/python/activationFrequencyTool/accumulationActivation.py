''' Tool for determining mean time to activation given a set of parameters.

Imagines activation as a filling capacitor/bucket. Activation occurs at a time step when some normally distributed
random variable returns a result greater than the level of the bucket. If the bucket fills by a known amount with each
time step, this calculates how many timesteps are needed to trigger a bucket on average.

This is valid partially as a function of time and the number of checks (because the probability of occurance of a
random variable below any value goes up with the number of checks. Thus, it is important that the random variable is
only regenerated every millisecond for this to be true.
'''
import math
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

MEAN = 25000
STD = 2000
N_REPS = 10000

def main(args):

    t = np.arange(args.plot_time)

    # accumulates at 1/time
    acc = np.arange(args.plot_time)

    tipping = []

    for i in range(args.reps):
        if args.randmethod == 'normal':
            randdist = np.random.normal(args.time, args.std, size=(args.plot_time))
        elif args.randmethod == 'uniform_sq':
            r1 = np.random.random_integers(0, int(math.sqrt(args.time*2)), size=(args.plot_time))
            r2 = np.random.random_integers(0, int(math.sqrt(args.time*2)), size=(args.plot_time))
            randdist = r1*r2
        gt = np.where(acc > randdist)[0][0]

        tipping.append(gt)

    # the histogram of the data
    n, bins, patches = plt.hist(tipping, 50, normed=1, facecolor='green', alpha=0.75)

    # add a 'best fit' line
    y_o = mlab.normpdf( t, args.time, args.std)
    l = plt.plot(t, y_o, 'b--', linewidth=1)
    y = mlab.normpdf( t, np.mean(tipping), np.std(tipping))
    l = plt.plot(t, y, 'r--', linewidth=1)

    print('On average, the system is triggered every %d timesteps.' %np.mean(tipping))

    plt.xlabel('Time (ms)')
    plt.ylabel('Proportion')
    plt.title(r'Time to Activation $(n=%d)\ \mu=%d,\ \sigma=%d$' %(args.reps, args.time, args.std))
    plt.axis([0, args.plot_time, 0, np.max(n)*1.5])
    plt.grid(True)

    plt.show()

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--time', type=int, default=MEAN, help='Time in milliseconds as a mean for the normal distribution.')
    parser.add_argument('--std', type=int, default=STD, help='Standard deviation for normal distribution.')
    parser.add_argument('-n', '--reps', type=int, default=N_REPS, help='Number of repetitions of the experiment.')
    parser.add_argument('--plot-time', type=int, default=-1, help='Length of the plot.')
    args = parser.parse_args()

    if args.plot_time == -1:
        args.plot_time = args.time*2

    print( args )

    main(args)