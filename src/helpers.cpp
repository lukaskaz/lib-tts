#include "helpers.hpp"

#include <curl/curl.h>
#include <curl/easy.h>

#include <fstream>

namespace helpers
{

static size_t WriteFunction(char* data, size_t size, size_t nmemb,
                            std::ofstream* ofs)
{
    size_t datasize{size * nmemb};
    ofs->write(data, datasize);
    return datasize;
}

bool Helpers::downloadFile(std::string& url, const std::string& text,
                           const std::string& filepath)
{
    CURLcode res{CURLE_FAILED_INIT};
    if (auto curl = curl_easy_init(); curl != nullptr)
    {
        std::ofstream ofs(filepath, std::ios::out | std::ofstream::binary);
        auto escapedtext =
            curl_easy_escape(curl, text.c_str(), (int)text.length());
        url.append(escapedtext);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
        res = curl_easy_perform(curl); // synchronous file download
        curl_free(escapedtext);
        curl_easy_cleanup(curl);
    }
    return res == CURLE_OK;
}

std::shared_ptr<HelpersIf> HelpersFactory::create()
{
    return std::make_shared<Helpers>();
}

} // namespace helpers
