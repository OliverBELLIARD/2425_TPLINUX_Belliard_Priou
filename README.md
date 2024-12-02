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

### 1.2 Démarrage
