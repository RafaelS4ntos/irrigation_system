import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;

public class CreateSensorDataTable {

    public static void main(String[] args) {
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

            // Executa o comando SQL para criar a tabela
            statement.executeUpdate(createTableSQL);

            // Fecha a conexão e o objeto de instrução
            statement.close();
            connection.close();

            System.out.println("Tabela 'sensor_data' criada com sucesso!");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
