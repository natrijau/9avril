/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:05:09 by natrijau          #+#    #+#             */
/*   Updated: 2024/04/09 16:41:08 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	check_arg(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 5 && ac != 6)
	{
		printf("Invalid number of arguments !\n");
		return (false);
	}
	while (av[i])
	{
		if (!its_integer(av[i]))
		{
			printf("One of the arguments is invalid !\n");
			return (false);			
		}
		i++;
	}	
	if (!all_positiv_num(av))
	{
		printf("One of the arguments is invalid !\n");
		return(false);
	}
	return(true);
}	

void	check_dead(t_philosophers *philo, int flag)
{
	if (flag == 0)
	{
		philo->start_dead = 0;
		if (philo->time_to_eat >= philo->time_to_die)
		{
			// printf("\teating\tLe philosophe %d alive ? %d\n",philo->id_philosphers, philo->alive);
			// philo->alive = false;
			philo->end_time = philo->time_to_die / 1000;
		}
	}
	if (flag == 1)
	{
		philo->start_dead += philo->time_to_sleep + philo->time_to_eat;
		if (philo->start_dead >= philo->time_to_die
			|| philo->time_to_sleep >= philo->time_to_die)
		{
			// philo->alive = false;
			// printf("\tsleeping\tLe philosophe %d alive ? %d\n",philo->id_philosphers, philo->alive);
			philo->end_time = philo->time_to_die / 1000;
		}
	}
}

void	ft_usleep(t_philosophers *philo, long int mili_second)
{
	(void) philo;
	(void) mili_second;
	struct timeval my_time;
  	long int current_time;
  	long int i;

	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	printf("current time %lu\n", current_time);
	i = 0;
	// printf("i %ld\n", i);
	
	while (i < mili_second / 1000)
	{
		usleep(170);
		gettimeofday(&my_time, NULL);
		i = ((my_time.tv_sec * 1000) + (my_time.tv_usec / 1000)) - philo->start_time;
		// printf("i %ld\n", i);
	}
}

void	ft_eating(t_philosophers *philo)
{
	pthread_mutex_lock(&philo->my_fork);
	pthread_mutex_lock(philo->next_fork);

	pthread_mutex_lock(&philo->print);
	if (philo->data->dead_id > 0){
		
		pthread_mutex_unlock(&philo->my_fork);
		pthread_mutex_unlock(philo->next_fork);
		
		pthread_mutex_unlock(&philo->print);
		return;
	}
	pthread_mutex_unlock(&philo->print);
	
	struct timeval my_time;
  	long int current_time;
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	if (philo->time_to_die <= philo->time_to_eat)
	{
		printf("%ld\tLe philosophe %d take fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
		printf("%ld\tLe philosophe %d take second fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
		printf("%ld\tLe philosophe %d start eat \n",current_time - philo->start_time,  philo->id_philosphers);
		usleep(philo->time_to_die);
		philo->end_time += philo->time_to_die / 1000;
		pthread_mutex_lock(&philo->print);
		philo->data->dead_id += 1;
		if (philo->data->dead_id == 1)
			printf("%ld Le philosophe %d est MORTTTTTTTTTTTT !\n", philo->end_time, philo->id_philosphers);
		pthread_mutex_unlock(&philo->print);
		
		pthread_mutex_unlock(&philo->my_fork);
		pthread_mutex_unlock(philo->next_fork);
		return ;
	}	
	pthread_mutex_lock(&philo->print);
	if (philo->data->dead_id == 0)
	{
		printf("%ld\tLe philosophe %d take fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
		printf("%ld\tLe philosophe %d take second fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
		printf("%ld\tLe philosophe %d start eat \n",current_time - philo->start_time,  philo->id_philosphers);
		usleep(philo->time_to_eat);
	}
	philo->number_of_times_each_philosopher_must_eat--;
	philo->start_dead = philo->time_to_eat;
	pthread_mutex_unlock(&philo->print);
	pthread_mutex_unlock(&philo->my_fork);
	pthread_mutex_unlock(philo->next_fork);
}

void	sleeping(t_philosophers *philo)
{
	struct timeval my_time;
  	long int current_time;
	
	pthread_mutex_lock(&philo->print);
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	// check_dead(philo, 1);
	
	if (philo->start_dead + philo->time_to_sleep >= philo->time_to_die)
	{
		// printf("fonction sleeping\n\tphilo->start_dead + philo->time_to_sleep %ld =? %d philo->time_to_die\nphilo->data->dead_id %d\n", philo->start_dead + philo->time_to_sleep, philo->time_to_die, philo->data->dead_id);
		philo->data->dead_id += 1;
		// philo->end_time += philo->time_to_die / 1000;
		// pthread_mutex_lock(&philo->print);
		// printf("%ld\tLe philosophe %d start sleep !\n",current_time - philo->start_time, philo->id_philosphers);
		// pthread_mutex_unlock(&philo->print);
		// pthread_mutex_lock(&philo->print);
		// if (philo->data->dead_id == 1)
			// printf("%ld Le philosophe %d est MORTTTTTTTTTTTT !\n", philo->end_time, philo->id_philosphers);
		// pthread_mutex_unlock(&philo->print);
		return ;
	}
	if (philo->data->dead_id == 0)
	{
		printf("%ld\tLe philosophe %d start sleep !\n",current_time - philo->start_time, philo->id_philosphers);
	    usleep(philo->time_to_sleep);
	}
	philo->start_dead = current_time - philo->start_time;
	// ft_usleep(philo, philo->time_to_sleep);
	pthread_mutex_unlock(&philo->print);
}

void	thinking(t_philosophers *philo)
{
	// if (philo->data->alive == false)
	// {
	// 	return;
	// }
	
	struct timeval my_time;
  	long int current_time;
	pthread_mutex_lock(&philo->print);
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	// philo->end_time = current_time - philo->start_time;
	if (philo->start_dead >= philo->time_to_die / 1000)
	{
		philo->end_time = philo->time_to_die / 1000;
		philo->data->dead_id += 1;
	}
	if (philo->data->dead_id == 0)
		printf("%ld\tLe philosophe %d think !\n",(current_time - philo->start_time),  philo->id_philosphers);
	pthread_mutex_unlock(&philo->print);
}

void	*round_table(void *arg)
{
	t_philosophers	*philo = (t_philosophers *) arg;
	struct timeval start;
	gettimeofday(&start, NULL);
	philo->start_time = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	if (philo->id_philosphers % 2 == 1)
	{
		thinking(philo);
		usleep(50);
	}	
	while (1)
	{
		ft_eating(philo);					
		sleeping(philo);
		thinking(philo);
		// if(philo->number_of_times_each_philosopher_must_eat == 0)
		// 	break;
		
		pthread_mutex_lock(&philo->print);
		printf("philo->data->dead_id %d\n", philo->data->dead_id);
		if (philo->data->dead_id > 0)
		{
			pthread_mutex_unlock(&philo->print);
			break;
		}
		pthread_mutex_unlock(&philo->print);
	}
	
	pthread_exit(EXIT_SUCCESS);	
}

int	get_thread(t_data *data)
{
	unsigned int	num_fork;
	
	num_fork = data->data_philo->number_of_philosophers;
	while (num_fork > 0)
	{
		pthread_create(&data->data_philo[num_fork -1].thread_philo, NULL, round_table, &data->data_philo[num_fork - 1]);
		num_fork--;
	}
	while (num_fork < data->data_philo->number_of_philosophers)
	{
		pthread_join(data->data_philo[num_fork].thread_philo, NULL);
		num_fork++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data data;
	if (!check_arg(ac, av))
		return (1);
	init_philo(&data, av);
	// print_test_init(&data);
	get_thread(&data);
	// free_all(&data);
	return (0);
}