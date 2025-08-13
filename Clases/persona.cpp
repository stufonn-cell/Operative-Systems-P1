#include <iostream>
#include <iomanip>  // Para formato de salida (setprecision, fixed)
#include <ctime> // Para obtener la fecha actual
#include <map>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include "persona.h"
#include "generador.h"

// Implementación del constructor
Persona::Persona(std::string nom, std::string ape, std::string id,
                 std::string ciudad, std::string fecha, double ingresos,
                 double patri, double deud, bool declara)
    : nombre(nom), apellido(ape), id(id), ciudadNacimiento(ciudad),
      fechaNacimiento(fecha), ingresosAnuales(ingresos), patrimonio(patri),
      deudas(deud), declaranteRenta(declara)
{
    // Inicialización mediante lista de inicialización para eficiencia
}

// --- Implementación de getters ---
// Devuelven valores de campos privados sin permitir modificación (const)

std::string Persona::getNombre() const { return nombre; }
std::string Persona::getApellido() const { return apellido; }
std::string Persona::getId() const { return id; }
std::string Persona::getCiudadNacimiento() const { return ciudadNacimiento; }
std::string Persona::getFechaNacimiento() const { return fechaNacimiento; }
double Persona::getIngresosAnuales() const { return ingresosAnuales; }
double Persona::getPatrimonio() const { return patrimonio; }
double Persona::getDeudas() const { return deudas; }
bool Persona::getDeclaranteRenta() const { return declaranteRenta; }

// Muestra información detallada de la persona
void Persona::mostrar() const {
    std::cout << "-------------------------------------\n";
    // Encabezado con ID y nombre completo
    std::cout << "[" << id << "] Nombre: " << nombre << " " << apellido << "\n";
    // Datos personales
    std::cout << "   - Ciudad de nacimiento: " << ciudadNacimiento << "\n";
    std::cout << "   - Fecha de nacimiento: " << fechaNacimiento << "\n\n";

    // Formato para valores monetarios (2 decimales)
    std::cout << std::fixed << std::setprecision(2);

    // Datos económicos
    std::cout << "   - Ingresos anuales: $" << ingresosAnuales << "\n";
    std::cout << "   - Patrimonio: $" << patrimonio << "\n";
    std::cout << "   - Deudas: $" << deudas << "\n";
    // Operador ternario para mostrar Sí/No según condición booleana
    std::cout << "   - Declarante de renta: " << (declaranteRenta ? "Sí" : "No") << "\n";
}

// Versión compacta para mostrar en listados
void Persona::mostrarResumen() const {
    // ID, nombre completo, ciudad e ingresos en una sola línea
    std::cout << "[" << id << "] " << nombre << " " << apellido
              << " | " << ciudadNacimiento
              << " | $" << std::fixed << std::setprecision(2) << ingresosAnuales;
}

// Clase auxiliar para manejar fechas
class Fecha {
    int anio, mes, dia;
public:
    Fecha(const std::string& fecha) {
        sscanf(fecha.c_str(), "%d-%d-%d", &anio, &mes, &dia);
    }
    int getAnio() const { return anio; }
    int getMes() const { return mes; }
    int getDia() const { return dia; }

    // Obtiene la fecha actual como objeto Fecha
    static Fecha hoy() {
        int anioActual = 2025;
        int mesActual = 12;
        int diaActual = 28;

        char buffer[11];
        snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", anioActual, mesActual, diaActual);
        return Fecha(buffer);
    }
};

int Persona::calcularEdad() const {
    Fecha nacimiento(fechaNacimiento);
    Fecha actual = Fecha::hoy();
    int edad = actual.getAnio() - nacimiento.getAnio();
    if (actual.getMes() < nacimiento.getMes() ||
        (actual.getMes() == nacimiento.getMes() && actual.getDia() < nacimiento.getDia())) {
        edad--;
    }
    return edad;
}

std::map<std::string, std::vector<Persona>> Persona::agruparPorCiudad(std::vector<Persona> &personas){
    std::map<std::string, std::vector<Persona>> grupos;

    for(const auto &persona : personas){
        std::string ciudadNacimiento = persona.getCiudadNacimiento();

    grupos[persona.getCiudadNacimiento()].push_back(persona);
    }

    return grupos;
}

