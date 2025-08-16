#include <iostream>
#include <iomanip>  // Para formato de salida (setprecision, fixed)
#include <ctime> // Para obtener la fecha actual
#include <map>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include "persona.h"
#include "generador.h"

// Constructor por defecto
Persona::Persona(){}

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

std::map<std::string, std::vector<Persona>> Persona::agruparPorCiudadValor(const std::vector<Persona> personas){
    std::map<std::string, std::vector<Persona>> grupos;

    for(const auto &persona : personas){
        std::string ciudadNacimiento = persona.getCiudadNacimiento();

        grupos[persona.getCiudadNacimiento()].push_back(persona);
    }

    return grupos;
}

void Persona::agruparPorCiudad(const std::vector<Persona> &personas,
                               std::map<std::string, std::vector<Persona>> &grupos) {

    for(const auto &persona : personas){
        std::string ciudadNacimiento = persona.getCiudadNacimiento();

        grupos[persona.getCiudadNacimiento()].push_back(persona);
    }
}

void Persona::agruparPorDeclaracion(const std::vector<Persona> &personas,
                                    std::map<std::string, std::vector<Persona>> &grupos) {
    for(const auto &persona : personas){
        std::string documento = persona.getId();
        std::string ultimosDigitos = documento.substr( (documento.size() - 2) );
        int digitosNumericos = std::stoi(ultimosDigitos);

        if (digitosNumericos <= 39) grupos["A"].push_back(persona);
        else if (digitosNumericos <= 79) grupos["B"].push_back(persona);
        else if (digitosNumericos <= 99) grupos["C"].push_back(persona);
    }
}

std::map<std::string, std::vector<Persona>> Persona::agruparPorDeclaracionValor(const std::vector<Persona> personas) {
    std::map<std::string, std::vector<Persona>> grupos;

    for(const auto &persona : personas){
        std::string documento = persona.getId();
        std::string ultimosDigitos = documento.substr( (documento.size() - 2) );
        int digitosNumericos = std::stoi(ultimosDigitos);

        if (digitosNumericos <= 39) grupos["A"].push_back(persona);
        else if (digitosNumericos <= 79) grupos["B"].push_back(persona);
        else if (digitosNumericos <= 99) grupos["C"].push_back(persona);
    }

    return grupos;
}


// ***************************** (1) Persona Mas Longeva *************************************
void Persona::personaMaxLongeva(const std::vector<Persona> &personas, Persona &longeva){
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    longeva = *std::max_element(personas.begin(), personas.end(),
                [](const Persona &a, const Persona &b)
                {
                    return a.calcularEdad() < b.calcularEdad();
                });
}

Persona Persona::personaMaxLongevaValor(const std::vector<Persona> personas){
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

void Persona::personaMaxPatrimonio(const std::vector<Persona> &personas, Persona &maxPatrimonio) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    maxPatrimonio = *std::max_element(personas.begin(), personas.end(),
                    [](const Persona &a, const Persona &b)
                    {
                        return a.getPatrimonio() < b.getPatrimonio();
                    });
}

Persona Persona::personaMaxPatrimonioValor(const std::vector<Persona> personas) {
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
    if (digito1 == -1) return -1;
    if (digito2 == -1) return digito1;
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

std::map<std::string, std::vector<const Persona*>>
Persona::agruparDeclarantesPorCalendarioPtr(const std::vector<Persona>& personas,
                                            std::map<std::string, int>* contador) {
    std::map<std::string, std::vector<const Persona*>> grupos;


    grupos["Grupo A"];
    grupos["Grupo B"];
    grupos["Grupo C"];

    if (contador) {
        contador->clear();
        (*contador)["Grupo A"] = 0;
        (*contador)["Grupo B"] = 0;
        (*contador)["Grupo C"] = 0;
    }

    for (const auto& persona : personas) {
        if (!persona.getDeclaranteRenta()) continue;

        std::string grupo = grupoDIAN2025(persona);
        if (grupo == "Grupo A" || grupo == "Grupo B" || grupo == "Grupo C") {
            grupos[grupo].push_back(&persona);
            if (contador) ++(*contador)[grupo];
        }
    }
    return grupos;
}

Persona::CalendarioAgrupadito
Persona::agruparDeclarantesPorCalendarioValor(std::vector<Persona> personas)
{
    CalendarioAgrupadito resultado;


    resultado.grupos["Grupo A"] = std::vector<Persona>();
    resultado.grupos["Grupo B"] = std::vector<Persona>();
    resultado.grupos["Grupo C"] = std::vector<Persona>();

    resultado.conteo["Grupo A"] = 0;
    resultado.conteo["Grupo B"] = 0;
    resultado.conteo["Grupo C"] = 0;


    for (auto personaActual : personas)
    {
        if (!personaActual.getDeclaranteRenta())
        {
            continue;
        }


        std::string documento = personaActual.getId();

        int ultimo = -1;
        int penultimo = -1;

        for (int i = static_cast<int>(documento.size()) - 1; i >= 0; --i)
        {
            char ch = documento[i];

            if (ch >= '0' && ch <= '9')
            {
                if (ultimo == -1)
                {
                    ultimo = ch - '0';
                }
                else
                {
                    penultimo = ch - '0';
                    break;
                }
            }
        }

        int dosDigitos;

        if (ultimo == -1)
        {
            dosDigitos = -1;
        }
        else if (penultimo == -1)
        {
            dosDigitos = ultimo;
        }
        else
        {
            dosDigitos = penultimo * 10 + ultimo;
        }

        // asigación de grupitos
        std::string grupo;

        if (dosDigitos >= 0 && dosDigitos <= 39)
        {
            grupo = "Grupo A";
        }
        else if (dosDigitos >= 40 && dosDigitos <= 79)
        {
            grupo = "Grupo B";
        }
        else if (dosDigitos >= 80 && dosDigitos <= 99)
        {
            grupo = "Grupo C";
        }
        else
        {
            grupo = "Desconocido";
        }

        // Solo guardamos A/B/C
        if (grupo == "Grupo A" || grupo == "Grupo B" || grupo == "Grupo C")
        {
            resultado.grupos[grupo].push_back(personaActual);
            resultado.conteo[grupo] = resultado.conteo[grupo] + 1;
        }
    }

    return resultado;
}

bool Persona::validarAsignacionCalendario(const Persona& persona, const std::string& grupoEsperado) {
    if (!persona.getDeclaranteRenta()) return false;
    std::string grupo = grupoDIAN2025(persona);
    return grupo == grupoEsperado;
}

//===============================(4) Menor Patrimonio ==================================

void Persona::personaMinPatrimonio(const std::vector<Persona> &personas, Persona &minPatrimonio){
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }
    
    minPatrimonio = *std::min_element(personas.begin(), personas.end(),
                    [](const Persona &a, const Persona &b) {
                        return a.getPatrimonio() < b.getPatrimonio();
                    });
}

