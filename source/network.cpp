#include "network.h"
#include <curl/curl.h>
#include <string>

// Hilfsfunktion: Speichert die Antwort des Servers in einem String
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void Network::init() {
    // Initialisiert die globale Curl-Umgebung
    curl_global_init(CURL_GLOBAL_ALL);
}

void Network::exit() {
    // Beendet die Curl-Umgebung sauber
    curl_global_cleanup();
}

bool Network::isConnected() {
    CURL* curl = curl_easy_init();
    if(curl) {
        // Wir fragen nur den Header der 3DS-Route ab, um Daten zu sparen
        curl_easy_setopt(curl, CURLOPT_URL, "https://adaptable-integrity-production-78c1.up.railway.app/3ds");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); 
        
        // SSL-Zertifikatsprüfung auf 0, da der 3DS oft veraltete Root-Zertifikate hat
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        
        // Timeout auf 2 Sekunden setzen, damit die App nicht hängen bleibt
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2L);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        // Wenn der Request erfolgreich war (CURLE_OK), ist der Server online
        return (res == CURLE_OK);
    }
    return false;
}

void Network::sendMessage(std::string user, std::string text) {
    CURL* curl = curl_easy_init();
    if(curl) {
        std::string url = "https://adaptable-integrity-production-78c1.up.railway.app/send";
        
        // JSON Body erstellen
        std::string jsonPayload = "{\"username\":\"" + user + "\",\"text\":\"" + text + "\"}";

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

std::string Network::fetchData() {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if(curl) {
        std::string url = "https://adaptable-integrity-production-78c1.up.railway.app/3ds";
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}