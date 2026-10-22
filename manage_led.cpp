#include <iostream>
#include <lgpio.h>
#include <unistd.h>

int main() {
    int led_pin = 17;
    int button_pin = 27;

    // Sul Raspberry Pi 5, il chip RP1 che gestisce i GPIO esterni è il numero 4
    int handle = lgGpiochipOpen(4);
    if (handle < 0) {
        std::cerr << "Errore: impossibile aprire il chip GPIO 4." << std::endl;
        return 1;
    }

    // Imposta LED come output (0 = flag di default, 0 = livello iniziale spento)
    lgGpioClaimOutput(handle, 0, led_pin, 0);
    
    // Imposta Pulsante come input
    lgGpioClaimInput(handle, 0, button_pin);

    std::cout << "Premi il pulsante per accendere il LED. CTRL+C per uscire." << std::endl;

    while (true) {
        // Legge lo stato del pulsante (1 = premuto a 3.3V, 0 = rilasciato a GND)
        int button_state = lgGpioRead(handle, button_pin);
        
        // Accende o spegne il LED in base al pulsante
        if (button_state == 1) {
            lgGpioWrite(handle, led_pin, 1);
        } else {
            lgGpioWrite(handle, led_pin, 0);
        }
        
        usleep(50000); // Pausa di 50 millisecondi per alleggerire la CPU
    }

    lgGpiochipClose(handle);
    return 0;
}
