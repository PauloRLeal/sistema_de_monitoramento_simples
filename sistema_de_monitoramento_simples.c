#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/netif.h" // Para acessar netif_default e IP

// Configurações de Wi-Fi
#define WIFI_SSID "Paulo"
#define WIFI_PASSWORD "paulo1243"

// Definição dos pinos dos LEDs
#define BTN_A_PIN 5
#define BTN_B_PIN 6
#define JOYSTICK_X_PIN 26
#define JOYSTICK_Y_PIN 27

// Globais
int btn_a_state, btn_b_state;
float x_position, y_position;
char wind_rose_direction[20] = "Centro";

// ---------- Funções Auxiliares ----------
void read_buttons()
{
    btn_a_state = !gpio_get(BTN_A_PIN);
    btn_b_state = !gpio_get(BTN_B_PIN);
}

void calculate_wind_direction(float x, float y)
{
    const float threshold = 0.6;
    const float margin = 0.2;

    if (fabs(x) <= margin && fabs(y) <= margin)
    {
        strcpy(wind_rose_direction, "Centro");
    }
    else if (y >= threshold && fabs(x) <= margin)
    {
        strcpy(wind_rose_direction, "Norte");
    }
    else if (y <= -threshold && fabs(x) <= margin)
    {
        strcpy(wind_rose_direction, "Sul");
    }
    else if (x >= threshold && fabs(y) <= margin)
    {
        strcpy(wind_rose_direction, "Leste");
    }
    else if (x <= -threshold && fabs(y) <= margin)
    {
        strcpy(wind_rose_direction, "Oeste");
    }
    else if (y >= threshold && x >= threshold)
    {
        strcpy(wind_rose_direction, "Nordeste");
    }
    else if (y >= threshold && x <= -threshold)
    {
        strcpy(wind_rose_direction, "Noroeste");
    }
    else if (y <= -threshold && x >= threshold)
    {
        strcpy(wind_rose_direction, "Sudeste");
    }
    else if (y <= -threshold && x <= -threshold)
    {
        strcpy(wind_rose_direction, "Sudoeste");
    }
    else
    {
        strcpy(wind_rose_direction, "Centro");
    }
}

int direction_to_number(const char *dir)
{
    if (strcmp(dir, "Centro") == 0)
        return 0;
    if (strcmp(dir, "Norte") == 0)
        return 1;
    if (strcmp(dir, "Sul") == 0)
        return 2;
    if (strcmp(dir, "Leste") == 0)
        return 3;
    if (strcmp(dir, "Oeste") == 0)
        return 4;
    if (strcmp(dir, "Nordeste") == 0)
        return 5;
    if (strcmp(dir, "Noroeste") == 0)
        return 6;
    if (strcmp(dir, "Sudeste") == 0)
        return 7;
    if (strcmp(dir, "Sudoeste") == 0)
        return 8;
    return -1;
}


float converterValor(float input)
{
    float min_input = 0;
    float max_input = 4096;
    float range = 200;

    // Verifica se o input está dentro dos limites
    if (input < min_input)
        input = min_input;
    if (input > max_input)
        input = max_input;

    if (input >= 1800 && input <= 2200)
        return 0.0;

    // Calcula o valor normalizado
    float output = -1.0 + 2.0 * (input - min_input) / (max_input - min_input);

    // Arredonda para os limites do range de coleta
    float steps = (max_input - min_input) / range;
    output = ((float)(output * steps + 0.5)) / (float)steps;

    if (output > 1)
        output = 1;
    else if (output < 1)
        output = -1;

    return output;
}

void read_analog()
{
    adc_select_input(1);
    x_position = converterValor(adc_read());
    adc_select_input(0);
    y_position = converterValor(adc_read());
    printf("X: %.2f, Y: %.2f", x_position, y_position);
    calculate_wind_direction(x_position, y_position);
}

