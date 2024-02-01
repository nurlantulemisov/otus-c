#include <stdio.h>
#include <curl/curl.h>
#include <jansson.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

char *response;

void print_help(void)
{
    printf("Usage: ./myprog -c <input_path>\n");
    printf("Options:\n");
    printf("  -c <input_path>  City\n");
    printf("  -h               Print this help message\n");
}

size_t write_response(void *contents, size_t size, size_t nmemb)
{
    size_t realsize = size * nmemb;
    response = realloc(response, response ? strlen(response) + realsize + 1 : realsize + 1);
    if (response == NULL)
    {
        return 0;
    }
    memcpy(&(response[strlen(response)]), contents, realsize);
    response[strlen(response) + realsize] = 0;
    return realsize;
}

const char *convert_to_angle(int angle)
{
    if (angle >= 0 && angle < 22.5)
    {
        return "\u2191"; // Северный
    }
    else if (angle >= 22.5 && angle < 67.5)
    {
        return "\u2197"; // Северо-восточный
    }
    else if (angle >= 67.5 && angle < 112.5)
    {
        return "\u2192"; // Восточный
    }
    else if (angle >= 112.5 && angle < 157.5)
    {
        return "\u2198"; // Юго-восточный
    }
    else if (angle >= 157.5 && angle < 202.5)
    {
        return "\u2193"; // Южный
    }
    else if (angle >= 202.5 && angle < 247.5)
    {
        return "\u2199"; // Юго-западный
    }
    else if (angle >= 247.5 && angle < 292.5)
    {
        return "\u2190"; // Западный
    }
    else if (angle >= 292.5 && angle < 337.5)
    {
        return "\u2196"; // Северо-западный
    }
    else
    {
        return "\u2191"; // Северный
    }
}

const char *wheather_location(json_t *root)
{
    json_t *request = json_object_get(root, "request");
    if (json_array_size(request) > 0)
    {
        json_t *query = json_object_get(json_array_get(request, 0), "query");
        if (json_is_string(query))
        {
            return json_string_value(query);
        }
    }

    return NULL;
}

const char *condition_field(json_t *current_condition, const char *key)
{
    json_t *field_val = json_object_get(json_array_get(current_condition, 0), key);
    if (json_is_string(field_val))
    {
        return json_string_value(field_val);
    }

    if (json_array_size(field_val) > 0)
    {
        json_t *value = json_object_get(json_array_get(field_val, 0), "value");
        if (json_is_string(value))
        {
            return json_string_value(value);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    char *city = NULL;

    int c;
    while ((c = getopt(argc, argv, "c:h")) != -1)
    {
        switch (c)
        {
        case 'c':
            city = optarg;
            break;
        case 'h':
            print_help();
            return 0;
        default:
            printf("Invalid option: %c\n", c);
            return 1;
        }
    }

    if (city == NULL)
    {
        printf("Missing required options: -c\n");
        return EXIT_FAILURE;
    }

    CURL *curl;
    CURLcode res;

    response = malloc(1);
    *response = '\0';

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    char url[100];

    sprintf(url, "https://wttr.in/%s?format=j1", city);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
    }
    else
    {
        json_error_t error;
        json_t *root = json_loads(response, 0, &error);

        if (root)
        {
            printf("City: %s\n", city);
            const char *location = wheather_location(root);
            printf("Location: %s\n", location);

            json_t *current_condition = json_object_get(root, "current_condition");
            if (json_array_size(current_condition) > 0)
            {
                const char *temp_c = condition_field(current_condition, "temp_C");
                const char *weather_desc = condition_field(current_condition, "weatherDesc");
                const char *wind_speed = condition_field(current_condition, "windspeedKmph");
                const char *wind_degree = condition_field(current_condition, "winddirDegree");

                printf("Temprature: %s\nWeather description: %s\nWind speed: %s\nWind degree: %s", temp_c, weather_desc, wind_speed, convert_to_angle(atoi(wind_degree)));
            }

            json_decref(root);
        }
    }

    curl_easy_cleanup(curl);
    free(response);

    curl_global_cleanup();

    return EXIT_SUCCESS;
}
