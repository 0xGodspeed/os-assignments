import plotly.express as px

data = open("execution_times.txt").readlines()

values = [int(i.split()[-2]) for i in data]
labels = ['SCHED_OTHER', 'SCHED_FIFO', 'SCHED_RR']

fig = px.histogram(x=labels, y=values, title="Execution times", labels={'x': 'Scheduler', 'y': 'Execution time (ns)'}, width=700, height=700, color=labels)
fig.update_layout(legend_title_text='Scheduling Policy')
fig.show()