// Função de callback para processar requisições HTTP
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    char *request = (char *)malloc(p->len + 1);
    memcpy(request, p->payload, p->len);
    request[p->len] = '\0';

    printf("Request: %s\n", request);


    // Cria a resposta HTML
    char html[1024];

    snprintf(html, sizeof(html),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "<!DOCTYPE html>\n"
             "<html>\n"
             "<head>\n"
             "<title>LED Control</title>\n"
             "<style>\n"
             "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
             "h1 { font-size: 64px; margin-bottom: 30px; }\n"
             "button { font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
             ".button { font-size: 48px; margin-top: 30px; color: #333; }\n"
             "</style>\n"
             "</head>\n"
             "<body>\n"
             "<h1>Sistema de monitoramento</h1>\n"
             "<p class=\"button\">Botao A: %d</p>\n"
             "<p class=\"button\">Botao B: %d</p>\n"
             "<p class=\"button\">Joystick X: %.2f</p>\n"
             "<p class=\"button\">Joystick Y: %.2f</p>\n"
             "<p class=\"button\">Rosa dos Ventos: %s</p>\n"

             "</body>\n"
             "</html>\n",
             btn_a_state, btn_b_state, x_position, y_position, wind_rose_direction);

    /*       snprintf(html, sizeof(html),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "<!DOCTYPE html>\n"
             "<html>\n"
             "<head>\n"
             "<title>LED Control</title>\n"
             "<style>\n"
             "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
             "h1 { font-size: 64px; margin-bottom: 30px; }\n"
             "button { font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
             ".temperature { font-size: 48px; margin-top: 30px; color: #333; }\n"
             "</style>\n"
             "</head>\n"
             "<body>\n"
             "<h1>LED Control</h1>\n"
             "<form action=\"./blue_on\"><button style=\"background-color: #2196F3; color: white;\">Ligar Azul</button></form>\n"
             "<form action=\"./blue_off\"><button style=\"background-color: #BBDEFB; color: black;\">Desligar Azul</button></form>\n"
             "<form action=\"./green_on\"><button style=\"background-color: #4CAF50; color: white;\">Ligar Verde</button></form>\n"
             "<form action=\"./green_off\"><button style=\"background-color: #C8E6C9; color: black;\">Desligar Verde</button></form>\n"
             "<form action=\"./red_on\"><button style=\"background-color: #F44336; color: white;\">Ligar Vermelho</button></form>\n"
             "<form action=\"./red_off\"><button style=\"background-color: #FFCDD2; color: black;\">Desligar Vermelho</button></form>\n"
             "<p class=\"temperature\">Temperatura Interna: %.2f &deg;C</p>\n"
             "</body>\n"
             "</html>\n",
             temperature);
    */

    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);

    free(request);
    pbuf_free(p);

    return ERR_OK;
}

// Função de callback ao aceitar conexões TCP
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    tcp_recv(newpcb, tcp_server_recv);
    return ERR_OK;
}

// Função principal
int main()
{
    stdio_init_all();

    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);
    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN);
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // cyw43_arch_deinit(); // Desativa o Wi-Fi

    while (cyw43_arch_init())
    {
        printf("Falha ao inicializar Wi-Fi\n");
        sleep_ms(100);
        return -1;
    }

    cyw43_arch_gpio_put(LED_PIN, 0);
    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi...\n");
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 20000))
    {
        printf("Falha ao conectar ao Wi-Fi\n");
        sleep_ms(100);
        return -1;
    }

    printf("Conectado ao Wi-Fi\n");

    if (netif_default)
    {
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    // Configura o servidor TCP
    struct tcp_pcb *server = tcp_new();
    if (!server)
    {
        printf("Falha ao criar servidor TCP\n");
        return -1;
    }

    if (tcp_bind(server, IP_ADDR_ANY, 80) != ERR_OK)
    {
        printf("Falha ao associar servidor TCP à porta 80\n");
        return -1;
    }

    server = tcp_listen(server);
    tcp_accept(server, tcp_server_accept);

    printf("Servidor ouvindo na porta 80\n");


    while (true)
    {
        read_buttons();
        read_analog();
        cyw43_arch_poll();
        sleep_ms(1000); // Delay de 1s
    }

    cyw43_arch_deinit();
    return 0;
}
