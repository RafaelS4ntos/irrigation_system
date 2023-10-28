# irrigation_system

# Sistema de Irrigação

Este projeto é um sistema de irrigação automatizado que monitora a umidade do solo e a temperatura ambiente para controlar a irrigação de plantas. Ele se comunica com um dispositivo ESP8266 (como um NodeMCU) para obter informações dos sensores e grava os dados em um banco de dados MySQL.

## Pré-requisitos

Antes de executar o projeto, certifique-se de ter as seguintes ferramentas e recursos instalados:

- Dispositivo ESP8266 (por exemplo, NodeMCU).
- Arduino IDE para carregar o código no ESP8266.
- MySQL Server instalado e configurado.
- Driver JDBC do MySQL para Java.
- Java Development Kit (JDK).
- Git (opcional para clonar o repositório do GitHub).

## Como Executar

1. **Arduino Communication:**
   - Abra o projeto Arduino no Arduino IDE.
   - Carregue o código no dispositivo ESP8266.
   - Certifique-se de que o ESP8266 está conectado ao sensor de umidade do solo e ao sensor de temperatura.
   - O ESP8266 se comunicará com o servidor MySQL para gravar dados.

2. **Configurar o Servidor MySQL:**
   - Certifique-se de que o servidor MySQL esteja em execução.
   - Crie um banco de dados chamado `sensor_data`.
   - Execute o script SQL fornecido para criar a tabela `sensor_data` no banco de dados.

3. **Configurar o Driver JDBC:**
   - Certifique-se de que o driver JDBC do MySQL esteja em seu projeto.
   - Configure a URL de conexão para o banco de dados MySQL no código Java.

4. **Executar o Projeto Java:**
   - Execute o código Java para conectar-se ao servidor MySQL e gravar os dados dos sensores.
   - Certifique-se de que o projeto Java seja capaz de se comunicar com o dispositivo ESP8266.
   
   Exemplo linha de comando:
   ```
   $ javac -cp .:lib/mysql-connector-java-8.1.0.jar Main.java
   ```
