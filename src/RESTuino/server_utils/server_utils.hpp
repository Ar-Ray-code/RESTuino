#pragma once

#include <Arduino.h>

#define MAX_LENGETH 1024

#define MODE_GET 0
#define MODE_POST 1
#define MODE_PUT 2
#define MODE_DELETE 3
#define MODE_UNKNOWN 4

static const char *host_name = "restuino"; // RESTuino
static const uint8_t n = 40;
static uint16_t gpio_arr[n] = {}; //すべて0で初期化
// Pubished values for SG90 servos; adjust if needed
static const uint32_t minUs = 0;
static const uint32_t maxUs = 5000;
// 180> angle > angle0 >= 0にすること
static const uint8_t angle0 = 5;
static const uint8_t angle = 60;


class HTTPKeyValue
{
public:
    HTTPKeyValue() {}
    String url_dir;
    uint16_t gpio_num;

    String host;
    String user_agent;
    String content_type;
    String content_length;
    uint16_t content_length_min = MAX_LENGETH;

    String connection;
    String accept;
    String accept_encoding;
    String accept_language;
    String accept_charset;
    String pragma;
    String upgrade_insecure_requests;
    String if_modified_since;
    String if_none_match;
    String cookie;
    String origin;
    String sec_fetch_site;

    String data;

    uint16_t mode = MODE_UNKNOWN;

private:
    String key;
};

class ServerUtils: public HTTPKeyValue
{
public:
    ServerUtils() {}
    ~ServerUtils() {}

    // ======= update_cache =======
    // input char to buffer
    // output : no action -> 0
    //          read complete -> 1
    // ===========================
    int update_cache(char c)
    {
        //  \n
        if (c == 10)
        {
            if (cache.length() > 0)
            {
                if (cache.indexOf("GET") == 0)
                {
                    // add value
                    url_dir = cache.substring(4, cache.indexOf("HTTP") - 1);
                    mode = MODE_GET;

                    if (url_dir.indexOf("gpio") > 0)
                        gpio_num = url_dir.substring(url_dir.indexOf("gpio") + 4).toInt();

                    // print
                    Serial.println("GET (Update): " + url_dir);
                }
                else if (cache.indexOf("POST") == 0)
                {
                    // add value
                    url_dir = cache.substring(5, cache.indexOf("HTTP") - 1);
                    mode = MODE_POST;

                    if (url_dir.indexOf("gpio") > 0)
                        gpio_num = url_dir.substring(url_dir.indexOf("gpio") + 4).toInt();

                    // print
                    Serial.println("POST (Update): " + url_dir);
                }
                else if (cache.indexOf("PUT") == 0)
                {
                    // add value
                    url_dir = cache.substring(4, cache.indexOf("HTTP") - 1);
                    mode = MODE_PUT;

                    if (url_dir.indexOf("gpio") > 0)
                        gpio_num = url_dir.substring(url_dir.indexOf("gpio") + 4).toInt();

                    // print
                    Serial.println("PUT (Update): " + url_dir);
                }
                else if (cache.indexOf("DELETE") == 0)
                {
                    // add value
                    url_dir = cache.substring(7, cache.indexOf("HTTP") - 1);
                    mode = MODE_DELETE;

                    if (url_dir.indexOf("gpio") > 0)
                        gpio_num = url_dir.substring(url_dir.indexOf("gpio") + 4).toInt();

                    // print
                    Serial.println("DELETE (Update): " + url_dir);
                }
                else if (cache.indexOf("Host:") == 0)
                {
                    // add value
                    host = cache.substring(6, cache.length());
                    // print
                    Serial.println("Host (Update): " + host);
                }
                else if (cache.indexOf("User-Agent:") == 0)
                {
                    // add value
                    user_agent = cache.substring(12, cache.length());
                    // print
                    Serial.println("User-Agent (Update): " + user_agent);
                }
                else if (cache.indexOf("Accept:") == 0)
                {
                    // add value
                    accept = cache.substring(8, cache.length());
                    // print
                    Serial.println("Accept (Update): " + accept);
                }
                else if (cache.indexOf("Accept-Encoding:") == 0)
                {
                    // add value
                    accept_encoding = cache.substring(17, cache.length());
                    // print
                    Serial.println("Accept-Encoding (Update): " + accept_encoding);
                }
                else if (cache.indexOf("Accept-Language:") == 0)
                {
                    // add value
                    accept_language = cache.substring(17, cache.length());
                    // print
                    Serial.println("Accept-Language (Update): " + accept_language);
                }
                else if (cache.indexOf("Connection:") == 0)
                {
                    // add value
                    connection = cache.substring(12, cache.length());
                    // print
                    Serial.println("Connection (Update): " + connection);
                }
                else if (cache.indexOf("Upgrade-Insecure-Requests:") == 0)
                {
                    // add value
                    upgrade_insecure_requests = cache.substring(27, cache.length());
                    // print
                    Serial.println("Upgrade-Insecure-Requests (Update): " + upgrade_insecure_requests);
                }
                else if (cache.indexOf("Content-Type:") == 0)
                {
                    // add value
                    content_type = cache.substring(14, cache.length());
                    // print
                    Serial.println("Content-Type (Update): " + content_type);
                }
                else if (cache.indexOf("Content-Length:") == 0)
                {
                    // add value
                    content_length = cache.substring(16, cache.length());
                    // to int
                    content_length_min = content_length.toInt();
                    // print
                    Serial.println("Content-Length (Update): " + content_length);
                }
                else if (cache.indexOf("Origin:") == 0)
                {
                    // add value
                    origin = cache.substring(8, cache.length());
                    // print
                    Serial.println("Origin (Update): " + origin);
                }
                else
                {
                    ;
                }
            }
            clear_cache();
        }
        else
        {
            cache += c;
            // cache lengh is over int content_length_int
            if (cache.length() == content_length_min)
            {
                data = cache;
                // print
                Serial.println("Data (Update): " + data);
                content_length_min = MAX_LENGETH;
                clear_cache();
                return 1;
            }
        }
        return 0;
    }

    // c
    String cache = "";

    void clear_cache()
    {
        cache = "";
    }

    // ======= gen_msg =======
    // create responce
    // - arg1 : status (200, 404, 500, ...)
    // - arg2 : content_type (text/html, text/plain, ...)
    // - arg3 : content (String)
    // =======================
    String gen_msg(uint16_t status, String content_type, String content)
    {
        String response = "";
        switch (status)
        {
        case 200:
            response = "HTTP/1.1 200 OK\r\n";
            break;
        case 400:
            response = "HTTP/1.1 400 Bad Request\r\n";
            break;
        case 404:
            response = "HTTP/1.1 404 Not Found\r\n";
            break;
        case 500:
            response = "HTTP/1.1 500 Internal Server Error\r\n";
            break;
        default:
            // unknown
            response = "HTTP/1.1 500 Internal Server Error\r\n";
            break;
        }

        response += "Content-Type: " + content_type + "\r\n";
        response += "Content-Length: " + String(content.length()) + "\r\n";
        response += "Connection: close\r\n";
        response += "\r\n";
        response += content;

        return response;
    }

    String handle_not_found()
    {
        return gen_msg(404, "text/plain", "Not Found.\r\n");
    }
};