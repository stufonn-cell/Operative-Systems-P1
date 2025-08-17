#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <cctype>

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
    anio = 1970; mes = 12; dia = 28;

    if (fechaISO.size() >= 10) {
        if (fechaISO[4] == '-' && fechaISO[7] == '-') {    // "YYYY-MM-DD"
            if (std::sscanf(fechaISO.c_str(), "%d-%d-%d", &anio, &mes, &dia) == 3) return;
        } else if (fechaISO[2] == '/' && fechaISO[5] == '/') { // "DD/MM/AAAA"
            int d=1, m=1, y=1970;
            if (std::sscanf(fechaISO.c_str(), "%d/%d/%d", &d, &m, &y) == 3) { anio=y; mes=m; dia=d; return; }
        }
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

inline int calcularEdad(const Persona& persona) {
    int anioN, mesN, diaN; descomponerFechaYMD(persona.fechaNacimiento, anioN, mesN, diaN);
    int anioH, mesH, diaH; obtenerHoyYMD(anioH, mesH, diaH);

    int edad = anioH - anioN;
    if (mesH < mesN || (mesH == mesN && diaH < diaN)) --edad;
    return edad;
}
//

//================================ Agrupar por ciudad ============================================
inline std::map<std::string, std::vector<Persona>>
agruparPorCiudadValor(const std::vector<Persona> personas) {

    std::map<std::string, std::vector<Persona>> grupos;
    for (const auto p : personas) grupos[p.ciudadNacimiento].push_back(p);

    return grupos;
}


inline void agruparPorCiudad(const std::vector<Persona>& personas,
    std::map<std::string, std::vector<Persona>>& grupos) {

    grupos.clear();
    for (const auto& p : personas) grupos[p.ciudadNacimiento].push_back(p);
}

// ================================ Agrupar por declaración ==========================================
inline void agruparPorDeclaracion(
    std::vector<Persona>& personas,
    std::map<std::string, std::vector<Persona>>& grupos)
{
    grupos.clear();
    for (auto& p : personas) {
        int dd = extraerUltimosDosDigitos(p.id);
        std::string etiqueta = grupoDIANDesdeDigitos(dd);
        if (etiqueta == "Grupo A" || etiqueta == "Grupo B" || etiqueta == "Grupo C") {
            grupos[etiqueta].push_back(p);
        }
    }
}

inline std::map<std::string, std::vector<Persona>> agruparPorDeclaracionValor(
std::vector<Persona> personas)
{
    std::map<std::string, std::vector<Persona>> grupos;
    
    grupos.clear();
    for (auto p : personas) {
        int dd = extraerUltimosDosDigitos(p.id);
        std::string etiqueta = grupoDIANDesdeDigitos(dd);
        if (etiqueta == "Grupo A" || etiqueta == "Grupo B" || etiqueta == "Grupo C") {
            grupos[etiqueta].push_back(p);
        }
    }

    return grupos;
}

// ***************************** (1) Persona Mas Longeva *************************************

inline void personaMaxLongeva(const std::vector<Persona> &personas, Persona &longeva){
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    longeva = *std::max_element(personas.begin(), personas.end(),
                [](const Persona &a, const Persona &b)
                {
                    return calcularEdad(a) < calcularEdad(b);
                });
}

inline Persona personaMaxLongevaValor(std::vector<Persona> personas) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    return *std::max_element(personas.begin(), personas.end(),
                [](const Persona &a, const Persona &b)
                {
                    return calcularEdad(a) < calcularEdad(b);
                });
}


// ***************************** (2) Persona con Mayor Patrimonio *************************************

inline void personaMaxPatrimonio(const std::vector<Persona> &personas, Persona &maxPatrimonio) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    maxPatrimonio = *std::max_element(personas.begin(), personas.end(),
                    [](const Persona &a, const Persona &b)
                    {
                        return a.patrimonio < b.patrimonio;
                    });
}

inline Persona personaMaxPatrimonioValor(std::vector<Persona> personas) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }
    return *std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b)
        {
        return a.patrimonio < b.patrimonio;
        }
    );
}

