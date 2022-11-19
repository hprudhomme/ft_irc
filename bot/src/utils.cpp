/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 14:09:53 by ocartier          #+#    #+#             */
/*   Updated: 2022/11/19 14:53:58 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
# include <string>
# include <iostream>
# include <vector>

std::vector<std::string> split(std::string str, std::string delimiter)
{
	std::vector<std::string> result;

	char *ptr = strtok(strdup(str.c_str()), delimiter.c_str());
	while (ptr != NULL)
	{
		result.push_back(ptr);
		ptr = strtok(NULL, delimiter.c_str());
	}

	return (result);
}