Persona Persona::personaMinPatrimonioValor(const std::vector<Persona> personas){
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }
    
    return *std::min_element(personas.begin(), personas.end(),
                    [](const Persona &a, const Persona &b) {
                        return a.getPatrimonio() < b.getPatrimonio();
                    });
}

//===============================(5) Mayor Deuda en el pais ==================================

void Persona::personaMaxDeuda(const std::vector<Persona> &personas, Persona &maxDeuda) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    maxDeuda = *std::max_element(personas.begin(), personas.end(),
            [](const Persona &a, const Persona &b)
            {
                return a.getDeudas() < b.getDeudas();
            });
}

Persona Persona::personaMaxDeudaValor(const std::vector<Persona> personas) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    return *std::max_element(personas.begin(), personas.end(),
            [](const Persona &a, const Persona &b)
            {
                return a.getDeudas() < b.getDeudas();
            });
}

//===============================(6) Declarantes por ciudad ==================================

std::map<std::string, std::vector<Persona>> Persona::declarantePorCiudadValor(const std::vector<Persona> personas) {
    std::map<std::string, std::vector<Persona>> resultado = Persona::agruparPorCiudadValor(personas);

    for (auto& par : resultado) {
        std::vector<Persona>& grupo = par.second;
        grupo.erase(std::remove_if(grupo.begin(), grupo.end(),
                    [](const Persona& p) {
                        return !p.getDeclaranteRenta();
                    }), grupo.end());
    }

    return resultado;
}

void Persona::declarantePorCiudad(const std::vector<Persona> &personas,
                                    std::map<std::string, std::vector<Persona>> &grupos) {

    Persona::agruparPorCiudad(personas, grupos);

    for (auto &par : grupos) {
        std::vector<Persona> &grupo = par.second;
        grupo.erase(std::remove_if(grupo.begin(), grupo.end(),
                    [](const Persona& p) {
                        return !p.getDeclaranteRenta();
                    }), grupo.end());
    }

}


int main()
{
    std::vector<Persona> personas = generarColeccion(1000000);

    std::map<std::string, std::vector<Persona>> agrupado;
    auto agrupado2 = Persona::declarantePorCiudadValor(personas);
    Persona::declarantePorCiudad(personas, agrupado);

    long total = 0;
    for (auto par: agrupado) {
        int declarantes = par.second.size();
        std::cout << "Ciudad: " << par.first << " | Declarantes: " << declarantes << "\n";
        total += declarantes;
    }

    std::cout << "Total de declarantes: " << total << "\n";

    std::cout << "=============================\n";

    total = 0;
    for (auto par: agrupado) {
        int declarantes = par.second.size();
        std::cout << "Ciudad: " << par.first << " | Declarantes: " << declarantes << "\n";
        total += declarantes;
    }

    std::cout << "Total de declarantes: " << total << "\n";

    // std::map<std::string, Persona> resultado =
    //     Persona::menorPatrimonioPorCiudad(personas);

    // std::cout << "Menor patrimonio por ciudad:\n\n";

    // for (auto par : resultado) // copia del par (por valor)
    // {
    //     std::string ciudad = par.first;
    //     Persona personaConMenorPatrimonio = par.second; // copia

    //     std::cout << "Ciudad: " << ciudad << "\n";
    //     std::cout << "  Nombre: " << personaConMenorPatrimonio.getNombre()
    //               << " " << personaConMenorPatrimonio.getApellido() << "\n";
    //     std::cout << "  ID: " << personaConMenorPatrimonio.getId() << "\n";
    //     std::cout << "  Patrimonio: " << personaConMenorPatrimonio.getPatrimonio() << "\n\n";
    //     }

    return 0;
}