// ***************************** (4) Persona con Menor Patrimonio *************************************
inline void personaMinPatrimonio(const std::vector<Persona> &personas, Persona &minPatrimonio){
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }
    
    minPatrimonio = *std::min_element(personas.begin(), personas.end(),
                    [](const Persona &a, const Persona &b) {
                        return a.patrimonio < b.patrimonio;
                    });
}

inline Persona personaMinPatrimonioValor(const std::vector<Persona> personas){
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }
    
    return *std::min_element(personas.begin(), personas.end(),
                    [](const Persona &a, const Persona &b) {
                        return a.patrimonio < b.patrimonio;
                    });
}

// ***************************** (5) Mayor deuda en el pais  *************************************

inline void personaMaxDeuda(const std::vector<Persona> &personas, Persona &maxDeuda) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    maxDeuda = *std::max_element(personas.begin(), personas.end(),
            [](const Persona &a, const Persona &b)
            {
                return a.deudas < b.deudas;
            });
}

inline Persona personaMaxDeudaValor(const std::vector<Persona> personas) {
    if(personas.empty()){
        throw std::runtime_error("La lista está vacía");
    }

    return *std::max_element(personas.begin(), personas.end(),
            [](const Persona &a, const Persona &b)
            {
                return a.deudas < b.deudas;
            });
}

//===============================(6) Declarantes por ciudad ==================================

inline std::map<std::string, std::vector<Persona>> declarantePorCiudadValor(const std::vector<Persona> personas) {
    std::map<std::string, std::vector<Persona>> resultado = agruparPorCiudadValor(personas);

    for (auto& par : resultado) {
        std::vector<Persona>& grupo = par.second;
        grupo.erase(std::remove_if(grupo.begin(), grupo.end(),
                    [](const Persona& p) {
                        return !p.declaranteRenta;
                    }), grupo.end());
    }

    return resultado;
}

inline void declarantePorCiudad(const std::vector<Persona> &personas,
                                    std::map<std::string, std::vector<Persona>> &grupos) {

    agruparPorCiudad(personas, grupos);

    for (auto &par : grupos) {
        std::vector<Persona> &grupo = par.second;
        grupo.erase(std::remove_if(grupo.begin(), grupo.end(),
                    [](const Persona& p) {
                        return !p.declaranteRenta;
                    }), grupo.end());
    }
}


// ==================== Agrupar por calendario DIAN 2025 ===========================
struct CalendarioAgrupadito {
    std::map<std::string, std::vector<Persona>> grupos;
    std::map<std::string, int> conteo;
};

inline std::map<std::string, std::vector<const Persona*>>
agruparDeclarantesPorCalendario_ptr(const std::vector<Persona>& personas,
std::map<std::string, int>* contador = nullptr) {

    std::map<std::string, std::vector<const Persona*>> grupos;
    if (contador) { (*contador)["Grupo A"]=0; (*contador)["Grupo B"]=0; (*contador)["Grupo C"]=0; }

    for (const auto& personita : personas) {
        if (!personita.declaranteRenta)
            continue;
        std::string grupito = grupoDIAN2025(personita);
        if (grupito=="Grupo A" || grupito=="Grupo B" || grupito=="Grupo C") {
            grupos[grupito].push_back(&personita);
            if (contador) ++(*contador)[grupito];
        }
    }
    return grupos;
}


inline CalendarioAgrupadito
agruparDeclarantesPorCalendario_valor(const std::vector<Persona> personas) {
    CalendarioAgrupadito res;
    res.grupos["Grupo A"]; res.grupos["Grupo B"]; res.grupos["Grupo C"];
    res.conteo["Grupo A"]=0; res.conteo["Grupo B"]=0; res.conteo["Grupo C"]=0;

    for (const auto personita : personas) {
        if (!personita.declaranteRenta) continue;
        std::string grupito = grupoDIAN2025(personita);
        if (grupito=="Grupo A" || grupito=="Grupo B" || grupito=="Grupo C") {
            res.grupos[grupito].push_back(personita);
            ++res.conteo[grupito];
        }
    }
    return res;
}
#endif