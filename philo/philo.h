/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:34:54 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/15 18:46:11 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// Include(s)
# include <limits.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

// Define(s)
# define DEBUG_MODE 0

// Typedef(s)
typedef pthread_mutex_t	t_mutex;
typedef struct s_data	t_data;

// structure(s)
typedef struct s_fork
{
	int			id;
	t_mutex		mutex;
}	t_fork;

typedef struct s_philo
{
	int			id;
	pthread_t	thread;
	t_fork		*first_fork;
	t_fork		*second_fork;
	long		last_meal_time;
	long		meal_ct;
	long		is_full;
	t_mutex		mutex;
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
	long		end_sim;
	long		all_threads_ready;
	long		mutex_init;
	pthread_t	monitor;
	t_mutex		state_mutex;
	t_mutex		write_mutex;
	t_fork		*fork;
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
	MILLISECOND,
	MICROSECOND
}	t_tc;

// sim.c
int		ft_sim(t_data *data);

// monitor.c
void	*ft_monitor(void *data);
void	ft_write_state(t_ps state, t_philo *philo);

// mutex.c
void	ft_mutexes_destroy(int n, t_data *data);
void	ft_mutex_set(t_mutex *mutex, long *dest, long value);
long	ft_mutex_get(t_mutex *mutex, long *value);
long	ft_sim_is_over(t_data *data);

// utils.c
long	ft_atol_s(const char *nptr);
int		ft_error(const char *err_msg, int exit_code);
long	ft_get_time(t_tc time_code, t_data *data);
void	ft_usleep(long sleep_time, t_data *data);
void	ft_cleanup(t_data *data);

#endif
