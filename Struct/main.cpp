#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include "persona.h"
#include "generador.h"
#include "monitor.h"
#include <ctime>

/**
 * Muestra el menú principal de la aplicación.
 *
 * POR QUÉ: Guiar al usuario a través de las funcionalidades disponibles.
 * CÓMO: Imprimiendo las opciones en consola.
 * PARA QUÉ: Interacción amigable con el usuario.
 */
void mostrarMenu() {
    std::cout << "\n\n=== MENÚ PRINCIPAL ===";
    std::cout << "\n0. Crear nuevo conjunto de datos";
    std::cout << "\n1. Mostrar resumen de todas las personas";
    std::cout << "\n2. Mostrar detalle completo por índice";
    std::cout << "\n3. Buscar persona por ID";
    std::cout << "\n4. Ejecutar TODOS los métodos por APUNTADORES";
    std::cout << "\n5. Ejecutar TODOS los métodos por VALOR";
    std::cout << "\n6. Mostrar estadísticas de rendimiento";
    std::cout << "\n7. Exportar estadísticas a CSV";
    std::cout << "\n8. Salir";
    std::cout << "\nSeleccione una opción: ";
}

/**
 * Punto de entrada principal del programa.
 *
 * POR QUÉ: Iniciar la aplicación y manejar el flujo principal.
 * CÓMO: Mediante un bucle que muestra el menú y procesa la opción seleccionada.
 * PARA QUÉ: Ejecutar las funcionalidades del sistema.
 */
