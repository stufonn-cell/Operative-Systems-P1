#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include <iomanip>

// Estructura que representa una persona con datos personales y fiscales
struct Persona {
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

    // --- Métodos de visualización ---
    void mostrar() const;         // Muestra todos los detalles completos
    void mostrarResumen() const;  // Muestra versión compacta para listados
};

// Implementación de métodos inline para mantener la estructura simple
inline void Persona::mostrar() const {
    std::cout << "-------------------------------------\n";
    std::cout << "[" << id << "] Nombre: " << nombre << " " << apellido << "\n";
    std::cout << "   - Ciudad de nacimiento: " << ciudadNacimiento << "\n";
    std::cout << "   - Fecha de nacimiento: " << fechaNacimiento << "\n\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "   - Ingresos anuales: $" << ingresosAnuales << "\n";
    std::cout << "   - Patrimonio: $" << patrimonio << "\n";
    std::cout << "   - Deudas: $" << deudas << "\n";
    std::cout << "   - Declarante de renta: " << (declaranteRenta ? "Sí" : "No") << "\n";
}

inline void Persona::mostrarResumen() const {
    std::cout << "[" << id << "] " << nombre << " " << apellido
              << " | " << ciudadNacimiento 
              << " | $" << std::fixed << std::setprecision(2) << ingresosAnuales;
}

inline void descomponerFechaYMD(const std::string& fechaISO, int& anio, int& mes, int& dia) {
    anio = 0; mes = 0; dia = 0;
    if (fechaISO.size() >= 10) {
        anio = std::stoi(fechaISO.substr(0, 4));
        mes  = std::stoi(fechaISO.substr(5, 2));
        dia  = std::stoi(fechaISO.substr(8, 2));
    }
}

inline void obtenerHoyYMD(int& anio, int& mes, int& dia) {
    anio = 2025; mes = 12; dia = 28; // fija para resultados deterministas
}

inline int extraerUltimosDosDigitos(const std::string& documentoIdentidad) {
    int ultimo = -1, penultimo = -1;
    for (int i = static_cast<int>(documentoIdentidad.size()) - 1; i >= 0; --i) {
        unsigned char c = static_cast<unsigned char>(documentoIdentidad[i]);
        if (std::isdigit(c)) {
            if (ultimo == -1) ultimo = c - '0';
            else { penultimo = c - '0'; break; }
        }
    }
    if (ultimo == -1) return -1;
    if (penultimo == -1) return ultimo;
    return penultimo * 10 + ultimo;
}

inline std::string grupoDIANDesdeDigitos(int ultimosDosDigitos) {
    if (ultimosDosDigitos < 0)   return "Desconocido";
    if (ultimosDosDigitos <= 39) return "Grupo A";
    if (ultimosDosDigitos <= 79) return "Grupo B";
    if (ultimosDosDigitos <= 99) return "Grupo C";
    return "Desconocido";
}

inline std::string grupoDIAN2025(const Persona& persona) {
    int ultimosDos = extraerUltimosDosDigitos(persona.id);
    return grupoDIANDesdeDigitos(ultimosDos);
}

#endif // PERSONA_H