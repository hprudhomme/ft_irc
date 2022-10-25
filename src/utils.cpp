/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:00:59 by ocartier          #+#    #+#             */
/*   Updated: 2022/10/21 17:36:31 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string>

std::string ft_inet_ntop6(const void *a0)
{
	const unsigned char *a = (const unsigned char *)a0;
	int i, j, max, best;
	char buf[INET6_ADDRSTRLEN];

	if (memcmp(a, "\0\0\0\0\0\0\0\0\0\0\377\377", 12))
		snprintf(buf, sizeof buf,
			"%x:%x:%x:%x:%x:%x:%x:%x",
			256*a[0]+a[1],256*a[2]+a[3],
			256*a[4]+a[5],256*a[6]+a[7],
			256*a[8]+a[9],256*a[10]+a[11],
			256*a[12]+a[13],256*a[14]+a[15]);
	else
		snprintf(buf, sizeof buf,
			"%x:%x:%x:%x:%x:%x:%d.%d.%d.%d",
			256*a[0]+a[1],256*a[2]+a[3],
			256*a[4]+a[5],256*a[6]+a[7],
			256*a[8]+a[9],256*a[10]+a[11],
			a[12],a[13],a[14],a[15]);
	/* Replace longest /(^0|:)[:0]{2,}/ with "::" */
	for (i=best=0, max=2; buf[i]; i++) {
		if (i && buf[i] != ':') continue;
		j = strspn(buf+i, ":0");
		if (j>max) best=i, max=j;
	}
	if (max>3) {
		buf[best] = buf[best+1] = ':';
		memmove(buf+best+2, buf+best+max, i-best-max+1);
	}
	return (buf);
}
