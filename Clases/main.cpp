// main.cpp (versión clases)
#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include <map>
#include <ctime>

#include "persona.h"
#include "generador.h"
#include "monitor.h"

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::map;

/**
 * Muestra el menú principal de la aplicación.
 */
void mostrarMenu() {
    cout << "\n\n=== MENÚ PRINCIPAL ===";
    cout << "\n0. Crear nuevo conjunto de datos";
    cout << "\n1. Mostrar resumen de todas las personas";
    cout << "\n2. Mostrar detalle completo por índice";
    cout << "\n3. Buscar persona por ID";
    cout << "\n4. Ejecutar TODOS los métodos por APUNTADORES";
    cout << "\n5. Ejecutar TODOS los métodos por VALOR";
    cout << "\n6. Mostrar estadísticas de rendimiento";
    cout << "\n7. Exportar estadísticas a CSV";
    cout << "\n8. Salir";
    cout << "\nSeleccione una opción: ";
}

int main() {
    srand(time(nullptr)); // Semilla para generación aleatoria

    // Dataset bajo propiedad única
    std::unique_ptr<vector<Persona>> dataset = nullptr;

    Monitor monitor; // Medición de rendimiento

    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;

        // Variables comunes
        size_t totalRegistros = 0;
        int indice;
        string idBuscado;
        long memoria_inicio = 0;

        switch (opcion) {
            case 0: { // Crear nuevo conjunto de datos
                int n;
                cout << "\nIngrese el número de personas a generar: ";
                cin >> n;

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                if (n <= 0) {
                    cout << "Error: Debe generar al menos 1 persona\n";
                    break;
                }

                // Generar y mover al puntero inteligente
                auto nuevas = generarColeccion(n);
                totalRegistros = nuevas.size();
                dataset.reset(new vector<Persona>(std::move(nuevas)));

                // Métricas
                double t_ms = monitor.detener_tiempo();
                long mem_kb = monitor.obtener_memoria() - memoria_inicio;

                cout << "Generadas " << totalRegistros << " personas en "
                     << t_ms << " ms, Memoria: " << mem_kb << " KB\n";

                monitor.registrar("Crear datos", t_ms, mem_kb);
                break;
            }

            case 1: { // Mostrar resumen
                if (!dataset || dataset->empty()) {
                    cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                totalRegistros = dataset->size();
                cout << "\n=== RESUMEN DE PERSONAS (" << totalRegistros << ") ===\n";
                for (size_t i = 0; i < totalRegistros; ++i) {
                    cout << i << ". ";
                    (*dataset)[i].mostrarResumen();
                    cout << "\n";
                }

                double t_ms = monitor.detener_tiempo();
                long mem_kb = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar resumen", t_ms, mem_kb);
                break;
            }

            case 2: { // Detalle por índice
                if (!dataset || dataset->empty()) {
                    cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                totalRegistros = dataset->size();
                cout << "\nIngrese el índice (0-" << totalRegistros - 1 << "): ";
                if (cin >> indice) {
                    if (indice >= 0 && static_cast<size_t>(indice) < totalRegistros) {
                        const Persona& p = (*dataset)[static_cast<size_t>(indice)];
                        p.mostrar();
                        // Ejemplo de métodos de instancia:
                        cout << " -> Edad (método de instancia): " << p.calcularEdad() << "\n";
                        // Ejemplo de métodos estáticos:
                        cout << " -> Grupo DIAN 2025 (estático): "
                             << Persona::grupoDIAN2025(p) << "\n";
                    } else {
                        cout << "Índice fuera de rango!\n";
                    }
                } else {
                    cout << "Entrada inválida!\n";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                double t_ms = monitor.detener_tiempo();
                long mem_kb = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar detalle", t_ms, mem_kb);
                break;
            }

            case 3: { // Buscar por ID
                if (!dataset || dataset->empty()) {
                    cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                cout << "\nIngrese el ID a buscar: ";
                cin >> idBuscado;

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                // Se asume que buscarPorID está en generador.h/.cpp (devuelve const Persona* o nullptr)
                if (const Persona* p = buscarPorID(*dataset, idBuscado)) {
                    p->mostrar();
                    cout << " -> Edad: " << p->calcularEdad() << "\n";
                    cout << " -> Grupo DIAN 2025: " << Persona::grupoDIAN2025(*p) << "\n";
                } else {
                    cout << "No se encontró persona con ID " << idBuscado << "\n";
                }

                double t_ms = monitor.detener_tiempo();
                long mem_kb = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Buscar por ID", t_ms, mem_kb);
                break;
            }

            // ===================== APUNTADORES / REFERENCIAS =====================
            case 4: {
                if (!dataset || dataset->empty()) {
                    cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                try {
                    map<string, vector<Persona>> porCiudad;

                    Persona::agruparPorCiudad(*dataset, porCiudad);
                    cout << "[REF] agruparPorCiudad -> " << porCiudad.size() << " ciudades\n";


                    Persona masLongeva;
                    Persona::personaMaxLongeva(*dataset, masLongeva);
                    cout << "\n[REF] Más longeva en el país: "; masLongeva.mostrarResumen(); cout << "\n";

                    // Persona más longeva por ciudad
                    for (const auto& par : porCiudad) {
                        const string& ciudad = par.first;
                        const vector<Persona>& personas = par.second;
                        if (!personas.empty()) {
                            Persona masLongevaCiudad;
                            Persona::personaMaxLongeva(personas, masLongevaCiudad);
                            cout << "[REF] Más longeva en " << ciudad << ": ";
                            masLongevaCiudad.mostrarResumen();
                            cout << "\n";
                        }
                    }
                    
                    Persona mayorPatri;
                    Persona::personaMaxPatrimonio(*dataset, mayorPatri);
                    cout << "[REF] Mayor patrimonio en el país: "; mayorPatri.mostrarResumen(); cout << "\n";

                    for (const auto& par : porCiudad) {
                        const string& ciudad = par.first;
                        const vector<Persona>& personas = par.second;
                        if (!personas.empty()) {
                            Persona mayorPatrimonioCiudad;
                            Persona::personaMaxPatrimonio(personas, mayorPatrimonioCiudad);
                            cout << "[REF] Mayor patrimonio en " << ciudad << ": ";
                            mayorPatrimonioCiudad.mostrarResumen();
                            cout << "\n";
                        }
                    }

                    // Mayor patrimonio por grupo de declaración (A, B, C)
                    map<string, vector<Persona>> porDeclaracionGrupo;
                    Persona::agruparPorDeclaracion(*dataset, porDeclaracionGrupo);

                    for (const auto& par : porDeclaracionGrupo) {
                        const string& grupo = par.first;
                        const vector<Persona>& personas = par.second;
                        if (!personas.empty()) {
                            Persona mayorPatrimonioGrupo;
                            Persona::personaMaxPatrimonio(personas, mayorPatrimonioGrupo);
                            cout << "[REF] Mayor patrimonio en grupo " << grupo << ": ";
                            mayorPatrimonioGrupo.mostrarResumen();
                            cout << "\n";
                        }
                    }

                    Persona menorPatri;
                    Persona::personaMinPatrimonio(*dataset, menorPatri);
                    cout << "[REF] Menor patrimonio: "; menorPatri.mostrarResumen(); cout << "\n";

                    Persona mayorDeuda;
                    Persona::personaMaxDeuda(*dataset, mayorDeuda);
                    cout << "[REF] Mayor deuda: "; mayorDeuda.mostrarResumen(); cout << "\n";
                } catch (const std::exception& e) {
                    cout << "Error en búsquedas por referencia: " << e.what() << "\n";
                }

                // Declarantes por ciudad (ref)
                {
                    map<string, vector<Persona>> declPorCiudad;
                    Persona::declarantePorCiudad(*dataset, declPorCiudad);
                    cout << "[REF] declarantePorCiudad -> " << declPorCiudad.size()
                         << " ciudades con declarantes\n";
                }

                {
                    map<string, int> contador;
                    auto gruposPtr = Persona::agruparDeclarantesPorCalendarioPtr(*dataset, &contador);
                    cout << "[PTR] Calendario -> A:" << contador["Grupo A"]
                         << " B:" << contador["Grupo B"]
                         << " C:" << contador["Grupo C"] << "\n";

                    for (auto etiqueta : {"Grupo A","Grupo B","Grupo C"}) {
                        auto it = gruposPtr.find(etiqueta);
                        if (it != gruposPtr.end() && !it->second.empty()) {
                            cout << "  [PTR] Ejemplo " << etiqueta << ": ";
                            it->second.front()->mostrarResumen();
                            cout << "\n";
                        }
                    }
                }

                double t_ms = monitor.detener_tiempo();
                long mem_kb = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Todos por APUNTADORES/REF", t_ms, mem_kb);
                break;
            }

            // ===================== POR VALOR =====================
            case 5: {
                if (!dataset || dataset->empty()) {
                    cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }

                monitor.iniciar_tiempo();
                memoria_inicio = monitor.obtener_memoria();

                try {
                    Persona a = Persona::personaMaxLongevaValor(*dataset);
                    cout << "\n[VALOR] Más longeva: "; a.mostrarResumen(); cout << "\n";

                    Persona b = Persona::personaMaxPatrimonioValor(*dataset);
                    cout << "[VALOR] Mayor patrimonio: "; b.mostrarResumen(); cout << "\n";

                    Persona c = Persona::personaMinPatrimonioValor(*dataset);
                    cout << "[VALOR] Menor patrimonio: "; c.mostrarResumen(); cout << "\n";

                    Persona d = Persona::personaMaxDeudaValor(*dataset);
                    cout << "[VALOR] Mayor deuda: "; d.mostrarResumen(); cout << "\n";
                } catch (const std::exception& e) {
                    cout << "Error en búsquedas por valor: " << e.what() << "\n";
                }

                auto gCiudad = Persona::agruparPorCiudadValor(*dataset);
                cout << "[VALOR] agruparPorCiudadValor -> " << gCiudad.size() << " ciudades\n";

                for (const auto& par : gCiudad) {
                    const string& ciudad = par.first;
                    const vector<Persona>& personas = par.second;
                    if (!personas.empty()) {
                        Persona masLongevaCiudad = Persona::personaMaxLongevaValor(personas);
                        cout << "[VALOR] Más longeva en " << ciudad << ": ";
                        masLongevaCiudad.mostrarResumen();
                        cout << "\n";
                    }
                }

                auto grupoDecl = Persona::agruparPorDeclaracionValor(*dataset);

                for (const auto& par : gCiudad) {
                    const string& ciudad = par.first;
                    const vector<Persona>& personas = par.second;
                    if (!personas.empty()) {
                        Persona mayorPatrimonioCiudad = Persona::personaMaxPatrimonioValor(personas);
                        cout << "[VALOR] Mayor patrimonio en " << ciudad << ": ";
                        mayorPatrimonioCiudad.mostrarResumen();
                        cout << "\n";
                    }
                }

                for (const auto& par : grupoDecl) {
                    const string& grupo = par.first;
                    const vector<Persona>& personas = par.second;
                    if (!personas.empty()) {
                        Persona mayorPatrimonioGrupo = Persona::personaMaxPatrimonioValor(personas);
                        cout << "[VALOR] Mayor patrimonio en grupo " << grupo << ": ";
                        mayorPatrimonioGrupo.mostrarResumen();
                        cout << "\n";
                    }
                }

                auto declCiudad = Persona::declarantePorCiudadValor(*dataset);
                cout << "[VALOR] declarantePorCiudadValor -> " << declCiudad.size()
                     << " ciudades con declarantes\n";

                // Calendario por valor (estructura con conteo)
                Persona::CalendarioAgrupadito cal = Persona::agruparDeclarantesPorCalendarioValor(*dataset);
                cout << "[VALOR] Calendario -> A:" << cal.conteo["Grupo A"]
                     << " B:" << cal.conteo["Grupo B"]
                     << " C:" << cal.conteo["Grupo C"] << "\n";

                // Agrupar por declaración (por valor) — usa claves "A","B","C"
                auto gruposDecl = Persona::agruparPorDeclaracionValor(*dataset);
                cout << "[VALOR] agruparPorDeclaracionValor -> "
                     << "A:" << gruposDecl["A"].size()
                     << " B:" << gruposDecl["B"].size()
                     << " C:" << gruposDecl["C"].size() << "\n";

                double t_ms = monitor.detener_tiempo();
                long mem_kb = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Todos por VALOR", t_ms, mem_kb);
                break;
            }

            case 6: // Mostrar estadísticas
                monitor.mostrar_resumen();
                break;

            case 7: // Exportar CSV
                monitor.exportar_csv();
                break;

            case 8: // Salir
                cout << "Saliendo...\n";
                break;

            default:
                cout << "Opción inválida!\n";
        }


        if (opcion >= 0 && opcion <= 5) {
            double t_ms = monitor.detener_tiempo();
            long mem_kb = monitor.obtener_memoria(); // lectura directa
            monitor.mostrar_estadistica("Opción " + std::to_string(opcion), t_ms, mem_kb);
        }

    } while (opcion != 8);

    return 0;
}
