#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <math.h>
# define M_PI           3.14159265358979323846
int main()
{
    std::ifstream arquivo_in("attitude_exam.log"); //abre o arquivo de dados da leitura do acelerometro
   
    
    //abre o console pro usuario digitar o nome do arquivo em que ele quer os dados de saida
    std::string user_in;
    std::cout << "Type the file name and extension(saida.txt for example) ";
    std::cin >> user_in;

    // Verifica se o arquivo ja existe
    std::ifstream e_file(user_in.c_str());
    if (e_file) {
        std::cerr << "Error: the file already exists" << std::endl;
        return 1; // encerra o programa caso o arquivo ja exista
    }
    
        std::ofstream arquivo_out(user_in.c_str()); //cria o arquivo de saida 
    
        
    if (arquivo_in.is_open())
    {
        // da linha 31 ate a 51 sao definidas as variaveis usadas
        std::string linha; // string pra armazenar as linhas do arquivo de dados
        std::vector<std::string> dados_acelerometro; //cria um vetor para serem armazenados os dados das medicoes
        std::string leituraxyz; //variavel para armazenar as leituras uma por uma
        std::string time_stamp; 
        
        double tg_roll; 
        double tg_pitch;
        double roll_error; //erro de roll, devido a aproximacao
        double re_num;
        double re_den;
        double G_px;// roll
        double G_py;//pitch
        double G_pz;//yaw
        double arctan_roll_rad;
        double roll_angle; //angulo roll
        double arctan_pitch_rad;
        double pitch_angle; //angulo pitch
        double k = 0.001; //constante para a aproximacao
        
        int sign_G_pz; //variavel auxiliar para a aproximacao 

        while (std::getline(arquivo_in, linha)) //le todas as linhas do arquivo de entrada
        {
            std::stringstream ss(linha);
            for (int i = 1; i <= 4; i++) { 
                std::getline(ss, leituraxyz, ';');
                dados_acelerometro.push_back(leituraxyz);     
            } // separa os dados das linhas do arquivo, e os armazena em um vetor de dimensao 4
            time_stamp = dados_acelerometro[0];
            G_px = std::stod(dados_acelerometro[1]);//le os dados no vetor e converte de string para double
            G_py = std::stod(dados_acelerometro[2]);
            G_pz = std::stod(dados_acelerometro[3]);           
            dados_acelerometro.clear(); 
            if (G_pz < 0) { //funcao sign, para a aproximacao 
                sign_G_pz = -1;
            }
            else
            {
                sign_G_pz = 1;
            }

            tg_roll = G_py / sign_G_pz * sqrt(pow(G_pz, 2.0) + k*pow(G_px, 2.0));
            tg_pitch = -G_px / sqrt(pow(G_py, 2.0) + pow(G_pz, 2.0));
            arctan_roll_rad = std::atan(tg_roll);
            roll_angle = arctan_roll_rad *180/M_PI; //calcula o angulo roll
            arctan_pitch_rad = std::atan(tg_pitch);
            pitch_angle = arctan_pitch_rad * 180 / M_PI; //calcula o angulo pitch
            re_num = G_py * (G_pz - sqrt(pow(G_pz, 2.0)+k* pow(G_px, 2.0)));
            re_den = pow(G_py, 2.0) + G_pz * sqrt(pow(G_pz, 2.0) + k * pow(G_px, 2.0));
            roll_error = std::atan2(re_num,re_den); //calcula o erro de angulo roll

            arquivo_out<< time_stamp<< " " << "The pitch angle is" << " " << pitch_angle << (char)176 << " " << "and the roll angle is" << " " << roll_angle << (char)176 << " " << "(" << roll_error << (char)176 << " " << "of roll error)" << std::endl;
            
        }

        arquivo_in.close();
        
    }
    else
    {
        std::cout << "Erro ao abrir o arquivo" << std::endl;
    
    }

    return 0;
}
