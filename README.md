*This project has been created as part of the 42 curriculum by gdosch.*

# Philosophers 🍝

> "I never thought philosophy would be so deadly."

## Description

This project is an algorithmic exercise that introduces the basics of concurrent programming, specifically multithreading and multiprocessing. The goal is to solve the classic **Dining Philosophers problem**, learning how to synchronize state and prevent fatal issues like deadlocks and data races.

The repository contains two distinct implementations:
- **Mandatory (`philo`)**: Uses **multithreading** (one thread per philosopher) and **mutexes** to protect the state of the forks (one fork between each pair of philosophers).
- **Bonus (`philo_bonus`)**: Uses **multiprocessing** (one process per philosopher) and POSIX **semaphores** to represent the pool of available forks placed in the middle of the table.

### Overview
In this simulation, a number of philosophers sit at a round table with a large bowl of spaghetti in the middle. The philosophers take turns to **eat, think, or sleep**.
While they are eating, they are not thinking nor sleeping; while thinking, they are not eating nor sleeping; and, of course, while sleeping, they are not eating nor thinking.

Because eating spaghetti with only one fork is impractical, a philosopher must take two forks to eat. When a philosopher has finished eating, they put their forks back on the table and start sleeping. Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation. Every philosopher needs to eat and should never starve. Philosophers don't speak with each other and don't know if another philosopher is about to die.

## Instructions

### 1. Compilation
Clone the repository and compile the desired version (`philo` or `philo_bonus`):

```bash
# For the mandatory part (threads & mutexes)
cd philo
make

# For the bonus part (processes & semaphores)
cd philo_bonus
make
```
Other available Makefile rules: `clean`, `fclean`, `re`.

### 2. Execution
Both programs take the exact same arguments:
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```
*(or `./philo_bonus`)*

* `number_of_philosophers`: The number of philosophers and forks.
* `time_to_die` (in ms): If a philosopher didn't start eating `time_to_die` ms since the beginning of their last meal or the beginning of the simulation, they die.
* `time_to_eat` (in ms): The time it takes for a philosopher to eat. During that time, they will hold two forks.
* `time_to_sleep` (in ms): The time a philosopher will spend sleeping.
* `number_of_times_each_philosopher_must_eat` (optional argument): If all philosophers have eaten at least this many times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

**Examples:**
```bash
# 5 philosophers, they die if they don't eat within 800ms, it takes 200ms to eat and 200ms to sleep.
./philo 5 800 200 200

# Simulator stops if all 5 philosophers eat at least 7 times.
./philo_bonus 5 800 200 200 7
```

## Resources

* [POSIX Threads Programming (pthreads)](https://computing.llnl.gov/tutorials/pthreads/)
* [Dining Philosophers Problem (Wikipedia)](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* [Mutexes and threads explained](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/)
* [Semaphores in C](https://www.geeksforgeeks.org/use-posix-semaphores-c/)

### AI Usage
AI tools were used for generating ideas for documentation layout, reviewing the commit strategy, and finding initial references on thread synchronization. Specifically:
- Formatting the README and structuring the documentation to comply with subject requirements.
- Reviewing performance updates and writing concise, normalized git commit messages.
- Providing brief explanations of standard functions such as `gettimeofday`, `pthread_mutex_init`, and `sem_open`.
