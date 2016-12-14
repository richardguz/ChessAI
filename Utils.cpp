#include <curl/curl.h>
#include "Utils.h"

void printGameboard(std::array<std::array<char, 8>, 8> gameBoard) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (gameBoard[i][j] =='\0') {
				std::cout << '-' << " ";
			}
			else {
				std::cout << gameBoard[i][j] << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
  ((std::string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}

std::string makeHTTPRequest(const char* requestUrl, std::string type, const char* fields){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	std::string response;

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, requestUrl);
		if (type == "POST") {
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
		}

		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return response;
}