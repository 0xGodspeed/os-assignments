# q1
- It is necessary that there is a fixed order between acquiring forks and bowls. In our implementation, forks are _always_ acquired before bowls. This is to prevent deadlock. If a philosopher acquires a bowl before a fork, it is possible that the philosopher will be stuck waiting for a fork that is held by another philosopher. This is a deadlock.

- to avoid deadlock when all philosophers acquire the left fork first, we can have one philosopher acquire the right fork first. This will prevent the deadlock from happening.

- a mutex is used to prevent multiple philosophers from acquiring the same fork at the same time. 
