#ifndef NETWORK_H
#define NETWORK_H

#include <string>

class Network {
public:
    static void init();                   // HTTP Service starten
    static void sendMessage(std::string user, std::string text);  // Nachricht senden
    static std::string fetchData();       // Daten vom Server holen
    static void exit();                   // HTTP Service beenden
};

#endif
