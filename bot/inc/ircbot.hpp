/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircbot.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 14:10:07 by ocartier          #+#    #+#             */
/*   Updated: 2022/12/08 14:19:01 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCBOT_HPP
# define IRCBOT_HPP

# include <string>
# include <iostream>
# include <vector>

std::vector<std::string>	split(std::string str, char delimiter);
std::string					intToString(int num);
int							randInt(int min, int max);
bool						containsOnlyDigits(const std::string &str);

#endif
