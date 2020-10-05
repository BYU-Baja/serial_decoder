#include <mqtt.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "mqtt_client.h"
#include "posix_sockets.h"

#define SEND_BUF_SIZE 2048
#define RECV_BUF_SIZE 1024
#define USERNAME NULL
#define PASSWORD NULL

/**
 * @brief The function will be called whenever a PUBLISH message is received.
 */
static void mqtt_client_publish_callback(void **unused, struct mqtt_response_publish *published);

/**
 * @brief The client's refresher. This function triggers back-end routines to
 *        handle ingress/egress traffic to the broker.
 *
 * @note All this function needs to do is call \ref __mqtt_recv and
 *       \ref __mqtt_send every so often. I've picked 100 ms meaning that
 *       client ingress/egress traffic will be handled every 100 ms.
 */
static void *mqtt_client_client_refresher(void *client);

static struct mqtt_client client;
static uint8_t sendbuf[SEND_BUF_SIZE]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
static uint8_t recvbuf[RECV_BUF_SIZE]; /* recvbuf should be large enough any whole mqtt message expected to be received */
static int sockfd;
static pthread_t client_daemon;
static char *mqtt_host;
static char *mqtt_port;
static char hostname[HOST_NAME_MAX];
static void (*message_handler)(char *topic, uint8_t *buf, size_t buf_len);

int mqtt_client_init(char *host, char *port)
{
    int result;
    result = gethostname(hostname, HOST_NAME_MAX);
    if (result)
    {
        perror("gethostname");
        return EXIT_FAILURE;
    }

    printf("MQTT -> client name: %s\n", hostname);

    mqtt_host = (char *)malloc(strlen(host) + 1);
    mqtt_port = (char *)malloc(strlen(port) + 1);
    strcpy(mqtt_host, host);
    strcpy(mqtt_port, port);

    sockfd = open_nb_socket(mqtt_host, mqtt_port);
    mqtt_init(&client, sockfd, sendbuf, SEND_BUF_SIZE, recvbuf, RECV_BUF_SIZE, mqtt_client_publish_callback);
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    mqtt_connect(&client, hostname, NULL, NULL, 0, USERNAME, PASSWORD, connect_flags, 400);

    /* check that we don't have any errors */
    if (client.error != MQTT_OK)
    {
        fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
        mqtt_client_close();
        return EXIT_FAILURE;
    }

    if (pthread_create(&client_daemon, NULL, mqtt_client_client_refresher, &client))
    {
        fprintf(stderr, "Failed to start client daemon.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int mqtt_client_publish(char *topic, uint8_t *buf, size_t buf_len)
{
    int result = mqtt_publish(&client, topic, buf, buf_len, MQTT_PUBLISH_QOS_1);
    if (result != MQTT_OK)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int mqtt_client_subscribe(char *topic)
{
    mqtt_subscribe(&client, topic, MQTT_PUBLISH_QOS_1);
    return EXIT_SUCCESS;
}

int mqtt_client_unsubscribe(char *topic)
{
    mqtt_unsubscribe(&client, topic);
    return EXIT_SUCCESS;
}

int mqtt_client_set_subscribe_handler()
{

    return EXIT_SUCCESS;
}

void mqtt_client_close()
{
    if (sockfd != -1)
        close(sockfd);
    pthread_cancel(client_daemon);
}

static void mqtt_client_publish_callback(void **unused, struct mqtt_response_publish *published)
{
    /* note that published->topic_name is NOT null-terminated (here we'll change it to a c-string) */
    char *topic_name = (char *)malloc(published->topic_name_size + 1);
    memcpy(topic_name, published->topic_name, published->topic_name_size);
    topic_name[published->topic_name_size] = '\0';

    printf("Received publish('%s'): %s\n", topic_name, (const char *)published->application_message);

    if (message_handler != NULL)
        message_handler(topic_name, (uint8_t *)published->application_message, published->application_message_size);

    free(topic_name);
}

static void *mqtt_client_client_refresher(void *client)
{
    while (1)
    {
        mqtt_sync((struct mqtt_client *)client);
        usleep(100000U);
    }
    return NULL;
}
