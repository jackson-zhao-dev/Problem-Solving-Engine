#pragma once

#include "HttpClient.h"

class CurlHttpClient : public HttpClient
{
public:
    HttpResponse post(
        const std::string& url,
        const std::vector<std::string>& headers,
        const std::string& body
    ) override;
};