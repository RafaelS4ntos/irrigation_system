import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;

@SuppressWarnings("unchecked") // Suprimir o aviso de tipo inseguro
public class ArduinoCommunication {

    public static void main(String[] args) {
        communicateWithESP8266();
        createSensorDataTable();
    }

    public static void createSensorDataTable() {
        // Informações de conexão com o banco de dados MySQL
        String jdbcURL = "jdbc:mysql://localhost:3306/sensor_data"; // Substitua pelo nome do seu banco de dados
        String username = "seu_usuario"; // Substitua pelo nome de usuário do MySQL
        String password = "sua_senha"; // Substitua pela senha do MySQL

        // Comando SQL para criar a tabela "sensor_data"
        String createTableSQL = "CREATE TABLE sensor_data (" +
                "sensor_data_id INT AUTO_INCREMENT PRIMARY KEY," +
                "sensor_type VARCHAR(255) NOT NULL," +
                "sensor_value DECIMAL(10, 2) NOT NULL," +
                "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP" +
                ")";

        try {
            // Estabelece a conexão com o banco de dados
            Connection connection = DriverManager.getConnection(jdbcURL, username, password);

            // Cria um objeto de instrução SQL
            Statement statement = connection.createStatement();

            // Executa o comando SQL para criar a tabela (somente se ela não existir)
            statement.executeUpdate(createTableSQL);

            // Fecha a conexão e o objeto de instrução
            statement.close();
            connection.close();

            System.out.println("Tabela 'sensor_data' criada com sucesso!");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }    

    public static void communicateWithESP8266() {
        try {
            Enumeration<CommPortIdentifier> portList = CommPortIdentifier.getPortIdentifiers();

            while (portList.hasMoreElements()) {
                CommPortIdentifier portId = portList.nextElement();

                if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                    if (portId.getName().equals("/dev/ttyUSB0")) { // Substitua pelo nome da porta correta
                        SerialPort serialPort = (SerialPort) portId.open("ESP8266", 2000);

                        serialPort.setSerialPortParams(115200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

                        InputStream in = serialPort.getInputStream();
                        OutputStream out = serialPort.getOutputStream();

                        out.write("AT\r\n".getBytes());

                        byte[] buffer = new byte[1024];
                        int len;
                        StringBuilder response = new StringBuilder();

                        while ((len = in.read(buffer)) > -1) {
                            response.append(new String(buffer, 0, len));
                            if (response.toString().contains("OK")) {
                                break;
                            }
                        }

                        System.out.println("Resposta do ESP8266: " + response.toString());

                        in.close();
                        out.close();
                        serialPort.close();
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
