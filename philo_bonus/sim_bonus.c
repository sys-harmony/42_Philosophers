/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:08:57 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/15 20:54:33 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	ft_eat(t_philo *philo)
{
	long	last_meal_time;

	sem_wait(philo->data->forks_sem);
	ft_write_state(TAKING_FIRST_FORK, philo);
	sem_wait(philo->data->forks_sem);
	last_meal_time = ft_get_time(MILLISECOND);
	if (last_meal_time < 0)
		ft_abort(philo->data);
	ft_sem_set(philo->lock_sem, &philo->last_meal_time, last_meal_time);
	ft_write_state(TAKING_SECOND_FORK_AND_EATING, philo);
	philo->meal_ct++;
	ft_usleep(philo->data->time_to_eat, philo->data);
	sem_post(philo->data->forks_sem);
	sem_post(philo->data->forks_sem);
}

static void	ft_dinner(t_philo *philo)
{
	t_data	*d;

	d = philo->data;
	ft_sem_set(philo->lock_sem, &philo->last_meal_time, d->start_time);
	if (pthread_create(&philo->thread, NULL, ft_monitor, philo))
		ft_abort(philo->data);
	pthread_detach(philo->thread);
	if (philo->id % 2)
		usleep(500);
	while (1)
	{
		ft_eat(philo);
		if (d->max_meals > 0 && philo->meal_ct >= d->max_meals)
			break ;
		ft_write_state(SLEEPING, philo);
		ft_usleep(d->time_to_sleep, d);
		ft_write_state(THINKING, philo);
		if (d->think_time > 0)
			ft_usleep(d->think_time, d);
	}
	exit(EXIT_SUCCESS);
}

static int	ft_start_processes(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_nbr)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
		{
			ft_error("philo_bonus: fork failed\n", 1);
			while (i--)
				kill(data->pid[i], SIGKILL);
			return (1);
		}
		else if (data->pid[i] == 0)
			ft_dinner(&data->philo[i]);
	}
	return (0);
}

static void	*ft_wait_all_meals(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	i = -1;
	while (++i < data->philo_nbr)
		waitpid(-1, NULL, 0);
	sem_post(data->stop_sem);
	return (NULL);
}

int	ft_sim(t_data *data)
{
	pthread_t	waiter_thread;
	int			i;

	data->start_time = ft_get_time(MILLISECOND);
	if (data->start_time < 0)
		return (1);
	if (ft_start_processes(data))
		return (1);
	if (pthread_create(&waiter_thread, NULL, ft_wait_all_meals, data))
		return (1);
	sem_wait(data->stop_sem);
	i = -1;
	while (++i < data->philo_nbr)
		kill(data->pid[i], SIGKILL);
	pthread_join(waiter_thread, NULL);
	return (0);
}
