# Tarefa U2C3 EmbarcaTech: Sistema IoT (servidor local - via IP na rede Wi-Fi)

Este repositório apresenta um projeto de Internet das Coisas (IoT) utilizando a placa BitDogLab, centrada no microcontrolador Raspberry Pi Pico W. O objetivo é realizar a coleta e transmissão de dados oriundos de sensores digitais e analógicos, como botões e joystick, expondo os dados mais recentes em uma interface web através do IP da BitDogLab no Wi-Fi cadastrado. Essa implementação reforça o aprendizado prático de redes embarcadas e sensores no contexto da computação física.

## Objetivos do projeto

- Desenvolver a leitura de sensores digitais e analógicos na placa BitDogLab, através do microcontrolador Raspberry Pi Pico W.
- Explorar o uso de bibliotecas de rede e conexão Wi-Fi em C.
- Implementar comunicação de dados com serviços de nuvem utilizando HTTP.
- Mapear os eixos do joystick e convertê-los em direções cardeais (rosa dos ventos).
- Utilizar o servidor local como plataforma de visualização dos dados.
- Desenvolver e exibir os dados de forma dinâmica em uma página web.


## Funcionalidades

- Comunicação via Wi-Fi.
- Coleta do status dos botões A e B da placa de desenvolvimento BitDogLab.
- Coleta dos dados dos eixos X e Y do joystick.
- Envio dos dados coletados para um servidor local (através do IP da BitDogLab na rede Wi-Fi).


## Estrutura do Repositório
```
/
├── sistema_de_monitoramento_simples.c           # Arquivo C principal
├── CMakeList.txt                                # Arquivo de compilação
├── img/                                         # Pasta com imagens do projeto
└── README.md                                    # Este arquivo
```

## Configuração e Uso

### Compatibilidade
- Placa de desenvolvimento BitDogLab.
- Raspberry Pi Pico W.

### Compilação e Upload
1. Clone este repositório:
   ```
   https://github.com/PauloRLeal/sistema_de_monitoramento_simples.git
   ```
2. Execute a extensão **Raspberry Pi Pico** do Visual Studio Code e abra o projeto na subpasta robo/
3. Atualize as linhas 13 e 14 do código com as suas devidas credenciais:
   ```
   #define WIFI_SSID "your_ssid" 
   ```

   ```
   #define WIFI_PASSWORD "your_password" 
   ```

4. Compile o código do sistema IoT para monitoramento remoto através da extensão
5. Faça o upload do firmware para o microcontrolador
6. Ligue o sistema IoT para monitoramento remoto e aguarde a conexão bem sucedida na rede WiFi. (Caso não conecte, reset o sistema)


## Possíveis dificuldades
Caso o seu sistema de controle e monitoramento não apresente o IP para acesso a página para acompanhamento dos dados coletados ou a pagina web não carregue, verifique se configurou corretamente as informações da rede Wi-Fi.

## Layout da página Web
<img src="img/Captura de tela 2025-05-11 001507.png">

## Direitos de Uso
Este projeto é disponibilizado para leitura e citação em outros trabalhos. Caso utilize este projeto como referência, por favor, forneça os devidos créditos ao autor.

## Autor
Desenvolvido por Paulo Roberto Araújo Leal.

## Anexos
- [Versão com servidor online (ThingSpeak)](https://github.com/PauloRLeal/comunicacao_iot_u2c3.git)
- [Vídeo no youtube com explicação e o projeto em funcionamento com o servidor online do ThingSpeak](https://youtu.be/eVDgW0HW090?si=Aiii0x1b1JSktnU9)
- [Página web desenvolvida para visualização dos dados enviados ao servidor ThingSpeak](https://v0-real-time-web-app-nu.vercel.app/)
- [Visualização dos gráficos no servidor do ThingSpeak](https://thingspeak.mathworks.com/channels/2952597)
- [Documentação explicativa do projeto com o servidor online (ThingSpeak)](https://docs.google.com/document/d/1QmjtPQbKv75fYgCW9ZC__xgsdDB3R5dtR7qd71a4LPk/edit?usp=sharing)
