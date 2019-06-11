import pandas as pd
import matplotlib.pyplot as plt
import configparser

plt.style.use('seaborn')
plt.figure(figsize=(15,12))

config = configparser.ConfigParser()
config.read('images/out32.csv.ini')
metadata = config['metadata']

framerate = float(metadata['framerate'])
asset_id = metadata['asset_id']
compilation_id = metadata['compilation_id']


df = pd.read_csv('images/out32.csv')
df['upper'] = df['avg'] + 2*df['std']
df['lower'] = df['avg'] - 2*df['std']
df['seconds'] = df['frame'] / framerate

fig = plt.figure()

plt.plot(df['seconds'], df['raw'], 'r', alpha=.25)
plt.plot(df['seconds'], df['bin'], 'k')
plt.plot(df['seconds'], df['avg'], 'r')
plt.plot(df['seconds'], 2*df['std'], 'b', alpha=0.3)
plt.plot(df['seconds'], df['min_avg'], 'g', alpha=0.6)
plt.plot(df['seconds'], 2*df['max_std'], 'g', alpha=0.6)
plt.fill_between(df['seconds'], 2*df['std'], color='b', alpha=.25)
plt.fill_between(df['seconds'], df['min_avg'], 1, alpha=0.25, color='g')
plt.fill_between(df['seconds'], 0, 2*df['max_std'], alpha=0.25, color='g')

fig.suptitle('Comparison: \n[compilation: ' + compilation_id + '], [asset: ' + asset_id + ']', fontsize=14)
plt.ylabel('similarity', fontsize=12)
plt.xlabel('seconds (compilation)', fontsize=12)

plt.legend(['raw similarity','1 if match, 0 if no match','filtered similarity','std of similarity'])

plt.savefig('images/out32.png')
