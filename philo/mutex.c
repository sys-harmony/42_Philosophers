/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:19:24 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/17 12:09:48 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_mutexes_destroy(int n, t_data *data)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		pthread_mutex_destroy(&data->philo[i].mutex);
		pthread_mutex_destroy(&data->fork[i].mutex);
	}
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->write_mutex);
}

void	ft_mutex_set(t_mutex *mutex, long *dest, long value)
{
	pthread_mutex_lock(mutex);
	*dest = value;
	pthread_mutex_unlock(mutex);
}

long	ft_mutex_get(t_mutex *mutex, long *value)
{
	long	ret;

	pthread_mutex_lock(mutex);
	ret = *value;
	pthread_mutex_unlock(mutex);
	return (ret);
}

long	ft_sim_is_over(t_data *data)
{
	const long	ret = ft_mutex_get(&data->state_mutex, &data->end_sim);

	return (ret);
}
