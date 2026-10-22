#include <iostream>
#include <lgpio.h>
#include <mosquitto.h>
#include <string.h>

int led_pin = 17;
int handle; // Variabile globale per il chip GPIO

// Questa funzione "Callback" scatta in automatico ogni volta che si clicca il pulsante sul web
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    if(msg->payloadlen) {
        // Convertiamo il payload in una stringa per comodità
        std::string payload((char*)msg->payload, msg->payloadlen);

        if (payload == "ON") {
            lgGpioWrite(handle, led_pin, 1);
            std::cout << "Comando ricevuto dal Web: LED ACCESO" << std::endl;
        } else if (payload == "OFF") {
            lgGpioWrite(handle, led_pin, 0);
            std::cout << "Comando ricevuto dal Web: LED SPENTO" << std::endl;
        }
    }
}

int main() {
    // 1. Configurazione GPIO (come prima)
    handle = lgGpiochipOpen(4);
    if (handle < 0) return 1;
    lgGpioClaimOutput(handle, 0, led_pin, 0);

    // 2. Configurazione MQTT
    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new("pi_led_subscriber", true, NULL);

    // Diciamo a Mosquitto quale funzione lanciare quando arriva un messaggio
    mosquitto_message_callback_set(mosq, on_message);

    // Ci connettiamo al broker locale
    mosquitto_connect(mosq, "localhost", 1883, 60);
    // Ci "abboniamo" al canale dei pulsanti web
    mosquitto_subscribe(mosq, NULL, "laboratorio/attuatori/led", 0);

    std::cout << "Sistema IoT avviato! Vai sulla Node-RED Dashboard per controllare il LED." << std::endl;

    // Un ciclo infinito che ascolta la rete senza affaticare la CPU
    mosquitto_loop_forever(mosq, -1, 1);

    // Pulizia finale (in caso di chiusura con CTRL+C)
    lgGpiochipClose(handle);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}
