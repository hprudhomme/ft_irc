/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircbot.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 14:10:07 by ocartier          #+#    #+#             */
/*   Updated: 2022/11/29 14:54:47 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCBOT_HPP
# define IRCBOT_HPP

# include <string>
# include <iostream>
# include <vector>

std::vector<std::string>	split(std::string str, std::string delimiter);
std::string					intToString(int num);
int							randInt(int min, int max);
bool						containsOnlyDigits(const std::string &str);

#endif
