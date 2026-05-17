/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:25:17 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/17 14:17:32 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_eat(t_philo *philo)
{
	t_data	*d;
	long	last_meal_time;

	d = philo->data;
	pthread_mutex_lock(&philo->first_fork->mutex);
	ft_write_state(TAKING_FIRST_FORK, philo);
	if (d->philo_nbr == 1)
		while (!ft_sim_is_over(d))
			usleep(100);
	else
	{
		pthread_mutex_lock(&philo->second_fork->mutex);
		last_meal_time = ft_get_time(MS, d);
		if (last_meal_time >= 0)
			ft_mutex_set(&philo->mutex, &philo->last_meal_time, last_meal_time);
		ft_write_state(TAKING_SECOND_FORK_AND_EATING, philo);
		if (!ft_sim_is_over(d) && ++philo->meal_ct == d->max_meals)
			ft_mutex_set(&philo->mutex, &philo->is_full, 1);
		if (!ft_sim_is_over(d))
			ft_usleep(d->time_to_eat, d);
		pthread_mutex_unlock(&philo->second_fork->mutex);
	}
	pthread_mutex_unlock(&philo->first_fork->mutex);
}

static void	*ft_dinner(void *data)
{
	t_philo	*philo;
	t_data	*d;

	philo = (t_philo *)data;
	d = philo->data;
	while (!ft_mutex_get(&d->state_mutex, &d->all_threads_ready))
		usleep(100);
	if (philo->id % 2)
		usleep(500);
	while (!ft_sim_is_over(d))
	{
		ft_eat(philo);
		if (ft_sim_is_over(d))
			break ;
		ft_write_state(SLEEPING, philo);
		ft_usleep(d->time_to_sleep, d);
		if (ft_sim_is_over(d))
			break ;
		ft_write_state(THINKING, philo);
		if (d->think_time > 0)
			ft_usleep(d->think_time, d);
	}
	return (NULL);
}

static int	ft_start_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
	{
		if (pthread_create(&data->philo[i].thread, NULL, ft_dinner,
				&data->philo[i]))
		{
			ft_mutex_set(&data->state_mutex, &data->end_sim, 1);
			return (ft_error("philo: pthread_create failed\n", i));
		}
	}
	if (pthread_create(&data->monitor, NULL, ft_monitor, data))
	{
		ft_mutex_set(&data->state_mutex, &data->end_sim, 1);
		return (ft_error("philo: pthread_create failed\n", i));
	}
	return (INT_MAX);
}

int	ft_sim(t_data *data)
{
	int	ret;
	int	exit_code;
	int	i;

	exit_code = 0;
	ret = ft_start_threads(data);
	if (ret == INT_MAX)
		data->start_time = ft_get_time(MS, data);
	if (ret != INT_MAX || data->start_time < 0)
		exit_code = 1;
	i = -1;
	while (++i < data->philo_nbr && !exit_code)
		ft_mutex_set(&data->philo[i].mutex, &data->philo[i].last_meal_time,
			data->start_time);
	ft_mutex_set(&data->state_mutex, &data->all_threads_ready, 1);
	if (ret == INT_MAX)
		pthread_join(data->monitor, NULL);
	i = -1;
	while (++i < data->philo_nbr)
		if (i < ret)
			pthread_join(data->philo[i].thread, NULL);
	return (exit_code);
}
