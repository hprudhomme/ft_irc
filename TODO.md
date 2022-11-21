- [ ] Récupérer le port d'écoute depuis les arguments commande
- [ ] Des erreurs quand un client leave
- [ ] Segfault si on envoie un message dans un channel non-rejoins
- [ ] Pas de messages (ou pas le bon format) pour les PART (donc segfault ensuite), donc les clients ne savent pas qui à quitté, et ça crée un segfault pour la même raison

- [ ] Voir si c'est normal de pouvoir join un chan du nom d'un utilisateur (sans # devant)

- [ ] Tout n'est pas en cpp98

- [ ] commande password

# Tests commandes approfondi:
	- [x] INVITE
	- [ ] JOIN
	- [ ] KICK
	- [ ] LIST
	- [x] MODE
	- [ ] NICK
	- [ ] NOTICE
	- [ ] PART
	- [ ] PASS
	- [ ] PING
	- [ ] PONG
	- [ ] PRIVMSG
	- [ ] QUIT
	- [ ] USER
	- [ ] WHO
