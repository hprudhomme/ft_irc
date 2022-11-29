/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 14:09:53 by ocartier          #+#    #+#             */
/*   Updated: 2022/11/29 14:54:27 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>

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

std::string intToString(int num)
{
	std::ostringstream ss;
	ss << num;
	return (ss.str());
}

int randInt(int min, int max)
{
	return (rand() % (max - min + 1) + min);
}

bool containsOnlyDigits(const std::string &str)
{
	return str.find_first_not_of("0123456789") == std::string::npos;
}
