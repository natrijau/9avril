/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:35:59 by natrijau          #+#    #+#             */
/*   Updated: 2024/04/09 15:57:29 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


//test init philo
void	print_test_init(t_data *data)
{
	for (size_t i = 0; i < data->data_philo->number_of_philosophers; i++)
	{
		printf("idphilo -> %d\n", data->data_philo[i].id_philosphers);
	}
	
}