std::map<std::string, std::vector<Persona>> Persona::agruparPorDeclaracion(std::vector<Persona> &personas) {
    std::map<std::string, std::vector<Persona>> grupos;

    for(const auto &persona : personas){
        std::string documento = persona.getId();
        std::string ultimosDigitos = documento.substr( (documento.size() - 2) );
        int digitosNumericos = std::stoi(ultimosDigitos);

        if (digitosNumericos <= 39) grupos["<39"].push_back(persona);
        else if (digitosNumericos <= 79) grupos["<79"].push_back(persona);
        else if (digitosNumericos <= 99) grupos["<99"].push_back(persona);
    }

    return grupos;
}


// ***************************** (1) Persona Mas Longeva *************************************
void Persona::personaMaxLongeva(std::vector<Persona> &personas, Persona &longeva){
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    longeva = *std::max_element(personas.begin(), personas.end(),
                [](const Persona &a, const Persona &b)
                {
                    return a.calcularEdad() < b.calcularEdad();
                });
}

Persona Persona::personaMaxLongevaValor(std::vector<Persona> personas){
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    return *std::max_element(personas.begin(), personas.end(),
                [](const Persona &a, const Persona &b)
                {
                    return a.calcularEdad() < b.calcularEdad();
                });
}


// ***************************** (2) Persona con Mayor Patrimonio *************************************

void Persona::personaMaxPatrimonio(std::vector<Persona> &personas, Persona &maxPatrimonio) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    maxPatrimonio = *std::max_element(personas.begin(), personas.end(),
                    [](const Persona &a, const Persona &b)
                    {
                        return a.getPatrimonio() < b.getPatrimonio();
                    });
}

Persona Persona::personaMaxPatrimonioValor(std::vector<Persona> &personas) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    return *std::max_element(personas.begin(), personas.end(),
            [](const Persona &a, const Persona &b)
            {
                return a.getPatrimonio() < b.getPatrimonio();
            });
}


// ***************************** (3) Declarantes de renta *************************************
int Persona::ultimosDosDigitosCC(const std::string& id) {
    int digito1= -1;
    int digito2= -1;

    for(int i = static_cast<int>(id.size()) -1; i >= 0; --i) {
        unsigned char counter = static_cast<unsigned char>(id[i]);
        if (std::isdigit(counter)) {
            if (digito1 == -1) {
                digito1 = counter - '0';
            } else if (digito2 == -1) {
                digito2 = counter - '0';
                break;
            }
        }
    }

    if (digito1 = -1) return -1;
    if (digito2 = -1) return digito1;
    return digito2 * 10 + digito1;
}

std::string Persona::grupoDIANusandoDigitos(int dd){
    if (dd < 0) return "Desconocido";
    if (dd <= 39) return "Grupo A";
    if (dd <= 79) return "Grupo B";
    if (dd <= 99) return "Grupo C";
    return "Desconocido";
}

std::string Persona::grupoDIAN2025(const Persona& persona) {
    int digitos = ultimosDosDigitosCC(persona.getId());
    std::cout << digitos << std::endl;
    return grupoDIANusandoDigitos(digitos);
}

int main() {
    std::vector<Persona> personas = generarColeccion(1000000);

    //Persona p("Juan", "Pérez", "123", "Bogotá", "1998-05-20", 50000, 200000, 10000, true);

    // std::string nombreMasLongevo = Persona::personaMaxLongeva(personas);

    // std::cout << nombreMasLongevo << std::endl;

    // auto agrupao = Persona::agruparPorCiudad(personas);

    // for (auto x : agrupao) {
    //     std::cout << x.first << " " << x.second.size() << std::endl;
    //     std::string longevo = Persona::personaMaxLongevaValor(x.second);
    //     std::cout << "Persona más longeva en " << x.first << ": " << longevo << std::endl;
    // }

    auto grupoDeclaracion = Persona::agruparPorDeclaracion(personas);

    for (auto grupo : grupoDeclaracion) {
        std::cout << grupo.first << " " << grupo.second.size() << std::endl;
    }

    return 0;
}