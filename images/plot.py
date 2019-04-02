import pandas as pd
import matplotlib.pyplot as plt

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
plt.fill_between(df.index, 2*df['std'], color='b', alpha=.25)
plt.fill_between(df.index, df['min_avg'], 1, alpha=0.25, color='g')
plt.fill_between(df.index, 0, 2*df['max_std'], alpha=0.25, color='g')

plt.savefig('images/out32.png')
