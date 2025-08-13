#ifndef PERSONA_H
#define PERSONA_H

#include <map>
#include <string>
#include <vector>

// Clase que representa una persona con datos personales y fiscales
class Persona {
private:
    // Datos básicos de identificación
    std::string nombre;           // Nombre de pila
    std::string apellido;         // Apellidos
    std::string id;               // Identificador único
    std::string ciudadNacimiento; // Ciudad de nacimiento en Colombia
    std::string fechaNacimiento;  // Fecha en formato DD/MM/AAAA

    // Datos fiscales y económicos
    double ingresosAnuales;       // Ingresos anuales en pesos colombianos
    double patrimonio;            // Valor total de bienes y activos
    double deudas;                // Deudas pendientes
    bool declaranteRenta;         // Si está obligado a declarar renta
    static int ultimosDosDigitosCC(const std::string& id);
    static std::string grupoDIANusandoDigitos(int dd);
public:

    class CalendarioAgrupadito {
        public:
            std::map<std::string, std::vector<Persona>> grupos;
            std::map<std::string, int> conteo;
        };
    // Constructor: Inicializa todos los campos de la persona
    Persona(std::string nom, std::string ape, std::string id,
            std::string ciudad, std::string fecha, double ingresos,
            double patri, double deud, bool declara);

    // --- Métodos de acceso (getters) ---
    // Permiten obtener valores de campos privados sin exponer implementación
    std::string getNombre() const;
    std::string getApellido() const;
    std::string getId() const;
    std::string getCiudadNacimiento() const;
    std::string getFechaNacimiento() const;
    double getIngresosAnuales() const;
    double getPatrimonio() const;
    double getDeudas() const;
    bool getDeclaranteRenta() const;

    // --- Métodos de visualización ---
    void mostrar() const;         // Muestra todos los detalles completos
    void mostrarResumen() const;  // Muestra versión compacta para listados
    int calcularEdad() const; // Calcula la edad a partir de la fecha de nacimiento
    static std::map<std::string, std::vector<Persona>> agruparPorCiudad(std::vector<Persona> &personas);
    static std::map<std::string, std::vector<Persona>> agruparPorDeclaracion(std::vector<Persona> &personas);
    static void personaMaxLongeva(std::vector<Persona> &personas, Persona &longeva);
    static Persona personaMaxLongevaValor(std::vector<Persona> personas);
    static void personaMaxPatrimonio(std::vector<Persona> &personas, Persona &maxPatrimonio);
    static Persona personaMaxPatrimonioValor(std::vector<Persona> &personas);
    static std::string grupoDIAN2025(const Persona& persona);
    static std::map<std::string, std::vector<const Persona*>>
    agruparDeclarantesPorCalendarioPtr(const std::vector<Persona>& personas,
                                    std::map<std::string, int>* contador = nullptr);
    static std::map<std::string, int>
    contarDeclarantesPorCalendario(const std::vector<Persona>& personas);
    static bool validarAsignacionCalendario(const Persona& p, const std::string& grupoEsperado);
    static CalendarioAgrupadito
    agruparDeclarantesPorCalendarioValor(std::vector<Persona> personas);
};

#endif // PERSONA_H