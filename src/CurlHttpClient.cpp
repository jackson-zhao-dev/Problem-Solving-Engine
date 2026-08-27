#include "CurlHttpClient.h"

#include <curl/curl.h>

#include <stdexcept>
#include <string>

namespace
{
    std::size_t writeCallback(
        char* data,
        std::size_t size,
        std::size_t count,
        void* userData
    )
    {
        const std::size_t totalSize =
            size * count;

        std::string* responseBody =
            static_cast<std::string*>(
                userData
            );

        responseBody->append(
            data,
            totalSize
        );

        return totalSize;
    }
}

HttpResponse CurlHttpClient::post(
    const std::string& url,
    const std::vector<std::string>& headers,
    const std::string& body
)
{
    CURL* curl =
        curl_easy_init();

    if (curl == nullptr)
    {
        throw std::runtime_error(
            "Failed to initialize CURL."
        );
    }

    curl_slist* headerList = nullptr;

    for (const std::string& header : headers)
    {
        headerList =
            curl_slist_append(
                headerList,
                header.c_str()
            );
    }

    HttpResponse response;

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POST,
        1L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headerList
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        body.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDSIZE,
        static_cast<long>(
            body.size()
        )
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        writeCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response.body
    );

    const CURLcode result =
        curl_easy_perform(
            curl
        );

    if (result != CURLE_OK)
    {
        const std::string errorMessage =
            curl_easy_strerror(
                result
            );

        curl_slist_free_all(
            headerList
        );

        curl_easy_cleanup(
            curl
        );

        throw std::runtime_error(
            "HTTP request failed: " +
            errorMessage
        );
    }

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &response.statusCode
    );

    curl_slist_free_all(
        headerList
    );

    curl_easy_cleanup(
        curl
    );

    return response;
}