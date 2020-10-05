#ifndef __MQTT_CLIENT_H
#define __MQTT_CLIENT_H

#include <stdint.h>
#include <stdlib.h>

int mqtt_client_init(char *host, char *port);
int mqtt_client_publish(char *topic, uint8_t *buf, size_t buf_len);
int mqtt_client_subscribe(char *topic);
int mqtt_client_unsubscribe(char *topic);
int mqtt_client_set_subscribe_handler();

/**
 * @brief Safelty closes the \p sockfd and cancels the \p client_daemon before \c exit.
 */
void mqtt_client_close();

#endif
