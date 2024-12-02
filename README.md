> 2425_TPLINUX_Belliard_Priou
> TP Linux sous forme de projet en 3 séances.

# TP Linux Embarqué
L'ensemble des fichiers nécessaires à ce TP se trouvent dans l'[archive à télécharger](https://filesender.renater.fr/?s=download&token=de7fee37-7ec4-4e67-a19a-57ef8ab2733f).

## 1 Prise en main
### 1.1 Préparation de la carte SD

Avant de commencer, il faut flasher la carte SD avec l’image fournie par Tera-sic. Il s'agit du fichier `VEEK_MT2S_LXDE/VEEK_MT2S_LXDE.img`.  
Nous allons flasher la carte SD depuis **Linux, Ubuntu** :  
1. D’abord, il faut identifier le device correspondant à la carte SD. Ce device est dans le dossier `/dev/`.
   On peut donc regarder le contenu de ce dossier avant d’insérer la carte SD, puis après, et comparer avec la commande :
   ```
   ls /dev
   ```
   > Plusieurs fichiers devraient être apparus, quelque chose du type `/dev/sdX`, `/dev/sdX1`, `/dev/sdX2`...
      Dans notre cas, notre carte SD semble être gérée via l'interface MMC (Multimedia card). Elle apparaît donc sous "mmcblk0". Elle a aussi 3 partitions, nommées respectivement mmcblk0p1, p2 et p3.  

      En voici un aperçu :
   
![image](https://github.com/user-attachments/assets/988564cb-1820-4ba1-b3ba-c18dc2819758)

   > Il faut noter bien la lettre (sda, sdb, sdc ou autre). Si on se trompe, on risque d’écraser le disque dur du PC. Et ce serait **très mal**.
   > En cas de doute, faire appel au prof.
3. Pour flasher la carte SD, tappez simplement les lignes suivantes dans un terminal :
   ```
   cd <chemin_vers_img>
   sudo dd if=VEEK_MT2S_LXDE.img of=/dev/sdX bs=4k status=progresssync
   ```
Nous avons donc bien monté l'image sur notre carte SD en prenant soin de sélectionner "mmcblk0" :  

![image](https://github.com/user-attachments/assets/0e4dc725-4b2a-4c4b-baa8-93d474a77197)


### 1.2 Démarrage
on insère la carte SD fraichement programmée, nous avons branché la carte VEEK-MT2S et l'avons allumée (bouton rouge).
Ça clignote de partout et un linux se lance.
> Nous prenons alors un peu de temps pour explorer les différents programmes fournis sur le bureau de Lxde.
  
Mais sans clavier, ce n’est pas très pratique pour programmer cette « brique ».  
Nous allons y remédier.  

### 1.3 Connexion au système
#### 1.3.1 Liaison série
Le premier moyen pour se connecter sur un objet embarqué, c’est très souvent par le port série. Une fois que l’on aura eu accès au DE-10, on configurera le réseau, pour pouvoir ensuite y accéder via ssh.  
Tout d’abord, déterminer le port à utiliser pour se connecter à la carte. Il y a plusieurs ports USB sur la carte :
- 2 hôtes usb A
- 1 usb B : max blaster pour la programmation
- 1 usb mini : uart to usb **← c’est celui-là qui nous intéresse.**

#### 1.3.2 Utilisez un logiciel de liaison série
**Sous Linux :** Utilisez minicom pour vous connecter grace au port série.
```
minicom -D /dev/ttyUSBx -b 115200
```
Avec x le numéro du port.

Vous devriez voir des choses s’afficher après un appui sur <Entrée>.
Si rien ne s’affiche, essayez la combinaison suivante :  
`Ctrl+A` puis `O > Configurer le port série > F`.  
  
**Une fois connecté au SoC :**  
Pour vous identifier :
- login : root
- password : aucun (vraiment rien, ne tapez que sur entrée)
  
Pour l’exercice, nous allons redémarrer le SoC pour observer la séquence de démarrage. On commence par la commande :
```
reboot
```
Loggez-vous à nouveau. L’image copiée précédemment n’occupe pas la totalité de la carte SD.  
*Quelle est la taille occupée ?*  
Utilisez la commande suivante :
```
df -h
```
Des scripts sont fournis pour augmenter la taille du système de fichiers et
profiter de l’ensemble de la carte SD :
```
./expand_rootfs.sh
```
Rebootez la carte (proprement !), puis, une fois loggé :
```
./resize2fs_once
```
Vérifiez que vous avez bien 32GB de disponible sur la carte SD.

#### 1.3.3 Configuration réseau
- Branchez la carte VEEK sur le switch via un câble réseau,
- À l’aide de la commande ifconfig, vérifiez que la carte dispose d’une adresse IP,
- Éditer le fichier /etc/network/interfaces de la manière suivante :
  ```
  # interfaces(5) file used by ifup(8) and ifdown(8)
  # Include files from /etc/network/interfaces.d:
  source-directory /etc/network/interfaces.d
  auto eth0
  iface eth0 inet dhcp
  allow-hotplug eth0
  ```

Pour ces modification, vous allez devoir utiliser vim de la façon suivante :
- Éditez le fichier avec la commande `vim /etc/network/interfaces`,
- Effacez toutes les lignes inutiles en appuyant deux fois sur la touche `d`
- Insérez une nouvelle ligne en appuyant sur `o`. Cette touche vous bascule en mode `-- INSERT --`
- À partir de là vous pouvez taper le contenu du fichier,
- Quand c’est terminé, tapez sur la touche `Echap` pour revenir en mode command
- Tapez `:wq` puis sur entrée pour sauvegarder et quitter vim.  
  
Rebootez une nouvelle fois, et là normalement vous allez avoir une adresse IP correcte. Vérifiez avec `ifconfig`.

- Si tout est correct, vous devez pouvoir pinger avec le PC dans les 2 sens avec :
  ```
  ping <adresse_ip>
  ```
- Vérifiez que dans le fichier /etc/ssh/sshd_config, la ligne suivante est présente :
  ```
  PermitEmptyPasswords yes
  ```
- Vous devriez pouvoir vous logger en ssh sur la carte VEEK, avec Putty ou par le terminal :
  ```
  ssh root@<IP_VEEK>
  ```
  Une fois connecté en ssh, vous pouvez fermer la liaison série. Sur minicom ça se fait avec la combinaison suivante : `Ctrl+A` puis `Q`
  
### 1.4 Découverte de la cible
#### 1.4.1 Exploration des dossiers /sys/class et /proc

Explorez un peu votre environnement, par exemple :
— Répertoires présent sous la racine
— Dans /proc : cpuinfo, ioports, iomem. Utilisez les commandes cat, less
ou more pour voir le contenu des fichiers.
— Le répertoire /sys/class contient des entrées nouvelles (par raport à un
PC classique), saurez vous les reconnaître ? En particulier, explorez les ré-
pertoires suivants :
— /sys/class/leds/fpga_ledX/
— /proc/ioport
— /proc/iomem
— /proc/device-tree/sopc@0 à comparer avec le fichier iomem.