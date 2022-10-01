#pragma once

#include <Arduino.h>

#define MAX_LENGETH 1024

#define MODE_GET 0
#define MODE_POST 1
#define MODE_PUT 2
#define MODE_DELETE 3
#define MODE_UNKNOWN 4

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
    String sec_fetch_site;

    String data;

    uint16_t mode = MODE_UNKNOWN;

private:
    String key;
};

class ServerUtils: public HTTPKeyValue
{
public:
    ServerUtils() {
    }
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
                    url_dir = cache.substring(4, cache.indexOf("HTTP") - 1);
                    mode = MODE_GET;

                    if (url_dir.indexOf("gpio") > 0)
                        gpio_num = url_dir.substring(url_dir.indexOf("gpio") + 4).toInt();

                    clear_cache();
                    return 1;
                }
                else if (cache.indexOf("POST") == 0)
                {
                    url_dir = cache.substring(5, cache.indexOf("HTTP") - 1);
                    mode = MODE_POST;

                    if (url_dir.indexOf("gpio") > 0)
                        gpio_num = url_dir.substring(url_dir.indexOf("gpio") + 4).toInt();
                }
                else if (cache.indexOf("PUT") == 0)
                {
                    url_dir = cache.substring(4, cache.indexOf("HTTP") - 1);
                    mode = MODE_PUT;

                    if (url_dir.indexOf("gpio") > 0)
                        gpio_num = url_dir.substring(url_dir.indexOf("gpio") + 4).toInt();
                }
                else if (cache.indexOf("DELETE") == 0)
                {
                    url_dir = cache.substring(7, cache.indexOf("HTTP") - 1);
                    mode = MODE_DELETE;

                    if (url_dir.indexOf("gpio") > 0)
                        gpio_num = url_dir.substring(url_dir.indexOf("gpio") + 4).toInt();

                    clear_cache();
                    return 1;
                }
                else if (cache.indexOf("Host:") == 0)
                {
                    host = cache.substring(6, cache.length());
                }
                else if (cache.indexOf("User-Agent:") == 0)
                {
                    user_agent = cache.substring(12, cache.length());
                }
                else if (cache.indexOf("Accept:") == 0)
                {
                    accept = cache.substring(8, cache.length());
                }
                else if (cache.indexOf("Accept-Encoding:") == 0)
                {
                    accept_encoding = cache.substring(17, cache.length());
                }
                else if (cache.indexOf("Accept-Language:") == 0)
                {
                    accept_language = cache.substring(17, cache.length());
                }
                else if (cache.indexOf("Connection:") == 0)
                {
                    connection = cache.substring(12, cache.length());
                }
                else if (cache.indexOf("Upgrade-Insecure-Requests:") == 0)
                {
                    upgrade_insecure_requests = cache.substring(27, cache.length());
                }
                else if (cache.indexOf("Content-Type:") == 0)
                {
                    content_type = cache.substring(14, cache.length());
                }
                else if (cache.indexOf("Content-Length:") == 0)
                {
                    content_length = cache.substring(16, cache.length());
                    content_length_min = content_length.toInt();
                    Serial.println("Content-Length (Update): " + content_length);
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
            Serial.print(cache);
            if (content_length_min == 1)
            {
                if (skip_read_flag_for_char)
                {
                    skip_read_flag_for_char = false;
                    return 0;
                }
            }

            if (cache.length() == content_length_min)
            {
                data = cache;
                Serial.println("Data (Update): " + data);
                content_length_min = MAX_LENGETH;
                skip_read_flag_for_char = true;
                clear_cache();
                return 1;
            }
        }
        return 0;
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

private:
    bool skip_read_flag_for_char = true;
    String cache = "";

    void clear_cache()
    {
        cache = "";
    }
};