import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import cm
import scipy.signal
import numpy as np

WINDOW_SIZE = 120
BOTTOM = 0.4
TOP = 1

plt.style.use('seaborn')
plt.figure(figsize=(10,8))

df = pd.read_csv('images/out32.csv')
df['upper'] = df['avg'] + 2*df['std']
df['lower'] = df['avg'] - 2*df['std']

plt.plot(df['raw'], 'r', alpha=.25)
plt.plot(df['bin'], 'k')
plt.plot(df['avg'], 'r')
plt.plot(2*df['std'], 'b', alpha=0.3)
plt.plot(df['min_avg'], 'g', alpha=0.6)
plt.plot(2*df['max_std'], 'g', alpha=0.6)
#plt.fill_between(df.index, df.lower, df.upper, alpha=.25)
#plt.plot(2*df['std'], color='b', alpha=.25)
plt.fill_between(df.index, 2*df['std'], color='b', alpha=.25)
plt.fill_between(df.index, df['min_avg'], 1, alpha=0.25, color='g')
plt.fill_between(df.index, 0, 2*df['max_std'], alpha=0.25, color='g')

print(df['min_avg'])

plt.savefig('images/out32.png')
"""
plt.subplot(3, 1, 2)
plt.plot(df['avg'], 'k')
plt.ylim(bottom=BOTTOM,top=TOP)
plt.fill_between(df.index, df.lower, df.upper)
plt.fill_between(df.index, MIN_AVG, 1, alpha=0.25, color='g')

plt.subplot(3, 1, 3)
plt.scatter(df.index, df['std'].values, c=df['avg'].values, cmap=plt.cm.get_cmap('jet'), s=4+32*(df['avg'] > MIN_AVG).values, marker='o')
plt.fill_between(df.index, 0, MAX_STD, alpha=0.25, color='g')

plt.savefig('images/out32.png')


for HASH_SIZE in [8, 16, 32, 64, 128, 256]:
    df = pd.read_csv('images/out'+str(HASH_SIZE)+'.csv')
    df['std'] = df['x'].rolling(window=WINDOW_SIZE).std().shift(-WINDOW_SIZE//2)
    df['avg'] = df['x'].rolling(window=WINDOW_SIZE).mean().shift(-WINDOW_SIZE//2)
    df['upper'] = df['avg'] + 2*df['std']
    df['lower'] = df['avg'] - 2*df['std']

    df['v'] = np.where((df['avg'] > MIN_AVG) & (df['std'] < MAX_STD), 1, 0)
    

    plt.figure(figsize=(10,12))
    plt.subplot(3, 1, 1)
    #plt.plot(df['x'], 'r')
    #plt.ylim(bottom=BOTTOM,top=TOP)

    plt.plot(df['v'])
    plt.subplot(3, 1, 2)
    
    plt.plot(df.x, 'r')
    plt.plot(df.avg, 'k')
    plt.ylim(bottom=BOTTOM,top=TOP)
    plt.fill_between(df.index, df.lower, df.upper)
    plt.fill_between(df.index, MIN_AVG, 1, alpha=0.25, color='g')
    #plt.hlines(MIN_AVG, xmin=df.index[0], xmax=df.index[-1], colors='g')

    plt.subplot(3, 1, 3)
    plt.scatter(df.index, df['std'].values, c=df['avg'].values, cmap=plt.cm.get_cmap('jet'), s=4, marker='o')
    plt.ylim(bottom=0, top=0.15)
    plt.fill_between(df.index, 0, MAX_STD, alpha=0.25, color='g')
    #plt.hlines(MAX_STD, xmin=df.index[0], xmax=df.index[-1], colors='g')

    plt.savefig('images/out'+str(HASH_SIZE)+'.png')
"""