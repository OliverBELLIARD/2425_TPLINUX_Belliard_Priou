#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>      // Pour msleep
#include <linux/fs.h>         // Pour la gestion des fichiers dans le noyau
#include <linux/uaccess.h>    // Pour le contexte utilisateur/noyau

#define DRIVER_AUTHOR "Christophe Barès"
#define DRIVER_DESC "Chenillard Module pour LEDs"
#define DRIVER_LICENSE "GPL"

// LED constants
#define NUM_LEDS 9         // Nombre de LEDs
#define DELAY_MS 100       // Délai en millisecondes entre chaque étape

// Fonction pour écrire dans un fichier (brightness des LEDs)
static int write_to_file(const char *path, const char *value) {
    struct file *file;
    loff_t pos = 0; // Position dans le fichier
    mm_segment_t old_fs;
    int ret = 0;

    // Changer le contexte mémoire pour pouvoir écrire dans le noyau
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    // Ouvrir le fichier
    file = filp_open(path, O_WRONLY, 0);
    if (IS_ERR(file)) {
        printk(KERN_ERR "Impossible d'ouvrir le fichier : %s\n", path);
        ret = PTR_ERR(file);
        set_fs(old_fs);
        return ret;
    }

    // Écrire dans le fichier
    vfs_write(file, value, strlen(value), &pos);

    // Fermer le fichier
    filp_close(file, NULL);

    // Restaurer le contexte mémoire
    set_fs(old_fs);
    return ret;
}

// Fonction pour allumer ou éteindre une LED
static void set_led(int led, int state) {
    char path[50] = {0};
    char value[2] = {0};

    // Construire le chemin du fichier brightness
    snprintf(path, sizeof(path), "/sys/class/leds/fpga_led%d/brightness", led);

    // Préparer la valeur à écrire
    value[0] = state ? '1' : '0';
    value[1] = '\0';

    // Écrire dans le fichier
    if (write_to_file(path, value) < 0) {
        printk(KERN_ERR "Erreur lors de l'écriture pour la LED %d\n", led);
    }
}

// Fonction pour éteindre toutes les LEDs
static void clear_all_leds(void) {
    int i;
    for (i = 1; i <= NUM_LEDS; i++) {
        set_led(i, 0);
    }
}

// Fonction principale du chenillard
static int __init chenillard_init(void) {
    int i;

    printk(KERN_INFO "Chenillard démarré\n");

    while (1) {
        // Séquence avant
        for (i = 1; i <= NUM_LEDS; i++) {
            clear_all_leds();
            set_led(i, 1);
            msleep(DELAY_MS);
        }

        // Séquence arrière
        for (i = NUM_LEDS; i >= 1; i--) {
            clear_all_leds();
            set_led(i, 1);
            msleep(DELAY_MS);
        }
    }

    return 0;
}

// Fonction de nettoyage lors du déchargement du module
static void __exit chenillard_exit(void) {
    clear_all_leds();
    printk(KERN_INFO "Chenillard arrêté\n");
}

module_init(chenillard_init);
module_exit(chenillard_exit);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);


