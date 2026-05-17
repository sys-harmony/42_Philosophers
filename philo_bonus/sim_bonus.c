/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdosch <gdosch@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 11:08:57 by gdosch            #+#    #+#             */
/*   Updated: 2026/05/17 14:41:23 by gdosch           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	ft_eat(t_philo *philo)
{
	t_data	*d;
	long	last_meal_time;

	d = philo->data;
	sem_wait(d->diners_sem);
	sem_wait(d->forks_sem);
	ft_write_state(TAKING_FIRST_FORK, philo);
	sem_wait(d->forks_sem);
	last_meal_time = ft_get_time(MS);
	if (last_meal_time < 0)
		ft_abort(philo->data);
	ft_sem_set(philo->lock_sem, &philo->last_meal_time, last_meal_time);
	ft_write_state(TAKING_SECOND_FORK_AND_EATING, philo);
	if (++philo->meal_ct == d->max_meals)
		sem_post(d->done_sem);
	ft_usleep(d->time_to_eat, d);
	sem_post(d->forks_sem);
	sem_post(d->forks_sem);
	sem_post(d->diners_sem);
}

static void	ft_dinner(t_philo *philo)
{
	t_data	*d;

	d = philo->data;
	ft_sem_set(philo->lock_sem, &philo->last_meal_time, d->start_time);
	if (pthread_create(&philo->thread, NULL, ft_monitor, philo))
		ft_abort(d);
	pthread_detach(philo->thread);
	if (philo->id % 2)
		usleep(500);
	while (1)
	{
		ft_eat(philo);
		ft_write_state(SLEEPING, philo);
		ft_usleep(d->time_to_sleep, d);
		ft_write_state(THINKING, philo);
		if (d->think_time > 0)
			ft_usleep(d->think_time, d);
	}
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
			{
				kill(data->pid[i], SIGKILL);
				waitpid(data->pid[i], NULL, 0);
			}
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
		sem_wait(data->done_sem);
	sem_wait(data->write_sem);
	sem_post(data->stop_sem);
	return (NULL);
}

int	ft_sim(t_data *data)
{
	pthread_t	waiter_thread;
	int			ret;
	int			i;

	data->start_time = ft_get_time(MS);
	if (data->start_time < 0 || ft_start_processes(data))
		return (1);
	ret = pthread_create(&waiter_thread, NULL, ft_wait_all_meals, data);
	if (ret)
	{
		ft_error("philo_bonus: pthread_create failed\n", 1);
		sem_post(data->stop_sem);
	}
	sem_wait(data->stop_sem);
	i = 0;
	while (i < data->philo_nbr)
		kill(data->pid[i++], SIGKILL);
	sem_post(data->write_sem);
	while (i--)
		sem_post(data->done_sem);
	if (!ret)
		pthread_join(waiter_thread, NULL);
	while (++i < data->philo_nbr)
		waitpid(-1, NULL, 0);
	return (ret);
}
