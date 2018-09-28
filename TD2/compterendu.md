# 1 Manipulation des procéssus UNIX
# 2 Création des procéssus UNIX

 - Si on augmente le scheduleur à le temps de passer d'un processus à un autre ce qui n'est pas le cas quand le compteur est à 10.
 - Si on lance le programme plusieur fois il y a des chances que le resultat sera différent
 - C'est la gentillesse le plus juste serait de les executer dans l'ordre pendant une période égale de temps et la plus efficaces
    serait des lancer les plus "legé" au debut en premier puis les plus lourd à la fin


- Le principe est le même, si la boucle est trop courte et donc trop rapide à s'executer, ont va avoir l'impression que les processus sont executé les uns après les autres.
    Si la boucle est plus longue à etre executé on se rend compte que les processus ne sont pas executé les uns après les autres, mais selon les "directives" du scheduler

# 3 La Terminaison des processus
## 3.1 Processus orphelin
- Il y a création de processus car le père n'attend pas le fils (la fonction wait permet de le faire) C'est pour ça que le père peu finir
    avant le fils laissant sont fils orphelin.

## 3.2 Processus zombie
- 


