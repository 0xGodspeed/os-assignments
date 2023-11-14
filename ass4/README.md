# Q1 - Dining Philosophers 
## Why can deadlocks occur?
- Deadlocks can occur when all philosophers acquire the left fork first. This will cause all philosophers to be stuck waiting for the right fork. 
- If some philosophers acquire the bowls first, and the others acqure the forks first or vice-versa then deadlock will occur since all conditions for eating will never be met.

## How our solution avoids deadlocks
- It is necessary that there is a fixed order between acquiring forks and bowls. In our implementation, forks are _always_ acquired before bowls. This is to prevent deadlock. If a philosopher acquires a bowl before a fork, it is possible that the philosopher will be stuck waiting for a fork that is held by another philosopher. This is a deadlock.
- to avoid deadlock when all philosophers acquire the left fork first, we can have one philosopher acquire the right fork first. This will prevent the deadlock from happening.
- a mutex is used to prevent multiple philosophers from acquiring the same fork/bowl at the same time. 

## Why our solution is starvation free
- In our solution, when a philosopher is done thinking and eating, it puts down both forks and waits for some time before trying to eat again. This makes sure other philosophers do not starve.
- A philosopher does not have to wait for a specific bowl, it can eat from any bowl that is available. This also reduces the chances of starvation.
- Also, since the number of philosophers is finite and not too big in this case (5) and the sleep time is random within a limit, it is very probable that eventually, all philosophers will get a chance to eat.

## Implementation details
- Bowls have been implemented using a single conditional variable which signals/waits for any bowl that is available. An integer array is used to keep track of which bowls are available and which bowl is held by which philosopher. Each philosopher loops through the array and checks if a bowl is available; if not it waits for the conditional variable to be signalled. When a philosopher is done eating, it puts down both forks and bowls (the array is updated) and signals the conditional variable. After the conditional variable is signalled and the code resumes, we loop through the array again to check which bowl was made available and update the array again.
- For forks, there is a conditional variable for each fork. Each philosopher waits for its respective left and right fork (in that order). The conditional variable of the respective fork is signaled when the fork is put down. 
