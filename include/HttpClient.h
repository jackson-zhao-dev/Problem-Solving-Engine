#pragma once

#include <string>
#include <vector>

struct HttpResponse
{
    long statusCode = 0;
    std::string body;
};

class HttpClient
{
public:
    virtual ~HttpClient() = default;

    virtual HttpResponse post(
        const std::string& url,
        const std::vector<std::string>& headers,
        const std::string& body
    ) = 0;
};