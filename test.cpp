#include<bits/stdc++.h>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
using json = nlohmann::json;
using namespace std;

string access_token="";
string expires_in="";
string refresh_token="";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void authorize() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://test.deribit.com/api/v2/public/auth?client_id=0gFWhS6k&client_secret=KmBCqN-VnIISld8z4ghIS-Ma8lYnHlfkZAbmJiwxT8Q&grant_type=client_credentials");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            json jsonResponse = json::parse(readBuffer);
            std::unordered_map<std::string, std::string> responseMap;
            for (auto& el : jsonResponse["result"].items()) {
                responseMap[el.key()] = el.value().dump(); // Store JSON values as strings
            }
            access_token=responseMap["access_token"];
            access_token=access_token.substr(1,access_token.length()-2);
            expires_in=responseMap["expires_in"];
            refresh_token=responseMap["refresh_token"];
        }
        curl_easy_cleanup(curl);
    }
}

void placeOrder() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://test.deribit.com/api/v2/private/buy?amount=4000&instrument_name=ETH-PERPETUAL&label=market0000234&type=market");
        
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        struct curl_slist* currentHeader = headers;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET"); 

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            cout<<"Your Order Placed Successfully"<<endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); 
    }
}
void cancelOrder(const std::string& orderId) {

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://test.deribit.com/api/v2/private/cancel?order_id=" + orderId;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        struct curl_slist* headers = nullptr;
        std::string authHeader = "Authorization: Bearer " + access_token;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        struct curl_slist* currentHeader = headers;

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Cancel Order Response: " << readBuffer << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); // Free headers
    }
}
void modifyOrder(const std::string& orderId, int amount, double price) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://test.deribit.com/api/v2/private/edit?&amount=" + std::to_string(amount) + "&order_id=" + orderId + "&price=" + std::to_string(price);        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        struct curl_slist* headers = nullptr;
        std::string authHeader = "Authorization: Bearer " + access_token;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        struct curl_slist* currentHeader = headers;

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET"); 

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Modify Order Response: " << readBuffer << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); // Free headers
    }
}
void getOrderBook(const std::string& instrumentName, int depth) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://test.deribit.com/api/v2/public/get_order_book?depth=" + std::to_string(depth) + "&instrument_name=" + instrumentName;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        struct curl_slist* currentHeader = headers;

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET"); 

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Order Book Response: " << readBuffer << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); // Free headers
    }
}
void getPosition(const std::string& currency, const std::string& kind) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://test.deribit.com/api/v2/private/get_positions?currency=" + currency + "&kind=" + kind;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        struct curl_slist* headers = nullptr;
        std::string authHeader = "Authorization: Bearer " + access_token;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Get Position Response: " << readBuffer << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers); // Free headers
    }
}








int main() {
    authorize();
    cout<<"access_token: "<<access_token<<endl<<endl;
    if(access_token!=""){
        //place market order
        // placeOrder();
    }
    // cancelOrder("ETH-14481533170");
    // modifyOrder("ETH-14481528434",1,10);
    // getOrderBook("ETH-PERPETUAL",5);
    getPosition("ETH","future");

    return 0;
}