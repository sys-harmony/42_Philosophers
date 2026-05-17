/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:09:06 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/17 14:49:10 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>

// Define(s)
# define DEBUG_MODE 0

// typedef(s)
typedef struct s_data	t_data;

// structure(s)
typedef struct s_philo
{
	int			id;
	pthread_t	thread;
	long		last_meal_time;
	long		meal_ct;
	sem_t		*lock_sem;
	t_data		*data;
}	t_philo;

struct s_data
{
	long		philo_nbr;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		think_time;
	long		max_meals;
	long		start_time;
	long		philo_sem_init;
	sem_t		*forks_sem;
	sem_t		*diners_sem;
	sem_t		*write_sem;
	sem_t		*done_sem;
	sem_t		*stop_sem;
	pid_t		*pid;
	t_philo		*philo;
};

// enum(s)
typedef enum e_philo_state
{
	TAKING_FIRST_FORK,
	TAKING_SECOND_FORK_AND_EATING,
	SLEEPING,
	THINKING,
	DIED
}	t_ps;

typedef enum e_time_code
{
	MS,
	US
}	t_tc;

// sim_bonus.c
int		ft_sim(t_data *data);

// monitor_bonus.c
void	*ft_monitor(void *data);
void	ft_write_state(t_ps state, t_philo *philo);
void	ft_abort(t_data *data);

// sem_bonus.c
char	*ft_sem_name(int id);
void	ft_remove_sem(sem_t *sem, const char *sem_name);
void	ft_remove_philo_sems(int n, t_data *data);
void	ft_sem_set(sem_t *sem, long *dest, long value);
long	ft_sem_get(sem_t *sem, long *value);

// utils_bonus.c
long	ft_atol_s(const char *nptr);
int		ft_error(const char *err_msg, int exit_code);
long	ft_get_time(t_tc time_code);
void	ft_usleep(long sleep_time, t_data *data);
void	ft_cleanup(t_data *data);

#endif
