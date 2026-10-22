#include <iostream>
#include <lgpio.h>
#include "httplib.h"

int main() {
    int led_pin = 17;
    
    // Apre il chip 4 (gestione GPIO su Raspberry Pi 5)
    int handle = lgGpiochipOpen(4);
    if (handle < 0) {
        std::cerr << "Errore: impossibile aprire il chip GPIO 4." << std::endl;
        return 1;
    }
    
    // Imposta il LED come output (livello iniziale 0 = spento)
    lgGpioClaimOutput(handle, 0, led_pin, 0);

    // Inizializza il server web
    httplib::Server svr;

    // 1. Pagina principale con interfaccia HTML (Formattazione classica C++)
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        std::string html = 
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head><title>Controllo LED</title></head>\n"
            "<body style=\"font-family: Arial; text-align: center; margin-top: 50px;\">\n"
            "    <h1>Controllo LED Raspberry Pi</h1>\n"
            "    <button onclick=\"fetch('/ON')\" style=\"padding: 20px; font-size: 20px; color: white; background: green; cursor: pointer; margin-right: 10px;\">ACCENDI</button>\n"
            "    <button onclick=\"fetch('/OFF')\" style=\"padding: 20px; font-size: 20px; color: white; background: red; cursor: pointer;\">SPEGNI</button>\n"
            "</body>\n"
            "</html>";
        res.set_content(html, "text/html");
    });

    // 2. Endpoint per accendere il LED
    svr.Get("/ON", [handle, led_pin](const httplib::Request &, httplib::Response &res) {
        lgGpioWrite(handle, led_pin, 1);
        std::cout << "Richiesta ricevuta: LED Acceso" << std::endl;
        res.set_content("OK", "text/plain");
    });

    // 3. Endpoint per spegnere il LED
    svr.Get("/OFF", [handle, led_pin](const httplib::Request &, httplib::Response &res) {
        lgGpioWrite(handle, led_pin, 0);
        std::cout << "Richiesta ricevuta: LED Spento" << std::endl;
        res.set_content("OK", "text/plain");
    });

    std::cout << "Server Web avviato! Apri il browser all'indirizzo http://rasp.local:8080" << std::endl;
    
    // Mette il server in ascolto sulla porta 8080 per tutta la rete locale
    svr.listen("0.0.0.0", 8080);

    // Chiusura sicura
    lgGpiochipClose(handle);
    return 0;
}
