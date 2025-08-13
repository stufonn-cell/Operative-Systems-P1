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

public:
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
    static std::string personaMaxLongeva(std::vector<Persona> &personas);
    static std::string personaMaxLongevaValor(std::vector<Persona> personas);
    static std::map<std::string, std::vector<Persona>> agruparPorCiudad(std::vector<Persona> &personas);
};

#endif // PERSONA_H