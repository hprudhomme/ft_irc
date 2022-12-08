/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 14:09:53 by ocartier          #+#    #+#             */
/*   Updated: 2022/12/08 14:18:48 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>

std::vector<std::string> split(std::string str, char delimiter)
{
	std::stringstream ss(str);
	std::vector<std::string> tokens;

	std::string token;
	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);
	return tokens;
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
