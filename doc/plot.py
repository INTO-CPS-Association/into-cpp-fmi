from pandas import DataFrame, read_csv
import matplotlib.pyplot as plt
import pandas as pd
# define data location
df = read_csv('stable/result.csv')

fig, axarr = plt.subplots(2, sharex=True)

#fig = plt.figure()
ax1=axarr[0]

ax1.title.set_text('stable')
name='{FMU2}.FMU2Instance.fk'
ax1.plot(df['time'], df[name], label=name)

name='{FMU}.FMUInstance.x1'
ax1.plot(df['time'], df[name], label=name)

name='{FMU}.FMUInstance.v1'
ax1.plot(df['time'], df[name], label=name)

df = read_csv('unstable/result.csv')
#fig = plt.figure()
ax2 = axarr[1]
ax2.title.set_text('un-stable')

name='{FMU2}.FMU2Instance.fk'
ax2.plot(df['time'], df[name], label=name)

name='{FMU}.FMUInstance.x1'
ax2.plot(df['time'], df[name], label=name)

name='{FMU}.FMUInstance.v1'
ax2.plot(df['time'], df[name], label=name)

plt.ylim( (-40, 40) )

plt.legend(bbox_to_anchor=(1, 1),
           bbox_transform=plt.gcf().transFigure)

plt.show()
