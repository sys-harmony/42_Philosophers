/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:05:04 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/17 14:16:59 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_write_state_debug(t_ps state, t_philo *philo, long elapsed_time)
{
	if (state == TAKING_FIRST_FORK)
		printf("%6ld | Philo No.%d (ID %d) has taken the 1st fork (ID %d)\n",
			elapsed_time, philo->id + 1, philo->id, philo->first_fork->id);
	else if (state == TAKING_SECOND_FORK_AND_EATING)
		printf("%6ld | Philo No.%d (ID %d) has taken the 2nd fork (ID %d)\n"
			"%6ld | Philo No.%d (ID %d) is eating meal No.%ld\n",
			elapsed_time, philo->id + 1, philo->id, philo->second_fork->id,
			elapsed_time, philo->id + 1, philo->id, philo->meal_ct + 1);
	else if (state == SLEEPING)
		printf("%6ld | Philo No.%d (ID %d) is sleeping\n",
			elapsed_time, philo->id + 1, philo->id);
	else if (state == THINKING)
		printf("%6ld | Philo No.%d (ID %d) is thinking\n",
			elapsed_time, philo->id + 1, philo->id);
	else if (state == DIED)
		printf("%6ld | Philo No.%d (ID %d) died\n",
			elapsed_time, philo->id + 1, philo->id);
}

void	ft_write_state(t_ps state, t_philo *philo)
{
	long	elapsed_time;
	long	current_time;

	pthread_mutex_lock(&philo->data->write_mutex);
	current_time = ft_get_time(MS, philo->data);
	if (!ft_sim_is_over(philo->data) || state == DIED)
	{
		elapsed_time = current_time - philo->data->start_time;
		if (DEBUG_MODE)
			ft_write_state_debug(state, philo, elapsed_time);
		else if (state == TAKING_FIRST_FORK)
			printf("%ld %d has taken a fork\n", elapsed_time, philo->id + 1);
		else if (state == TAKING_SECOND_FORK_AND_EATING)
			printf("%ld %d has taken a fork\n" "%ld %d is eating\n",
				elapsed_time, philo->id + 1, elapsed_time, philo->id + 1);
		else if (state == SLEEPING)
			printf("%ld %d is sleeping\n", elapsed_time, philo->id + 1);
		else if (state == THINKING)
			printf("%ld %d is thinking\n", elapsed_time, philo->id + 1);
		else if (state == DIED)
			printf("%ld %d died\n", elapsed_time, philo->id + 1);
	}
	pthread_mutex_unlock(&philo->data->write_mutex);
}

static long	ft_fed_or_dead(t_philo *philo)
{
	long	current_time;
	long	last_meal_time;
	long	is_full;

	pthread_mutex_lock(&philo->mutex);
	last_meal_time = philo->last_meal_time;
	is_full = philo->is_full;
	pthread_mutex_unlock(&philo->mutex);
	current_time = ft_get_time(MS, philo->data);
	if (ft_sim_is_over(philo->data))
		return (0);
	if (current_time - last_meal_time >= philo->data->time_to_die / 1e3
		&& !is_full)
	{
		ft_mutex_set(&philo->data->state_mutex, &philo->data->end_sim, 1);
		ft_write_state(DIED, philo);
		return (0);
	}
	return (is_full);
}

void	*ft_monitor(void *data)
{
	t_data	*d;
	int		full_count;
	int		i;

	d = (t_data *)data;
	while (!ft_mutex_get(&d->state_mutex, &d->all_threads_ready))
		usleep(100);
	while (!ft_sim_is_over(d))
	{
		i = -1;
		full_count = 0;
		while (++i < d->philo_nbr && !ft_sim_is_over(d))
			if (ft_fed_or_dead(&d->philo[i]))
				full_count++;
		if (d->max_meals > 0 && full_count == d->philo_nbr)
			ft_mutex_set(&d->state_mutex, &d->end_sim, 1);
		usleep(250);
	}
	return (NULL);
}
