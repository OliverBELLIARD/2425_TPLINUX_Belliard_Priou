#include <stdio.h>
#include <unistd.h> // For usleep

#define NUM_LEDS 9         // Number of LEDs
#define DELAY_US 100000    // Delay in microseconds (100ms)

// Function to set the brightness of an LED
void set_led(int led, int state) {
    char path[50];
    FILE *led_file;

    // Construct the LED file path
    snprintf(path, sizeof(path), "/sys/class/leds/fpga_led%d/brightness", led);

    // Open the file for writing
    led_file = fopen(path, "w");
    if (led_file == NULL) {
        perror("Error opening LED file");
        return;
    }

    // Write the state to the file
    fprintf(led_file, "%d", state);
    fclose(led_file);
}

// Function to turn off all LEDs
void clear_all_leds() {
    for (int i = 1; i <= NUM_LEDS; i++) {
        set_led(i, 0);
    }
}

int main() {
    while (1) {
        // Forward sequence
        for (int i = 1; i <= NUM_LEDS; i++) {
            clear_all_leds();
            set_led(i, 1);
            usleep(DELAY_US);
        }

        // Reverse sequence
        for (int i = NUM_LEDS; i >= 1; i--) {
            clear_all_leds();
            set_led(i, 1);
            usleep(DELAY_US);
        }
    }

    return 0;
}