int main() {
    srand(time(nullptr)); // Semilla para generación aleatoria

    // Puntero inteligente para gestionar la colección de personas
    std::unique_ptr<std::vector<Persona>> personas = nullptr;

    Monitor monitor; // Monitor para medir rendimiento

    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;

        // Variables locales para uso en los casos
        size_t tam = 0;
        int indice;
        std::string idBusqueda;
        long memoria_inicio = 0;

        switch(opcion) {
            case 0: { // Crear nuevo conjunto de datos
                int n;
                std::cout << "\nIngrese el número de personas a generar: ";
                std::cin >> n;

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                if (n <= 0) {
                    std::cout << "Error: Debe generar al menos 1 persona\n";
                    break;
                }

                // Generar el nuevo conjunto de personas
                auto nuevasPersonas = generarColeccion(n);
                tam = nuevasPersonas.size();

                // Mover el conjunto al puntero inteligente (propiedad única)
                personas.reset(new std::vector<Persona>(std::move(nuevasPersonas)));

                // Medir tiempo y memoria usada
                double tiempo_gen = monitor.detener_tiempo();
                long memoria_gen = monitor.obtener_memoria() - memoria_inicio;

                std::cout << "Generadas " << tam << " personas en "
                          << tiempo_gen << " ms, Memoria: " << memoria_gen << " KB\n";

                // Registrar la operación
                monitor.registrar("Crear datos", tiempo_gen, memoria_gen);
                break;
            }

            case 1: { // Mostrar resumen de todas las personas
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                tam = personas->size();
                std::cout << "\n=== RESUMEN DE PERSONAS (" << tam << ") ===\n";
                for(size_t i = 0; i < tam; ++i) {
                    std::cout << i << ". ";
                    (*personas)[i].mostrarResumen();
                    std::cout << "\n";
                }

                double tiempo_mostrar = monitor.detener_tiempo();
                long memoria_mostrar = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar resumen", tiempo_mostrar, memoria_mostrar);
                break;
            }

            case 2: { // Mostrar detalle por índice
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                tam = personas->size();
                std::cout << "\nIngrese el índice (0-" << tam-1 << "): ";
                if(std::cin >> indice) {
                    if(indice >= 0 && static_cast<size_t>(indice) < tam) {
                        (*personas)[indice].mostrar();
                    } else {
                        std::cout << "Índice fuera de rango!\n";
                    }
                } else {
                    std::cout << "Entrada inválida!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar detalle", tiempo_detalle, memoria_detalle);
                break;
            }

            case 3: { // Buscar por ID
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                std::cout << "\nIngrese el ID a buscar: ";
                std::cin >> idBusqueda;

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                if(const Persona* encontrada = buscarPorID(*personas, idBusqueda)) {
                    encontrada->mostrar();
                } else {
                    std::cout << "No se encontró persona con ID " << idBusqueda << "\n";
                }

                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Buscar por ID", tiempo_busqueda, memoria_busqueda);
                break;
            }
 // ===================== APUNTADORES =====================
            case 4: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                // Búsqueda por apuntador
                try {
                    Persona longeva;
                    personaMaxLongeva(*personas, longeva);
                    std::cout << "\n[APUNTADOR] Más longeva : "; longeva.mostrarResumen(); std::cout << "\n";

                    Persona max_patri;
                    personaMaxPatrimonio(*personas, max_patri);
                    std::cout << "[APUNTADOR] Mayor patrimonio  : "; max_patri.mostrarResumen(); std::cout << "\n";

                    Persona min_patri;
                    personaMinPatrimonio(*personas, min_patri);
                    std::cout << "[APUNTADOR] Menor patrimonio  : "; min_patri.mostrarResumen(); std::cout << "\n";

                    Persona max_deu;
                    personaMaxDeuda(*personas, max_deu);
                    std::cout << "[APUNTADOR] Mayor deuda  : "; max_deu.mostrarResumen(); std::cout << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error en búsquedas por valor: " << e.what() << "\n";
                }


                // Agrupaciones por apuntadores
                std::map<std::string, std::vector<Persona>> gruposCiudad;
                agruparPorCiudad(*personas, gruposCiudad);
                std::cout << "[APUNTADOR] agruparPorCiudadValor -> " << gruposCiudad.size() << " ciudades\n";

                std::map<std::string, std::vector<Persona>> declaranteCiudad;
                declarantePorCiudad(*personas, declaranteCiudad);
                std::cout << "[APUNTADOR] declarantePorCiudadValor -> " << declaranteCiudad.size() << " ciudades con declarantes\n";

                std::map<std::string, std::vector<Persona>> gruposPtr;

                for (const auto& [ciudad, personasEnCiudad] : gruposCiudad) {
                    if (!personasEnCiudad.empty()) {
                        Persona longevaCiudad;
                        personaMaxLongeva(personasEnCiudad, longevaCiudad);
                        std::cout << "[APUNTADOR] Más longeva en " << ciudad << ": ";
                        longevaCiudad.mostrarResumen();
                        std::cout << "\n";
                    }
                }

                // Mayor patrimonio por ciudad (por apuntador)
                for (const auto& [ciudad, personasEnCiudad] : gruposCiudad) {
                    if (!personasEnCiudad.empty()) {
                        Persona maxPatrimonioCiudad;
                        personaMaxPatrimonio(personasEnCiudad, maxPatrimonioCiudad);
                        std::cout << "[APUNTADOR] Mayor patrimonio en " << ciudad << ": ";
                        maxPatrimonioCiudad.mostrarResumen();
                        std::cout << "\n";
                    }
                }

                agruparPorDeclaracion(*personas, gruposPtr);

                // Mayor patrimonio por declaracion (por apuntador)
                for (auto par : gruposPtr) {
                    const auto& grupo = par.first;
                    const auto personasGrupo = par.second;
                    if (!personasGrupo.empty()) {
                        Persona maxPatrimonioGrupo;
                        personaMaxPatrimonio(personasGrupo, maxPatrimonioGrupo);
                        std::cout << "[APUNTADOR] Mayor patrimonio en " << grupo << ": ";
                        maxPatrimonioGrupo.mostrarResumen();
                        std::cout << "\n";
                    }
                }
                
                std::cout << "[PTR] agruparPorDeclaracion -> "
                          << "A:" << gruposPtr["Grupo A"].size()
                          << " B:" << gruposPtr["Grupo B"].size()
                          << " C:" << gruposPtr["Grupo C"].size() << "\n";

                // Calendario por punteros
                std::map<std::string,int> contador;
                auto calPtr = agruparDeclarantesPorCalendario_ptr(*personas, &contador);
                std::cout << "[PTR] calendario -> A:" << contador["Grupo A"]
                << " B:" << contador["Grupo B"]
                << " C:" << contador["Grupo C"] << "\n";

                double t = monitor.detener_tiempo();
                long mem = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Todos por APUNTADORES", t, mem);
                break;
            }

// ===================== NUEVO CASE: por VALOR =====================
            case 5: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                // BÚSQUEDAS por valor
                try {
                    Persona a = personaMaxLongevaValor(*personas);
                    std::cout << "\n[VALOR] Más longeva  : "; a.mostrarResumen(); std::cout << "\n";

                    Persona b = personaMaxPatrimonioValor(*personas);
                    std::cout << "[VALOR] Mayor patrimonio  : "; b.mostrarResumen(); std::cout << "\n";

                    Persona c = personaMinPatrimonioValor(*personas);
                    std::cout << "[VALOR] Menor patrimonio  : "; c.mostrarResumen(); std::cout << "\n";

                    Persona d = personaMaxDeudaValor(*personas);
                    std::cout << "[VALOR] Mayor deuda  : "; d.mostrarResumen(); std::cout << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error en búsquedas por valor: " << e.what() << "\n";
                }

                // AGRUPACIONES por valor
                auto gCiudad = agruparPorCiudadValor(*personas);
                std::cout << "[VALOR] agruparPorCiudadValor -> " << gCiudad.size() << " ciudades\n";

                for (const auto& [ciudad, personasEnCiudad] : gCiudad) {
                    if (!personasEnCiudad.empty()) {
                        Persona maxPatrimonioCiudad = personaMaxPatrimonioValor(personasEnCiudad);
                        std::cout << "[VALOR] Mayor patrimonio en " << ciudad << ": ";
                        maxPatrimonioCiudad.mostrarResumen();
                        std::cout << "\n";
                    }
                }
                
                auto grupoDecl = agruparPorDeclaracionValor(*personas);

                for (const auto& [grupo, personasGrupo] : grupoDecl) {
                    if (!personasGrupo.empty()) {
                        Persona maxPatrimonioGrupo = personaMaxPatrimonioValor(personasGrupo);
                        std::cout << "[VALOR] Mayor patrimonio en " << grupo << ": ";
                        maxPatrimonioGrupo.mostrarResumen();
                        std::cout << "\n";
                    }
                }


                auto declCiudad = declarantePorCiudadValor(*personas);
                std::cout << "[VALOR] declarantePorCiudadValor -> " << declCiudad.size() << " ciudades con declarantes\n";

                // Calendario por valor
                CalendarioAgrupadito cal = agruparDeclarantesPorCalendario_valor(*personas);
                std::cout << "[VALOR] calendario -> A:" << cal.conteo["Grupo A"]
                          << " B:" << cal.conteo["Grupo B"]
                          << " C:" << cal.conteo["Grupo C"] << "\n";

                std::map<std::string, std::vector<Persona>> dummy = agruparPorDeclaracionValor(*personas);
                std::cout << "[VALOR] agruparPorDeclaracionValor (invocada)\n";

                double t = monitor.detener_tiempo();
                long mem = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Todos por VALOR", t, mem);
                break;
            }

            case 6: // Mostrar estadísticas de rendimiento
                monitor.mostrar_resumen();
                break;

            case 7: // Exportar estadísticas a CSV
                monitor.exportar_csv();
                break;

            case 8: // Salir
                std::cout << "Saliendo...\n";
                break;

            default:
                std::cout << "Opción inválida!\n";
        }

        // Mostrar estadísticas de la operación (excepto para opciones 4,5,8)
        if (opcion >= 0 && opcion <= 5) {
            double tiempo = monitor.detener_tiempo();
            long memoria = monitor.obtener_memoria(); // sin delta aquí por simplicidad
            monitor.mostrar_estadistica("Opción " + std::to_string(opcion), tiempo, memoria);
        }

    } while(opcion != 8);

    return 0;
}
