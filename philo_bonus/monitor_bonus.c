/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:09:12 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/15 13:31:33 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static long	ft_elapsed_time(t_philo *philo)
{
	const long	current_time = ft_get_time(MILLISECOND);

	if (current_time < 0)
		ft_abort(philo->data);
	return (current_time - philo->data->start_time);
}

static void	ft_write_state_debug(t_ps state, t_philo *philo)
{
	if (state == TAKING_FIRST_FORK)
		printf("%6ld | Philo No.%d (ID %d) has taken the 1st fork\n",
			ft_elapsed_time(philo), philo->id + 1, philo->id);
	else if (state == TAKING_SECOND_FORK_AND_EATING)
		printf("%6ld | Philo No.%d (ID %d) has taken the 2nd fork\n"
			"%6ld | Philo No.%d (ID %d) is eating meal No.%ld\n",
			ft_elapsed_time(philo), philo->id + 1, philo->id,
			ft_elapsed_time(philo), philo->id + 1, philo->id,
			philo->meal_ct + 1);
	else if (state == SLEEPING)
		printf("%6ld | Philo No.%d (ID %d) is sleeping\n",
			ft_elapsed_time(philo), philo->id + 1, philo->id);
	else if (state == THINKING)
		printf("%6ld | Philo No.%d (ID %d) is thinking\n",
			ft_elapsed_time(philo), philo->id + 1, philo->id);
	else if (state == DIED)
		printf("%6ld | Philo No.%d (ID %d) died\n",
			ft_elapsed_time(philo), philo->id + 1, philo->id);
}

void	ft_abort(t_data *data)
{
	sem_post(data->stop_sem);
	exit(EXIT_FAILURE);
}

void	ft_write_state(t_ps state, t_philo *philo)
{
	sem_wait(philo->data->write_sem);
	if (DEBUG_MODE)
		ft_write_state_debug(state, philo);
	else if (state == TAKING_FIRST_FORK)
		printf("%ld %d has taken a fork\n",
			ft_elapsed_time(philo), philo->id + 1);
	else if (state == TAKING_SECOND_FORK_AND_EATING)
		printf("%ld %d has taken a fork\n" "%ld %d is eating\n",
			ft_elapsed_time(philo), philo->id + 1,
			ft_elapsed_time(philo), philo->id + 1);
	else if (state == SLEEPING)
		printf("%ld %d is sleeping\n", ft_elapsed_time(philo), philo->id + 1);
	else if (state == THINKING)
		printf("%ld %d is thinking\n", ft_elapsed_time(philo), philo->id + 1);
	else if (state == DIED)
		printf("%ld %d died\n", ft_elapsed_time(philo), philo->id + 1);
	if (state != DIED)
		sem_post(philo->data->write_sem);
}

void	*ft_monitor(void *arg)
{
	t_philo	*philo;
	t_data	*d;
	long	current_time;
	long	last_meal_time;

	philo = (t_philo *)arg;
	d = philo->data;
	while (1)
	{
		last_meal_time = ft_sem_get(philo->lock_sem, &philo->last_meal_time);
		current_time = ft_get_time(MILLISECOND);
		if (current_time < 0)
			ft_abort(d);
		if (current_time - last_meal_time >= d->time_to_die / 1e3)
		{
			ft_write_state(DIED, philo);
			ft_abort(d);
		}
		usleep(750);
	}
	return (NULL);
}
