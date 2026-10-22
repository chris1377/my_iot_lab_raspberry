#include <iostream>
#include <lgpio.h>
#include <unistd.h>

int main() {
    int button_pin = 27;

    // Apre il chip 4 (quello dei GPIO esterni sul Pi 5)
    int handle = lgGpiochipOpen(4);
    if (handle < 0) {
        std::cerr << "Errore: impossibile aprire il chip GPIO 4." << std::endl;
        return 1;
    }

    // Imposta il pin del pulsante come input
    lgGpioClaimInput(handle, 0, button_pin);

    std::cout << "Lettura del GPIO " << button_pin << " in corso..." << std::endl;
    std::cout << "Premi il pulsante per vedere il cambio di stato. CTRL+C per uscire." << std::endl;

    while (true) {
        // Legge lo stato attuale del pin
        int state = lgGpioRead(handle, button_pin);
        
        // Stampa lo stato a schermo
        std::cout << "Stato pin " << button_pin << ": " << state << std::endl;
        
        // Pausa di 250 millisecondi (4 letture al secondo)
        usleep(250000); 
    }

    lgGpiochipClose(handle);
    return 0;
}
