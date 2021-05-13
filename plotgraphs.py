import pandas as pd
import csv
from matplotlib import style
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as mticker

style.use('ggplot')

headers = ['Date','Time','Temperature','Humidity','Moisture']
df = pd.read_csv(r'C:\Users\jabez\OneDrive\Desktop\combinelogs.txt',names=headers,header=None)
#print (df)

x = df['Time']
t = df['Temperature']
h= df['Humidity']
m=df['Moisture']

# plot
fig, ((ax1),(ax2),(ax3)) = plt.subplots(3,1,sharex=True,figsize=(15,10))

ax1.plot(x, t, 'g') #row=0
ax1.set_xlabel('time')
ax1.set_ylabel('temp ∘C')
ax1.set_title('Temperature')
ax2.plot(x, h, 'k') #row=1
ax2.set_xlabel('time')
ax2.set_ylabel('humidity')
ax2.set_title('Humidity')
ax3.plot(x, m, 'b') #row=2
ax3.set_xlabel('time')
ax3.set_ylabel('moisture')
ax3.set_title('Moisture')

#preventing the x axis labels from overlapping
for label in ax1.xaxis.get_ticklabels():
        label.set_rotation(45)
myLocator = mticker.MultipleLocator(15)
ax1.xaxis.set_major_locator(myLocator)

fig.autofmt_xdate() # for spacing b/w x axis time
fig.tight_layout() #for spacing b/w subplots
